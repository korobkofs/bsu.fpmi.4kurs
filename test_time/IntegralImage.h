/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/*
 * IntegralImage.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef INTEGRALIMAGE_H_
#define INTEGRALIMAGE_H_

#include <opencv/cv.h>

namespace tld
{


template <class T>
/**
 * @brief The class for calculating the integral image.
 *
 */
class IntegralImage
{
public:
    T *data;  //!< Array containg the entries for the integral image in row-first manner. Of size width*height
    int width; //!< the width of integral image.
    int height; //!< the height of integral image.

    /**
     * @brief  This constructor allocates the integral image.
     *
     * @param size
     *          the size of the integral image.
     */
    IntegralImage(cv::Size size)
    {
        data = new T[size.width * size.height];
    }

    /**
     * @brief This destructor releases of data memory.
     *
     */
    virtual ~IntegralImage()
    {
        delete[] data;
    }

    /**
     * @brief Calculates the integral image.
     *
     * @param img
     *          the transforming image.
     * @param squared
     *          if true, the integral image is calculated for square pixel values;  if false, the integral image is calculated standard
     */
    void calcIntImg(const cv::Mat &img, bool squared = false)
    {
        const unsigned char *input = (const unsigned char *)(img.data);
        T *output = data;

        for(int i = 0; i < img.cols; i++)
        {
            for(int j = 0; j < img.rows; j++)
            {
                T A = (i > 0) ? output[img.cols * j + i - 1] : 0;
                T B = (j > 0) ? output[img.cols * (j - 1) + i] : 0;
                T C = (j > 0 && i > 0) ? output[img.cols * (j - 1) + i - 1] : 0;
                T value = input[img.step * j + i];

                if(squared)
                {
                    value = value * value;
                }

                output[img.cols * j + i] = A + B - C + value;
            }
        }

    }
};


} /* namespace tld */
#endif /* INTEGRALIMAGE_H_ */
