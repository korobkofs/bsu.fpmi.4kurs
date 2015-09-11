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
 * ForegroundDetector.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef FOREGROUNDDETECTOR_H_
#define FOREGROUNDDETECTOR_H_

#include <vector>

#include <opencv/cv.hpp>

#include "DetectionResult.h"

namespace tld
{

/**
 * @brief The foreground detector.
 *
 */
class ForegroundDetector
{
public:
    int fgThreshold; //!< the threshold for diffference two images
    int minBlobSize; //!< the minimum size of blob
    cv::Mat bgImg; //!< the background image
    DetectionResult *detectionResult; //!< the result of detection

    /**
     * @brief This constructor initialize some class variables by default.
     *
     */
    ForegroundDetector();

    /**
     * @brief Empty destructor.
     *
     */
    virtual ~ForegroundDetector();

    /**
     * @brief Empty method.
     *
     */
    void release();

    /**
     * @brief Finds regions of foreground in the image.
     *
     * @param img
     *          the image
     */
    void nextIteration(const cv::Mat &img);

    /**
     * @brief Check presence the background image.
     *
     * @return true, if the background image present; false, otherwise
     */
    bool isActive();
};

} /* namespace tld */
#endif /* FOREGROUNDDETECTOR_H_ */
