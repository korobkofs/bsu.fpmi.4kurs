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
 * MedianFlowTracker.h
 *
 *  Created on: Nov 17, 2011
 *      Author: Georg Nebehay
 */

#ifndef MEDIANFLOWTRACKER_H_
#define MEDIANFLOWTRACKER_H_

#include <opencv/cv.h>

namespace tld
{

/**
 * @brief The median flow tracker (based on the forward-backward error).
 *
 */
class MedianFlowTracker
{
public:
    cv::Rect *trackerBB; //!< the found bounding box

    /**
     * @brief This Constructor initializes the bounding box by NULL.
     *
     */
    MedianFlowTracker();

    /**
     * @brief This Destructor cleans the variable class.
     *
     */
    virtual ~MedianFlowTracker();

    /**
     * @brief Cleans data about the bounding box.
     *
     */
    void cleanPreviousData();

    /**
     * @brief Calculate the bounding box of an Object in a following Image (with analyse of success and correctness of bounding boxes)
     *
     * @param prevImg
     *          the previous image
     * @param currImg
     *          the current image
     * @param prevBB
     *          the previous bounding box
     */
    void track(const cv::Mat &prevImg, const cv::Mat &currImg, cv::Rect *prevBB);
};

} /* namespace tld */
#endif /* MEDIANFLOWTRACKER_H_ */
