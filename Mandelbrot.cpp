/*
 * Mandelbrot.cpp
 *
 *  Created on: Dec 2, 2019
 *      Author: joseph
 */

#include "Mandelbrot.h"

Mandelbrot::Mandelbrot() {
	this->width = 0;
	this->height = 0;

	this->minX = 0;
	this->maxX = 0;
	this->minY = 0;
	this->maxY = 0;

	numOfCombinedBits = 0;
	iterations = 34;
}

Mandelbrot::Mandelbrot(unsigned width, unsigned height) {
	this->width = width;
	this->height = height;

	this->minX = 0;
	this->maxX = 0;
	this->minY = 0;
	this->maxY = 0;

	numOfCombinedBits = HelperFunctions::getInstance()->gd(width, 30);
	iterations = 34;
}

Mandelbrot::Mandelbrot(unsigned width, unsigned height, unsigned minX, unsigned maxX, unsigned minY, unsigned maxY) {
	this->width = width;
	this->height = height;

	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;

	numOfCombinedBits = HelperFunctions::getInstance()->gd(width, 30);
	iterations = 34;
}

Mandelbrot::Mandelbrot(unsigned width, unsigned height, unsigned minX, unsigned maxX, unsigned minY, unsigned maxY, int iterations) {
	this->width = width;
	this->height = height;

	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;

	numOfCombinedBits = HelperFunctions::getInstance()->gd(width, 30);
	this->iterations = iterations;
}

Mandelbrot::~Mandelbrot() {
}

void Mandelbrot::setCompressionLevel(int numOfCombinedBits) {
	this->numOfCombinedBits = HelperFunctions::getInstance()->gd(width, numOfCombinedBits);
}

void Mandelbrot::calculateCompressedImage(std::string &returnBuf) {

	// fractal properties
	double MinRe = -1.2;
	double MaxRe = 1.2;
	double MinIm = -1.2;
	double MaxIm = 1.2;
	double Re_facor = (MaxRe - MinRe) / (width - 1);
	double Im_factor = (MaxIm - MinIm) / (height - 1);

	// implementation of the mathematical limes -> to infinite (in this case 34)
	unsigned MaxIterations = this->iterations;

	// compression buffer
	int bufIndex = 0;
	int valCoded = 0;

	for (unsigned y = minY; y < maxY; ++y) {
		double c_im = MaxIm - y * Im_factor;

		for (unsigned x = minX; x < maxX; ++x) {
			double c_re = MinRe + x * Re_facor - 0.65; // image on Re-axis move with 0.65

			double Z_re = c_re, Z_im = c_im;
			bool isInside = true;

			for (unsigned n = 0; n < MaxIterations; ++n) {
				double Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4) {
					isInside = false;
					break;
				}

				Z_im = 2 * Z_re * Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			if (isInside) {
				valCoded = (valCoded << 1) + 1;
			} else {
				valCoded = (valCoded << 1);
			}

			bufIndex++;

			if(bufIndex > numOfCombinedBits - 1) {
				bufIndex = 0;
				returnBuf += std::to_string(valCoded) + "\n";
				valCoded = 0;
			}
		}
	}
}

void Mandelbrot::calculateImage(PPMImage &image) {

	// fractal properties
	double MinRe = -1.2;
	double MaxRe = 1.2;
	double MinIm = -1.2;
	double MaxIm = 1.2;
	double Re_facor = (MaxRe - MinRe) / (width - 1);
	double Im_factor = (MaxIm - MinIm) / (height - 1);

	// implementation of the mathematical limes -> to infinite (in this case 34)
	unsigned MaxIterations = this->iterations;

	for (unsigned y = minY; y < maxY; ++y) {
		double c_im = MaxIm - y * Im_factor;

		for (unsigned x = minX; x < maxX; ++x) {
			double c_re = MinRe + x * Re_facor - 0.65; // image on Re-axis move with 0.65

			double Z_re = c_re, Z_im = c_im;
			bool isInside = true;

			for (unsigned n = 0; n < MaxIterations; ++n) {
				double Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4) {
					isInside = false;
					break;
				}

				Z_im = 2 * Z_re * Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			if (isInside) {
				// rotating the image (left orientated) -> x and y change
				image[y][x].r = 0;
				image[y][x].g = 0;
				image[y][x].b = 0;
			} else {
				image[y][x].r = 1;
				image[y][x].g = 1;
				image[y][x].b = 1;
			}
		}
	}
}

void Mandelbrot::createPPMFile(std::string &filename) {
	std::cout << "Creating PPM image file " << width << "x" << height << std::endl;

	std::ofstream out(filename);
	out << "P3" << std::endl << width << " " << height << std::endl << 1 << std::endl << std::endl;
}

void Mandelbrot::writeToPPMFile(std::string &filename, std::string &content) {
	std::ofstream out;

	// std::ios::app is the open mode "append" meaning
	// new data will be written to the end of the file.
	out.open(filename, std::ios::app);

	out << content;
}
