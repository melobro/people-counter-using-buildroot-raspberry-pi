#include "gpio.h" 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Define GPIO pins numbers
#define TRIG_PIN_NUM 17  // GPIO 17
#define ECHO_PIN_NUM 18  // GPIO 18

// Function to measure distance
float measure_distance() {
    set_value(1, TRIG_PIN_NUM);
    usleep(10);  // Send trigger pulse for 10 microseconds
    set_value(0, TRIG_PIN_NUM);

    // Wait for echo to start
    while (!get_value(ECHO_PIN_NUM));

    // Measure the duration of the pulse
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    while (get_value(ECHO_PIN_NUM));
    gettimeofday(&endTime, NULL);

    // Calculate travel time in microseconds
    long travelTime = (endTime.tv_sec - startTime.tv_sec) * 1000000L + (endTime.tv_usec - startTime.tv_usec);

    // Calculate distance (in centimeters)
    float distance = travelTime / 58.0;  // Speed of sound is 340 m/s or 29 microseconds per centimeter
    return distance;
}

int main(void) {
    // Initialize GPIO pins
    export_pin(TRIG_PIN_NUM);
    export_pin(ECHO_PIN_NUM);

    // Set GPIO pins direction
    set_direction("out", TRIG_PIN_NUM);
    set_direction("in", ECHO_PIN_NUM);

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

    // Cleanup
    unexport_pin(TRIG_PIN_NUM);
    unexport_pin(ECHO_PIN_NUM);

    return 0;
}
