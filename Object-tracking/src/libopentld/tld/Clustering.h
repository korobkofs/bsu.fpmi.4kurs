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
 * Clustering.h
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#ifndef CLUSTERING_H_
#define CLUSTERING_H_

#include <vector>

#include <opencv/cv.h>

#include "DetectionResult.h"

namespace tld
{

/**
 * @brief The clustering of detector cascade results.
 *
 */
class Clustering
{
    /**
     * @brief Calculates the mean of indexed rectangles (windows).
     *
     * @param indices
     *          the indices of the interesting windows
     */
    void calcMeanRect(std::vector<int> * indices, int *clusterIndices);

    /**
     * @brief Calculates distances (0 to 1) between windows.
     *
     * @param distances
     *          the destination array of distances (however must be of size n*(n+1)/2, n=detectionResult->confidentIndices)
     */
    void calcDistances(float *distances);

    /**
     * @brief Determines the number of clusters and the indices of clusters which belong the сonfident windows to.
     *
     * @param distances
     *          the array of distances
     * @param clusterIndices
     *          the indices of clusters (the size must be equal detectionResult->confidentIndices->size())
     */
    void cluster(float *distances, int *clusterIndices);
public:
    int *windows; //!< the array of windows
    int numWindows; //!< the number of windows

    DetectionResult *detectionResult; //!< the result of detection

    //Configurable members
    float cutoff; //!< the threshold for detect of belong the windows to the same cluster


    /**
     * @brief This constructor initializes the class variables.
     *
     */
    Clustering();

    /**
     * @brief Empty destructor.
     *
     */
    virtual ~Clustering();

    /**
     * @brief Releases memory on the array of windows.
     *
     */
    void release();

    /**
     * @brief Calculates the bounding box from ConfidentIndices (the indices of сonfident windows), if the number of clusters equal 1.
     *
     */
    void clusterConfidentIndices();
};

} /* namespace tld */
#endif /* CLUSTERING_H_ */
