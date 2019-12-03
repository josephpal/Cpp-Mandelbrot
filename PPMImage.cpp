/*
 * PPMImage.cpp
 *
 *  Created on: Dec 2, 2019
 *      Author: joseph
 */

#include "PPMImage.h"

PPMImage::PPMImage(const size_t height, const size_t width) : Matrix(height, width) { }

void PPMImage::save(const std::string &filename) {
	std::cout << "Saving to image " << filename << " ..." << std::endl;

	std::ofstream out(filename);
	out << "P3" << std::endl
		<< _cols << " " << _rows << std::endl
		<< 1 << std::endl << std::endl;
	for (size_t y = 0; y < _rows; y++)
		for (size_t x = 0; x < _cols; x++)
			out << _matrix[y][x].r << " " << _matrix[y][x].g << " " << _matrix[y][x].b << std::endl;

	std::cout << "done.\n" << std::endl;
}

void PPMImage::codeImg(const std::string &filename) {

	std::cout << "Compressing and saving to coded image " << filename << " ..." << std::endl;

	std::ofstream out(filename);
	out << "P3" << std::endl
		<< _cols << " " << _rows << std::endl
		<< 1 << std::endl << std::endl;

	// combine maximal compressionLevel bits of the bitstream to a new number
	// e.g. 0110 0010 ... -> 10 2 ...
	int numOfCombined = HelperFunctions::getInstance()->gd((int)_cols, compressionLevel);

	std::cout << "compression level (bits combined): " << numOfCombined << std::endl;

	for (size_t y = 0; y < _rows; y++) {
		for (size_t x = 0; x < _cols; x += numOfCombined) {
			std::string tmp = "";
			int valCoded = 0;

			for ( int i = 0; i < numOfCombined; i++ ) {
				tmp += std::to_string(_matrix[y][x + i].r);
			}

			valCoded = std::stoi(tmp, 0, 2);

			if ( valCoded != 0 ) {
				// std::cout << "buf:\t" << tmp << std::endl;
				// std::cout << "coded:\t" << valCoded << std::endl;
			}

			out << valCoded << std::endl;
		}
	}

	std::cout << "Compressed from " << _rows*_cols << " to " << ((_rows * _cols) / numOfCombined) << " characters -> done." << std::endl;
	std::cout << "done.\n" << std::endl;
}

void PPMImage::decodeImg(const std::string &inputFile, const std::string &filename) {
	std::ifstream in(inputFile);
	std::string line = "";
	std::string result = "";

	int index = 0;
	int numOfCombined = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	std::cout << "Reading compressed *.ppm file ..." << std::endl;

	if (in.is_open()) {
		while ( std::getline (in,line) ){

			if( index < 3 ) {
				// header
				// std::cout << line << '\n';

				if( index == 1) {
					std::string delimiter = " ";

					width = atoi(line.substr(0, line.find(delimiter)).c_str());
					height = atoi(line.substr(line.find(delimiter) + 1, line.length()).c_str());

					std::cout << "width: " << width << ", height: " << height << std::endl;

					numOfCombined = HelperFunctions::getInstance()->gd(width, compressionLevel);

					std::cout << "compression level (bits combined): " << numOfCombined << std::endl;
				} else if( index == 2 ) {
					std::cout << "max color code: " << line << std::endl << std::endl;
					std::cout << "Uncompressing ..." << std::endl;
				}

			} else {
				result = HelperFunctions::getInstance()->decToBinary(line, numOfCombined);

				if( result != "" ) {
					for (int i = 0; i < numOfCombined; ++i) {
						if ( result[i] == '1' ) {
							// rotate left -> change x and y
							_matrix[x][y].r = 1;
							_matrix[x][y].g = 0;
							_matrix[x][y].b = 0;
						} else {
							_matrix[x][y].r = 0;
							_matrix[x][y].g = 0;
							_matrix[x][y].b = 0;
						}

						// incrementing x as column index of the image matrix
						x++;

						if( x == width ) {
							// width RGB values are written in one row of the matrix
							// -> index x to 0, and start new row
							// -> y increment as row index
							y++;
							x = 0;
						}
					}
				}
			}

			index++;
		}

		in.close();

		// first 4 lines of the *.ppm file contains the header information (no compressed bits)
	    std::cout << "Decompressed " << numOfCombined * (index - 4) << " characters -> done.\n" << std::endl;

	    save(filename);
	} else {
		std::cout << "Unable to open file" << std::endl;
	}
}
