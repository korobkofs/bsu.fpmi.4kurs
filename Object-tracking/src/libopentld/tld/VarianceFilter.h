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
 * VarianceFilter.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef VARIANCEFILTER_H_
#define VARIANCEFILTER_H_

#include <opencv/cv.h>

#include "DetectionResult.h"

namespace tld
{

/**
 * @brief The variance filter.
 *
 */
class VarianceFilter
{
    IplImage * integralImg; //!< the standart integral image
    IplImage * integralImg_squared; //!< the integral image for square pixel values

public:
    bool enabled; //!< enables variance filter
    int *windowOffsets; //!<the window offsets

    DetectionResult *detectionResult; //!< the result of detection

    float minVar; //!< minimum value for filtering the variance in window offsets.

    /**
     * @brief This constructor initializes the class variables.
     *
     */
    VarianceFilter();

    /**
     * @brief This destructor releases the memory.
     *
     */
    virtual ~VarianceFilter();


    /**
     * @brief Releases the memory.
     *
     */
    void release();

    /**
     * @brief Reallocates and calculates the standart integral image and the integral image for square pixel values.
     *
     * @param img
     *          the source image
     */
    void nextIteration(const cv::Mat &img1);

    /**
     * @brief Calculates the variance in window offsets and checks if this variance is more or equal minimum value (minVar).
     *
     * @param idx
     *          the index of window offset.
     * @return true, if the variance is more or equal minimum value; false, otherwise
     */
    bool filter(int idx);

    /**
     * @brief Calculates the variance in region.
     *
     * @param off
     *          the region of intrest (array - x1, y1, x2, y2, none, area)
     * @return the variance
     */
    float calcVariance(int *off);
};

} /* namespace tld */
#endif /* VARIANCEFILTER_H_ */
