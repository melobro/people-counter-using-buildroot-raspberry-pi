#include "gpio.h"  // Ensure this includes the proper definitions and functions from gpio.c
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// GPIO pins ledpird2
#define PIR1 16
#define PIR2 6

// GPIO pins ultra4H
#define TRIG_PIN_NUM 17  // GPIO 17
#define ECHO_PIN_NUM 18  // GPIO 18
#define LED_PIN_NUM 27  // GPIO 27 for the LED

// Global variables for sensor file descriptors
int fd_sensor1 = -1, fd_sensor2 = -1;

void setup_sensors();
int read_sensor(int fd);
void update_display(int count);
void trigger_led_for_sensor(int sensorState);
float measure_distance();
void open_sensor_files();

int main() {
    setup_sensors();
    open_sensor_files();
    activate_gpio_pin();
    display_on();

    int count = 0;
    int lastState1 = 0, lastState2 = 0;
    int sensor1TriggeredFirst = 0, sensor2TriggeredFirst = 0;

    while (1) {
        int sensor1 = read_sensor(fd_sensor1);
        int sensor2 = read_sensor(fd_sensor2);

        // Detect rising edge for sensor 1
        if (!lastState1 && sensor1) {
            sensor1TriggeredFirst = 1;
            trigger_led_for_sensor(1);
        }

        // Detect rising edge for sensor 2
        if (!lastState2 && sensor2) {
            sensor2TriggeredFirst = 1;
            trigger_led_for_sensor(2);
        }

        // Check for Sensor 1 -> Sensor 2 sequence
        if (sensor1TriggeredFirst && !lastState2 && sensor2) {
            if (count < 10) {
                count++;  // Increase count only if less than 10
            }
            // Reset flags after sequence completion
            sensor1TriggeredFirst = 0;
            sensor2TriggeredFirst = 0;
            trigger_led_for_sensor(2);
        }

        // Check for Sensor 2 -> Sensor 1 sequence
        if (sensor2TriggeredFirst && !lastState1 && sensor1) {
            if (count > 0) {
                count--;  // Decrease count only if greater than 0
            }
            // Reset flags after sequence completion
            sensor1TriggeredFirst = 0;
            sensor2TriggeredFirst = 0;
            trigger_led_for_sensor(1);
        }

        // Update states for the next iteration
        lastState1 = sensor1;
        lastState2 = sensor2;

        // Update the display if count has changed
        update_display(count);

        // Distance measurement should occur regardless of the count
        float dist = measure_distance();
        if (count == 1) {
            // Check if the distance is between 30 and 40 cm
            if (dist >= 30 && dist <= 40) {
                printf("Optimal distance - between 30 to 40 cm - distance: %.2f cm\n", dist);
                set_value(1, LED_PIN_NUM);  // Turn the LED on
            } else {
                printf("Object not at optimal distance: %.2f cm\n", dist);
                set_value(0, LED_PIN_NUM);  // Turn the LED off
            }
        
        }

        usleep(10000); // 0.1 seconds delay
    }

    // Clean up resources before exiting (if ever reached)
    close(fd_sensor1);
    close(fd_sensor2);
}

void setup_sensors() {
    export_pin(PIR1);
    set_direction(IN, PIR1);
    export_pin(PIR2);
    set_direction(IN, PIR2);
    
    // Initialize GPIO pins for ultrasonic sensor
    export_pin(TRIG_PIN_NUM);
    set_direction(OUT, TRIG_PIN_NUM);
    export_pin(ECHO_PIN_NUM);
    set_direction(IN, ECHO_PIN_NUM);
    export_pin(LED_PIN_NUM);
    set_direction(OUT, LED_PIN_NUM);
}

void open_sensor_files() {
    char value_file_path[128];

    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", PIR1);
    fd_sensor1 = open(value_file_path, O_RDONLY);
    if (fd_sensor1 < 0) {
        perror("Failed to open the value file for sensor 1");
        exit(EXIT_FAILURE);
    }

    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", PIR2);
    fd_sensor2 = open(value_file_path, O_RDONLY);
    if (fd_sensor2 < 0) {
        perror("Failed to open the value file for sensor 2");
        close(fd_sensor1);  // Close the other file descriptor if this one fails
        exit(EXIT_FAILURE);
    }
}

int read_sensor(int fd) {
    char val[3];
    lseek(fd, 0, SEEK_SET);  // Rewind the file pointer to the beginning of the file
    if (read(fd, val, 2) != 2) {
        perror("Failed to read from the value file");
        return -1;  // Returning an error code instead of exiting
    }
    val[2] = '\0';  // Null terminate the string
    return atoi(val);
}

void update_display(int count) {
    uint8_t display_array[4] = {0};
    display_array[3] = count % 10;
    display_handle(display_array);
}

void trigger_led_for_sensor(int sensorState) {
    // Simulate LED activation for sensor state change
    if (sensorState) {
        usleep(5000); // Simulate LED on for 0.05 seconds
    }
}

float measure_distance() {
    // Send a pulse to the sensor
    set_value(1, TRIG_PIN_NUM);
    usleep(10);  // Send trigger pulse for 10 microseconds
    set_value(0, TRIG_PIN_NUM);

    // Wait for echo to start
    while (!get_value(ECHO_PIN_NUM));

    // Measure the duration of the echo pulse
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    while (get_value(ECHO_PIN_NUM));
    gettimeofday(&endTime, NULL);

    // Calculate travel time and distance
    long travelTime = (endTime.tv_sec - startTime.tv_sec) * 1000000L + (endTime.tv_usec - startTime.tv_usec);
    float distance = travelTime / 58.0;
    return distance;
}
