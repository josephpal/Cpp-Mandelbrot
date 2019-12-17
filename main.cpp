#include <iostream>
#include <thread>
#include <vector>
#include <math.h>
#include <string.h>

#include "PPMImage.h"
#include "Mandelbrot.h"

void createMandelbrotImageThread(Mandelbrot * mandelbrot, PPMImage &image) {
	mandelbrot->calculateImage(image);
}

void createMandelbrotImage(PPMImage &image, unsigned int width, unsigned int height, int numOfThreads, int iterations) {

	/*
	 * 	sub image coordinate computation (e.g. 600x600, 4 threads) -> chess board method
	 *
	 *	-- x
	 *	|
	 *	y
	 *
	 *	P0a(0|0)		P2a(300|0)
	 *		x_______________x_______________
	 * 		|				|				|
	 * 		|				|				|
	 * 		|		0		|		2		|
	 * 		|				|				|
	 * 		|_______________|_______________|
	 *						x				x
	 * 				   P0e(300|300)	   P2e(600|300)
	 *
	 * 					  ------
	 *
	 * 	P1a(0|300)	 P3a(300|300)
	 * 		x_______________x_______________
	 * 		|				|				|
	 * 		|				|				|
	 * 		|		1		|		3		|
	 * 		|				|				|
	 * 		|_______________|_______________|
	 *						x				x
	 * 				   P1e(300|600)	   P3e(600|600)
	 *
	 */

	std::cout << "Creating image ...\n";

	// is the image divisible into numOfThreads sub pictures (chess board)
	if ( HelperFunctions::getInstance()->isPerfectSquare(numOfThreads) == true ) {
		int devider = sqrt(numOfThreads);

		std::cout << "sub images are arranged as a chess board with " << devider << "x" << devider <<" fields." << std::endl;

		// depending on the width/height, is the image still divisible into even numOfThreads sub pictures?
		if ( width % devider == 0 ) {
			unsigned minX, minY, maxX, maxY;

			std::vector<std::thread> workers;

			Mandelbrot *partMandelbrot[numOfThreads];

			for (int i = 0; i < numOfThreads; i++) {
				// coordinates calculation
				minX = (i / devider) * (width / devider);
				maxX = ((i / devider) + 1) * (width / devider);
				minY = (i % devider) * (height / devider);
				maxY = ((i % devider) + 1) * (height / devider);

				partMandelbrot[i] = new Mandelbrot(width, height, minX, maxX, minY, maxY, iterations);

				workers.push_back(std::thread(createMandelbrotImageThread, partMandelbrot[i], std::ref(image)));

				std::cout << "Thread #" << i << " created. Calculating (" << minX << "," << minY << ") to (" << maxX << "," << maxY << ").\n";
			}

			for (auto &th : workers)
			{
				th.join();
			}

			std::cout << "Finished.\n" << std::endl;;
		} else {
			std::cout << "error: Image width/height not divisible with given amount of threads! "
				  << width << " / " << "sqrt(" << numOfThreads << ") = "
				  << width / sqrt(numOfThreads) << "\n" << std::endl;

			std::cout << "Canceled.\n";
		}
	} else {
		std::cout << "error: Number of threads doesn't match! Has to be an even number of sqrt(" << numOfThreads << ") = "
			  << sqrt(numOfThreads) <<".\n" << std::endl;

		std::cout << "Canceled.\n";
	}
}

void createMandelbrotImageCompressedThread(Mandelbrot * mandelbrot, std::string &returnBuf) {
	mandelbrot->calculateCompressedImage(returnBuf);
}

