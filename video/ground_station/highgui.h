#ifndef __TK_HIGHGUI_H__
#define __TK_HIGHGUI_H__

#include <string>
#include <opencv2/core/core.hpp>

#ifndef __NVCC__

#include <armadillo>

/** Load an image in arma::cube format
 *  @param image_name the name of the image
 *  @return the cube which holds the image data
 */
arma::cube load_image(const std::string &image_name);

/** Save an image to an image name
 *  @param image_name the image name to be saved under
 *  @param image the image to be saved (grayscale)
 */
void save_image(const std::string &image_name, const arma::mat &image);

/** Save an image to an image name
 *  @param image_name the image name to be saved under
 *  @param image the image to be saved (rgb)
 */
void save_image(const std::string &image_name, const arma::cube &image);

/** Display an image in an OpenCV window
 *  @param window_name the name of the window to display the image
 *  @param image the image to be displayed (grayscale)
 *  @param mouseevent (optional) enable mouse events
 */
void disp_image(const std::string &window_name, const arma::mat &image, bool mouseevent = false);

/** Display an image in an OpenCV window
 *  @param window_name the name of the window to display the image
 *  @param image the image to be displayed (rgb)
 *  @param mouseevent (optional) enable mouse events
 */
void disp_image(const std::string &window_name, const arma::cube &image, bool mouseevent = false);

/** Grab the position of a left mouse click
 *  @param window_name the name of the window to get a click from
 *  @return the vector of positions where the mouse was clicked
 */
std::vector<int> disp_get_lclick_pos(const std::string &window_name);

/** See whether or not the mouse was clicked
 *  @param window_name the name of the window to get a click from
 *  @return true if clicked, false otherwise
 */
bool disp_get_lclicked(const std::string &window_name);

/** Grab the position of the right mouse click
 *  @param window_name the name of the window to get a click from
 *  @return the vector of positions where the mouse was clicked
 */
std::vector<int> disp_get_rclick_pos(const std::string &window_name);

/** See whether or not the mouse was clicked
 *  @param window_name the name of the window to get a click from
 *  @return true if clicked, false otherwise
 */
bool disp_get_rclicked(const std::string &window_name);

/** Get the current mouse position in the window
 *  @param window_name the name of the window to grab the mouse position from
 *  @return the vector of positions where the mouse is currently at
 */
std::vector<int> disp_get_mouse_pos(const std::string &window_name);

/** Wait for a key to be pressed before closing the window
 */
void disp_wait(void);

/** Wait for a key to be pressed for 30 fps
 *  @return status of a key pressed
 */
int disp_keyPressed(void);

/** Close a window of an image
 *  @param window_name the name of the window to close
 */
void disp_close(const std::string &window_name);

/** Convert the OpenCV image to an arma::cube
 *  @param cv_image the OpenCV image
 *  @return the arma::cube image
 */
arma::cube cvt_opencv2arma(const cv::Mat &cv_image);

/** Convert the arma::cube image to an OpenCV image
 *  @param image the arma::cube image
 *  @return the OpenCV image
 */
cv::Mat cvt_arma2opencv(const arma::cube &image);

/** Convert rgb to grayscale
 *  @param image the rgb image
 *  @return the grayscale image
 */
arma::mat cvt_rgb2gray(const arma::cube &image);

/** Convert grayscale to rgb
 *  @param the grayscale image
 *  @return the rgb image
 */
arma::cube cvt_gray2rgb(const arma::mat &image);

#else

#include "gcube.h"

/** Display a gcube on a window
 *  @param window_name the name of the window
 *  @param image the gcube to display
 */
void disp_gcube(const std::string &window_name, gcube &image);

/** Wait until presses a button on the window
 */
void disp_wait(void);

/** Wait 30 milliseconds or until user presses a button
 *  @return the value of the button pressed if a button was pressed,
 *  otherwise -1
 */
int disp_keyPressed(void);

/** Convert a gcube from RGB space to GRAY space
 *  @param image the RGB image
 *  @return an image in GRAY space
 */
gcube gpu_rgb2gray(const gcube &image);

/** Convert a gcube from GRAY space to RGB space
 *  @param image the GRAY image
 *  @return an image in RGB space
 */
gcube gpu_gray2rgb(const gcube &image);

#endif

#endif
