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

#ifndef GUI_H_
#define GUI_H_

#include <string>

#include <opencv/highgui.h>

namespace tld
{

/**
 * @brief Graphical user interface.
 *
 */
class Gui
{
public:

    /**
     * @brief This constructor gives the name the output window by default.
     *
     */
    Gui();

    /**
     * @brief Empty destructor.
     */
    ~Gui();

    /**
     * @brief Creates the output window.
     *
     */
    void init();

    /**
     * @brief Shows the image.
     *
     * @param image
     *          the image to be shown
     */
    void showImage(IplImage *image);

    //void writeImage(IplImage *image);

    /**
     * @brief Expects key press for some time.
     *
     * @return the character of the pressed key
     */
    char getKey();

    /**
     * @brief Gets the name of the output window
     *
     * @return the name of the output window
     */
    std::string windowName();

    //CvVideoWriter * writer;

private:
    std::string m_window_name; /**< The name of the output window */
};

/**
 * @brief Gets a bounding box from the user.
 * @param img image to display
 * @param rect CvRect containing the coordinates of the bounding box
 * @param gui initialized gui
 * @return PROGRAM_EXIT if 'q' or 'Q' pressed, SUCCESS if everything went right
 */
int getBBFromUser(IplImage *img, CvRect &rect, Gui *gui);

}

#endif /* GUI_H_ */