void createMandelbrotImageCompressed(std::string filename, unsigned int width, unsigned int height, int numOfThreads) {

	/*
	 * 	sub image coordinate computation (e.g. 600x600, 5 threads) -> row method
	 *
	 *	-- x
	 *	|
	 *	y
	 *
	 *	P0a(0|0)
	 *		x________________________________
	 * 		|								|
	 * 		|				0				|
	 * 		|_______________________________|
	 *										x
	 *	P1a(0|150)						P0a(600|150)
	 *		x________________________________
	 * 		|								|
	 * 		|				1				|
	 * 		|_______________________________|
	 *										x
	 *	P2a(0|300)						P1e(600|300)
	 *		x________________________________
	 * 		|								|
	 * 		|				2				|
	 * 		|_______________________________|
	 * 										x
	 * 						.			P2e(600|450)
	 * 						.
	 * 						.
	 *		_________________________________
	 * 		|								|
	 * 		|				3				|
	 * 		|_______________________________|
	 *		_________________________________
	 * 		|								|
	 * 		|				4				|
	 * 		|_______________________________|
	 *
	 */

	std::cout << "Creating compressed image...\n";

	// buffer for the results of the threads
	std::string * buf = new std::string[numOfThreads];

	// is the image divisible into numOfThreads sub pictures (rows)
	if ( width % numOfThreads == 0 ) {
		int devider = numOfThreads;
		unsigned minX, minY, maxX, maxY;

		std::cout << "sub images are arranged as " << devider << "x" << " rows." << std::endl;

		// combine maximal compressionLevel bits of the bitstream to a new number
		// e.g. 0110 0010 ... -> 10 2 ...
		int numOfCombinedBits = HelperFunctions::getInstance()->gd(width, 30);

		std::cout << "compression level (bits combined): " << numOfCombinedBits << std::endl;

		std::vector<std::thread> workers;

		Mandelbrot *partMandelbrot[numOfThreads];

		for (int i = 0; i < numOfThreads; i++) {
			minX = 0;
			maxX = width;
			minY = i * (height / numOfThreads);
			maxY = (i+1) * (height / numOfThreads);

			partMandelbrot[i] = new Mandelbrot(width, height, minX, maxX, minY, maxY);

			workers.push_back(std::thread(createMandelbrotImageCompressedThread, partMandelbrot[i], std::ref(buf[i])));

			std::cout << "Thread #" << i << " created. Calculating (" << minX << "," << minY << ") to (" << maxX << "," << maxY << ").\n";
		}

		for (auto &th : workers) {
			th.join();
		}

		std::cout << "Compressed from " << width*height << " to " << ((width * height) / numOfCombinedBits) << " characters -> done.\n" << std::endl;

		// collect results and create PPM file to store the compressed image

		Mandelbrot mandelbrot(width, height);

		mandelbrot.createPPMFile(filename);

		for(int i=0; i < numOfThreads; i++) {
			mandelbrot.writeToPPMFile(filename, buf[i]);
		}

		std::cout << "Finished.\n" << std::endl;;
	} else {
		std::cout << "error: Number of threads doesn't match! Has to be an even number of " << height << " / "
			  << numOfThreads << " = "
			  << (float)height / numOfThreads <<".\n" << std::endl;

		std::cout << "Canceled.\n";
	}
}

int main() {
	std::cout << "Mandelbrot Fractal Generator 1.0\n" << std::endl;

	/* example to create the Mandelbrot set depending on the number of iterations */
	PPMImage image_1(1024, 1024);

	for(int i = 0; i<50; i++) {
		if( i % 10 == 0 ) {
			createMandelbrotImage(image_1, 1024, 1024, 16, i);
			image_1.save("pic/mandelbrot-" + std::to_string(i/10) + ".ppm");
		}
	}

	/* example on how to encode / decode a created compressed *.ppm image */
	PPMImage image_2(600, 600);

	/* compress */

	/* using the chess board method */
	createMandelbrotImage(image_2, 600, 600, 16, 34);
	image_2.codeImg("pic/mandelbrot-coded-1.ppm");

	/* using the row method */
	createMandelbrotImageCompressed("pic/mandelbrot-coded-2.ppm", 600, 600, 4);

	/* uncompress */

	PPMImage image_3(600, 600);
	image_3.decodeImg("pic/mandelbrot-coded-1.ppm", "pic/mandelbrot-decoded-1.ppm");
	image_3.decodeImg("pic/mandelbrot-coded-2.ppm", "pic/mandelbrot-decoded-2.ppm");

    return 0;
}
