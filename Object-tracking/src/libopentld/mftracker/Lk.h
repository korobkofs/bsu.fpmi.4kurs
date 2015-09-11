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

/**
 * @file lk.h
 *
 *  Created on: 02.03.2011
 *      @author bernd
 */

#ifndef LK_H_
#define LK_H_

#include <opencv/cv.h>

/**
 * @brief Needed before start of trackLK and at the end of the program for cleanup.
 * Handles PYR(Pyramid cache) variable.
 */
void initImgs();

/**
 * @brief Tracks Points from 1.Image to 2.Image.
 * Need initImgs before start and at the end of the program for cleanup.
 *
 * @param imgI      previous Image source. (isn't changed)
 * @param imgJ      actual Image target. (isn't changed)
 * @param ptsI      points to track from first Image.
 *                  Format [0] = x1, [1] = y1, [2] = x2 ...
 * @param nPtsI     number of Points to track from first Image
 * @param ptsJ      container for calculated points of second Image.
 *                  Must have the length of nPtsI.
 * @param nPtsJ     number of Points
 * @param level     Pyramidlevel, default 5
 * @param fbOut        forward-backward confidence value.
 *                  (corresponds to euclidean distance between).
 *                  Must have the length of nPtsI: nPtsI * sizeof(float).
 * @param nccOut       normCrossCorrelation values. needs as inputlength nPtsI * sizeof(float)
 * @param statusOut    Indicates positive tracks. 1 = PosTrack 0 = NegTrack
 *                  needs as inputlength nPtsI * sizeof(char)
 * @return 1, if success; 0, if nPtsJ != nPtsI
 *
 */
int trackLK(IplImage *imgI, IplImage *imgJ, float ptsI[], int nPtsI,
            float ptsJ[], int nPtsJ, int level, float *fbOut, float *nccOut,
            char *statusOut);

#endif /* LK_H_ */
