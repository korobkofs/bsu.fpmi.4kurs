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
 * TLD.h
 *
 *  Created on: Nov 17, 2011
 *      Author: Georg Nebehay
 */

#ifndef TLD_H_
#define TLD_H_

#include <opencv/cv.h>

#include "MedianFlowTracker.h"
#include "DetectorCascade.h"

namespace tld
{

/**
 * @brief The tracking lerning detector.
 *
 */
class TLD
{   
    /**
     * @brief Stores the current data as previous.
     *
     */
    void storeCurrentData();

    /**
     * @brief Fuses (integrates) the results of tracker (Lucas-Kanade) and detector (detects the object with using patterns).
     *
     */
    void fuseHypotheses();

    /**
     * @brief Learns the tld.
     *
     */
    void learn();

    /**
     * @brief Initializes the learning.
     *
     */
    void initialLearning();
public:
    bool trackerEnabled; //!< enables tracker (Lucas-Kanade)
    bool detectorEnabled; //!< enables detector (detects the object with using patterns)
    bool learningEnabled; //!< enables learning
    bool alternating; //!< if set to true, detector is disabled while tracker is running.

    MedianFlowTracker *medianFlowTracker; //!< the tracker (Lucas-Kanade)
    DetectorCascade *detectorCascade; //!< the detector (detects the object with using patterns)
    NNClassifier *nnClassifier; //!< the nearest neighbor сlassifier
    bool valid; //!< the current result is valid
    bool wasValid; //!< the previous result is valid
    cv::Mat prevImg; //!< the previous image
    cv::Mat currImg; //!< the current image
    cv::Rect *prevBB; //!< the previous bounding box
    cv::Rect *currBB; //!< the current bounding box
    float currConf; //!< the current confident
    bool learning; //!< the learning happened or not

    /**
     * @brief This constructor initializes the class variables.
     *
     */
    TLD();

    /**
     * @brief This destructor releases memory.
     *
     */
    virtual ~TLD();

    /**
     * @brief Cleans memory for reinitialization.
     *
     */
    void release();

    /**
     * @brief Selects the object of intrest.
     *
     * @param img
     *          the image
     * @param bb
     *          the bounding box
     */
    void selectObject(const cv::Mat &img, cv::Rect *bb);

    /**
     * @brief Processes the image.
     *
     * @param img
     *          the image
     */
    void processImage(const cv::Mat &img);

    /**
     * @brief Write the tld model to file.
     *
     * @param path
     *          the path to new file
     */
    void writeToFile(const char *path);

    /**
     * @brief Reads the tld model from file.
     *
     * @param path
     *          the path to source file
     */
    void readFromFile(const char *path);
};

} /* namespace tld */
#endif /* TLD_H_ */
