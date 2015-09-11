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
 * main.h
 *
 *  Created on: Nov 18, 2011
 *      Author: Georg Nebehay
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "TLD.h"
#include "ImAcq.h"
#include "Gui.h"
#include <opencv2/opencv.hpp>

enum Retval
{
    PROGRAM_EXIT = 0,
    SUCCESS = 1
};

/**
 * @brief The main class.
 *
 */
class Main
{
public:
    tld::TLD *tld; //!<  the detector of tracking object
    ImAcq *imAcq; //!<  the capture frames
    tld::Gui *gui; //!<  the graphical user interface
    bool showOutput; //!<  if true, it shown the output window
    bool showTrajectory; //!<  if true, it shown the trajectory of tracking object on the output window
    int trajectoryLength; //!<  the number of the last frames which are considered by the trajectory
    const char *printResults; //!<  the path to the file where the results (values) should be printed; NULL -> results will not be printed
    const char *saveDir; //!< the directory where the results (images) should be printed
    double threshold; //!<  the threshold for determining the confident (0 or 1) of object detection
    bool showForeground; //!<  if true, it shown foreground
    bool showNotConfident; //!< if true, it shown bounding box also if confidence is low
    bool selectManually; //!<  if true, user can select initial bounding box
    int *initialBB; //!< the initial bounding box
    bool reinit; //!<  for reinititalization; it isn't used
    bool exportModelAfterRun; //!<  if true, model is exported after run.
    bool loadModel; //!< if true, model specified by "modelPath" is loaded at startup
    const char *modelPath; //!<  the path of loaded model
    const char *modelExportFile; //!< the path where model is saved on export
    int seed; //!< the seed for pseudo-random number generator

    /**
     * @brief Initializes class variables.
     *
     */
    Main()
    {
        tld = new tld::TLD();
        showOutput = 1;
        printResults = NULL;
        saveDir = ".";
        threshold = 0.5;
        showForeground = 0;

		showTrajectory = false;
		trajectoryLength = 0;

        selectManually = 0;

        initialBB = NULL;
        showNotConfident = true;

        reinit = 0;

        loadModel = false;

        exportModelAfterRun = false;
        modelExportFile = "model";
        seed = 0;

        gui = NULL;
        modelPath = NULL;
        imAcq = NULL;
    }

    /**
     * @brief Releases memory occupied with objects of tld and imAcq classes.
     *
     */
    ~Main()
    {
        delete tld;
        imAcqFree(imAcq);
    }

    static const double MOTION_THRESHOLD = 10; //!< The threshold for detection of moving points.
    static const double MIN_BOX_SIDE = 15; //!< The minimum size of the side of the bounding box.

    /**
     * @brief Begins the work.
     *
     */
    void doWork();

    /**
     * @brief Determines whether there is motion in the box.
     *
     * @param frame1
     *          the one of two images
     * @param frame2
     *          the one of two images
     * @return true or false
     */
    bool isMotionInBox(cv::Mat frame1, cv::Mat frame2);

    /**
     * @brief Determines a maximun of the moving regions.
     *
     * @param last_gray
     *          the last grabbed frame (image) in grayscale
     * @return the rectangle
     */
    cv::Rect maxMovingRegion(cv::Mat& last_gray);

    /**
     * @brief Unites the overlapping rectangles.
     *
     * @param src_rect
     *          the vector of the sources rectangles
     * @return the vector of the united rectangles
     */
    std::vector<cv::Rect> unionRectangls(std::vector<cv::Rect> src_rect);
};

#endif /* MAIN_H_ */
