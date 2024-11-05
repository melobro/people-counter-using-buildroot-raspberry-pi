#include "capture_video.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/stat.h>

#define VIDEO_DEVICE "/dev/video0"
#define OUTPUT_FILE "pi_camera"
#define FILE_EXTENSION ".mp4"
#define BUFFER_SIZE 4194304 // 4 MB
#define FRAME_RATE 30       // Frames per second
#define DURATION 3          // Duration of the video in seconds

int capture_video() {
    int video_fd, output_fd;
    struct v4l2_format fmt;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int i;

    // Open video device
    video_fd = open(VIDEO_DEVICE, O_RDWR);

    // Set video format
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(video_fd, VIDIOC_G_FMT, &fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(video_fd, VIDIOC_S_FMT, &fmt);

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

    // Capture video for 3 seconds
    for (i = 0; i < (FRAME_RATE * DURATION); i++) {
        bytes_read = read(video_fd, buffer, sizeof(buffer));
        write(output_fd, buffer, bytes_read);
        usleep(1000000 / FRAME_RATE); // Wait 1/FRAME_RATE second
    }

    // Close files and device
    close(video_fd);
    close(output_fd);

    // Inform the user that recording is finished
    printf("Video captured as %s.\n", output_file_name);

    return 0;
}

