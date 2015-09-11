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
 * DetectionResult.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef DETECTIONRESULT_H_
#define DETECTIONRESULT_H_

#include <vector>

#include <opencv/cv.h>

namespace tld
{

/**
 * @brief The detection result.
 *
 */
class DetectionResult
{
public:
    bool containsValidData; //!< true, if class contain valid data; false, otherwise
    std::vector<cv::Rect>* fgList; //!< the list of regions of foreground
    float *posteriors;  //!< the array contains the posteriors for each sliding window. Is of size numWindows. Allocated by initPosteriors.
    std::vector<int>* confidentIndices; //!< the vector of indices of confident windows
    int *featureVectors; //!< the feature vectors (in Essemble Classifier)
    float *variances; //!< the array of variance in window offsets
    int numClusters; //!< the number of clusters
    cv::Rect *detectorBB; //!< the result bounding box, contains a valid result only if numClusters = 1

    /**
     * @brief This constructor initializes the class variables.
     *
     */
    DetectionResult();

    /**
     * @brief This destructor releases the memory.
     *
     */
    virtual ~DetectionResult();

    /**
     * @brief Initializes the class variables.
     *
     * @param numWindows
     *          the number of windows
     * @param numTrees
     *          the number of trees
     */
    void init(int numWindows, int numTrees);

    /**
     * @brief Resets the class variables.
     *
     */
    void reset();

    /**
     * @brief Release the memory occupied with the class variables.
     *
     */
    void release();

};

} /* namespace tld */
#endif /* DETECTIONRESULT_H_ */
