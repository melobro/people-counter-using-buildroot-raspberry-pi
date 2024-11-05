#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Define GPIO pins
#define TRIG_PIN "/sys/class/gpio/gpio17/value"  // GPIO 17
#define ECHO_PIN "/sys/class/gpio/gpio18/value"  // GPIO 18

// Function to write value to GPIO pin
void write_pin(const char *pin, int value) {
    int fd;
    if ((fd = open(pin, O_WRONLY)) < 0) {
        perror("Failed to open pin for writing");
        exit(EXIT_FAILURE);
    }
    if (write(fd, value ? "1" : "0", 1) < 0) {
        perror("Failed to write pin value");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

// Function to read value from GPIO pin
int read_pin(const char *pin) {
    int fd;
    char buf[2];
    if ((fd = open(pin, O_RDONLY)) < 0) {
        perror("Failed to open pin for reading");
        exit(EXIT_FAILURE);
    }
    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("Failed to read pin value");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return atoi(buf);
}

// Function to measure distance
float measure_distance() {
    // Send a pulse to the sensor
    write_pin(TRIG_PIN, 1);
    usleep(10);
    write_pin(TRIG_PIN, 0);

    // Wait for echo to start
    while (!read_pin(ECHO_PIN));

    // Measure the duration of the pulse
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    while (read_pin(ECHO_PIN));
    gettimeofday(&endTime, NULL);

    // Calculate travel time
    long travelTime = (endTime.tv_sec - startTime.tv_sec) * 1000000L +
                      (endTime.tv_usec - startTime.tv_usec);

    // Calculate distance (in centimeters)
    float distance = travelTime / 58.0;  // Speed of sound
    return distance;
}

int main(void) {
    // Export GPIO pins
    system("echo 17 > /sys/class/gpio/export");
    system("echo 18 > /sys/class/gpio/export");

    // Set GPIO pins direction
    system("echo out > /sys/class/gpio/gpio17/direction");
    system("echo in > /sys/class/gpio/gpio18/direction");

    // Main loop
    while (1) {
        float dist = measure_distance();
        
        // Check if the distance is between 30 and 40 cm
        if (dist >= 30 && dist <= 40) {
            printf("Optimal distance - between 30 to 40 cm - distance: %.2f cm\n", dist);
        } 
        else {
            printf("Object not at optimal distance: %.2f cm\n", dist);
        }

        usleep(1000000);  // Delay 1 second
    }

    // Unexport GPIO pins
    system("echo 17 > /sys/class/gpio/unexport");
    system("echo 18 > /sys/class/gpio/unexport");

    return 0;
}

