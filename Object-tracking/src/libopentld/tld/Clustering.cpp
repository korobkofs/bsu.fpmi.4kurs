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
 * Clustering.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#include "Clustering.h"

#include "TLDUtil.h"
#include "DetectorCascade.h"

using namespace cv;

namespace tld
{

Clustering::Clustering()
{
    cutoff = .5;
    windows = NULL;
    numWindows = 0;
}

Clustering::~Clustering()
{
}

void Clustering::release()
{
    windows = NULL;
    numWindows = 0;
}

void Clustering::clusterConfidentIndices()
{
    int numConfidentIndices = detectionResult->confidentIndices->size();

    std::vector<Rect> rectangles;
    Rect rect;
    float x, y, w, h;

    for(int i = 0; i < numConfidentIndices; i++)
    {
        int *bb = &windows[TLD_WINDOW_SIZE * detectionResult->confidentIndices->at(i)];
        x = bb[0];
        y = bb[1];
        w = bb[2];
        h = bb[3];

        rect.x = floor(x + 0.5);
        rect.y = floor(y + 0.5);
        rect.width = floor(w + 0.5);
        rect.height = floor(h + 0.5);

        rectangles.push_back(rect);
        rectangles.push_back(rect);
    }

    cv::groupRectangles(rectangles, 1, 0.2);

    if (rectangles.size() > 0)
    {
        detectionResult->detectorBBs = new vector<Rect>();
        for (int i = 0; i < rectangles.size(); ++i)
        {
            detectionResult->detectorBBs->push_back(rectangles[i]);
        }
    }
}
} /* namespace tld */
