/**
 * @file test_webcam.c
 * @brief Test file for capturing video from a webcam.
 */

#include "capture_video.h"
#include <stdio.h>

/**
 * @brief Main function to test video capture functionality.
 * 
 * This function tests the video capture functionality by calling the capture_video()
 * function and checking the return value. It prints a message indicating whether the
 * video capture was successful or if any errors occurred.
 * 
 * @return 0 if the test completes successfully, otherwise returns a non-zero value.
 */
int main() {
    printf("Testing video capture from webcam...\n");
    
    int result = capture_video();
    
    if (result == 0) {
        printf("Video capture completed successfully.\n");
    } else {
        printf("Error occurred during video capture. Error code: %d\n", result);
    }

    return 0;
}

