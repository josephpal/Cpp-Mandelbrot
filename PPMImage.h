/*
 * PPMImage.h
 *
 *  Created on: Dec 2, 2019
 *      Author: joseph
 *
 *  src:
 *
 *  [1]	https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 *  [2] https://www.geeksforgeeks.org/converting-strings-numbers-cc/
 *  [3] https://www.includehelp.com/stl/convert-binary-string-to-integer-using-stoi-function-in-cpp-stl.aspx
 *  [4] https://en.wikipedia.org/wiki/Netpbm_format
 */

#ifndef PPMIMAGE_H_
#define PPMIMAGE_H_

#include <iostream>
#include <fstream>

#include "Matrix.h"
#include "HelperFunctions.h"

class PPMImage : public Matrix< RGB<unsigned int> > {
  public:
    PPMImage(const size_t height, const size_t width);

    void save(const std::string &filename);

    void codeImg(const std::string &filename);

    void decodeImg(const std::string &inputFile, const std::string &filename);

  private:
    int compressionLevel = 30;
};

#endif /* PPMIMAGE_H_ */

