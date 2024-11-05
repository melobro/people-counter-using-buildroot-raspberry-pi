/**
 * @file main.c
 * @brief Entry point for the video capture program.
 */

#include "capture_video.h"
#include <stdio.h>
#include <string.h> // Added for strerror

/**
 * @brief Entry point of the program.
 * 
 * This function calls the capture_video() function to capture video from a
 * video device. If an error occurs during video capture, an error message is
 * printed to stderr.
 * 
 * @return 0 if successful, otherwise returns a non-zero value.
 */
int main() {
    int result = capture_video();
    if (result != 0) {
        fprintf(stderr, "Error capturing video: %s\n", strerror(result));
        return 1;
    }
    return 0;
}

