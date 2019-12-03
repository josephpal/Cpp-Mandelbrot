/*
 * Mandelbrot.h
 *
 *  Created on: Dec 2, 2019
 *      Author: joseph
 *
 *  src:
 *
 *  [1]	http://warp.povusers.org/Mandelbrot/
 *  [2]	https://medium.com/farouk-ounanes-home-on-the-internet/mandelbrot-set-in-c-from-scratch-c7ad6a1bf2d9
 *  [3] https://stackoverflow.com/questions/53381279/mandelbrot-image-generator-in-c-using-multi-threading-overwrites-half-the
 */

#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <iostream>
#include "HelperFunctions.h"
#include "PPMImage.h"

class Mandelbrot {
public:
	/** default constructor; image width and height will be set 0 as well as minX, maxX, minY, maxY.
	 *  note that numOfCombinedBits will be null, too! This is only a template for an empty object.
	 *
	 *  @param	---
	 *  @return ---
	*/
	Mandelbrot();

	/** default constructor; image width and height will be set @param and minX, maxX, minY, maxY will be set to 0.
	 *  The constructor will compute the maximum number of combined bits with the help of gd(width, 30). In this case,
	 *  we define the maximum available number of combined bits to 30; nevertheless this number can be smaller because
	 *  width / numOfCombinedBits has to be even! This is catched by the gd() function as well.
	 *
	 *  @param	specify the width of the image
	 *  @param	specify the height of the image
	 *  @return ---
	*/
	Mandelbrot(unsigned width, unsigned height);

	/** Overloaded constructor; image width and height will be set on @param; the image has to be a square (width = height!).
	 * 	In case if width != height, height will be set to width
	 *
	 *  @param	specify the width of the image
	 *  @param	specify the height of the image
	 *  @param	specify the borders of the part image (minX, maxX, minY and maxY)
	 *  @return ---
	*/
	Mandelbrot(unsigned width, unsigned height, unsigned minX, unsigned maxX, unsigned minY, unsigned maxY);
	virtual ~Mandelbrot();

	/** function to set the number of bits, which will be combined to an integer value for writing into the *.ppm file.
	 *  Depending on the width/height, the passed parameter won't fit, in this case the value will be automatic decrease until it reaches
	 *  a even solution of width / @param -> this is achieved by using the function gd(int a, int b) in "HelperFunctions.h".
	 *
	 *  If not defined, the constructor will compute the maximum number of combined bits with the help of gd(width, 30). In this case,
	 *  we define the maximum available number of combined bits to 30; nevertheless this number can be smaller because width / numOfCombinedBits
	 *  has to be even!
	 *
	 *  @param	specify the number of combined bits in the bitstream of the *.ppm file
	 *  @return ---
	*/
	void setCompressionLevel(int numOfCombinedBits);

	/** function to calculate a part image of the mandelbrot fractal between minX, maxX, minY and maxY; the result will be returned as
	 * 	an compressed string, which contains a stream of int values regarding to maximum available value (2^numOfCombinedBits).
	 *
	 *  @param	specify the number of combined bits in the bitstream of the *.ppm file
	 *  @return &returnBuf will contain the int data stream of the part row picture
	*/
	void calculateCompressedImage(std::string &returnBuf);


	/** function to calculate a part image of the mandelbrot fractal between minX, maxX, minY and maxY; the result will be stored in
	 * 	matrix, which is a property of the reference of the PPMImage @param; the the matrix will contain rgb values.
	 *
	 *  @param	pass the reference to the PPMImage to store the result
	 *  @return &image will contain the points of the mandelbrot fractal in form of a matrix
	*/
	void calculateImage(PPMImage &image);

	/** ...
	 *
	 *  @param
	 *  @return
	*/
	void createPPMFile(std::string &filename);

	/** ...
	 *
	 *  @param
	 *  @return
	*/
	void writeToPPMFile(std::string &filename, std::string &content);

private:
	unsigned width;
	unsigned height;

	unsigned minX, maxX, minY, maxY;

	int numOfCombinedBits;
};

#endif /* MANDELBROT_H_ */
