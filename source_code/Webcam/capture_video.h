/**
 * @file capture_video.h
 * @brief Header file for capturing video.
 */

#ifndef CAPTURE_VIDEO_H
#define CAPTURE_VIDEO_H

/**
 * @brief Captures video from a video device and saves it to a file.
 * 
 * This function captures video from a specified video device (typically a webcam or similar device) 
 * using the Video4Linux API (V4L2) in a Linux environment. It records video for a specified duration 
 * and saves it to a file with a dynamically generated name.
 * 
 * @return 0 on success, otherwise returns an error code.
 */
int capture_video();

#endif /* CAPTURE_VIDEO_H */

