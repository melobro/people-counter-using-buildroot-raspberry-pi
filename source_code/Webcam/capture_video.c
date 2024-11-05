/**
 * @file capture_video.c
 * @brief Captures video from a video device and saves it to a file.
 */

#include "capture_video.h"
#include <stdio.h>
#include <string.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <errno.h> // Added for error handling
#include <time.h> 

#define VIDEO_DEVICE "/dev/video0"
#define OUTPUT_FILE "pi_camera"
#define FILE_EXTENSION ".mp4"
#define BUFFER_SIZE 16777216 // 16 MB
#define FRAME_RATE 30       // Frames per second
#define DURATION 3          // Duration of the video in seconds

/**
 * @brief Captures video from a video device and saves it to a file.
 * 
 * This function captures video from a specified video device (typically a webcam or similar device) 
 * using the Video4Linux API (V4L2) in a Linux environment. It records video for a specified duration 
 * and saves it to a file with a dynamically generated name.
 * 
 * @return 0 on success, otherwise returns an error code.
 */
int capture_video() {
    int video_fd, output_fd;
    struct v4l2_format fmt;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int i;

    // Open video device
    video_fd = open(VIDEO_DEVICE, O_RDWR);
    if (video_fd == -1) {
        perror("Error opening video device");
        return errno;
    }

    // Set video format
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(video_fd, VIDIOC_G_FMT, &fmt) == -1) {
        perror("Error getting video format");
        close(video_fd);
        return errno;
    }
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(video_fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("Error setting video format");
        close(video_fd);
        return errno;
    }

    // Inform the user that recording is starting
    printf("Starting to record:\n ");

    // Check if the output file already exists
    struct stat st;
    char output_file_name[50];
    int file_suffix = 0;
    snprintf(output_file_name, sizeof(output_file_name), "%s%s", OUTPUT_FILE, FILE_EXTENSION);
    while (stat(output_file_name, &st) == 0) {
        file_suffix++;
        snprintf(output_file_name, sizeof(output_file_name), "%s%d%s", OUTPUT_FILE, file_suffix, FILE_EXTENSION);
    }

    // Open output file
    output_fd = open(output_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("Error opening output file");
        close(video_fd);
        return errno;
    }

    // Capture video for 3 seconds
    for (i = 0; i < (FRAME_RATE * DURATION); i++) {
        bytes_read = read(video_fd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("Error reading video data");
            close(video_fd);
            close(output_fd);
            return errno;
        }
        if (write(output_fd, buffer, bytes_read) == -1) {
            perror("Error writing video data");
            close(video_fd);
            close(output_fd);
            return errno;
        }
        usleep(1000000 / FRAME_RATE); // Wait 1/FRAME_RATE second
    }

    // Close files and device
    close(video_fd);
    close(output_fd);

    // Inform the user that recording is finished
    printf("Video captured as %s.\n", output_file_name);

    return 0;
}

/**
 * @brief Entry point of the program.
 * 
 * Calls the capture_video function to capture video and handles any errors that occur.
 * 
 * @return 0 on success, 1 on error.
 */
int capture_main() {
    int result = capture_video();
    if (result != 0) {
        fprintf(stderr, "Error capturing video: %s\n", strerror(result));
        return 1;
    }
    return 0;
}

