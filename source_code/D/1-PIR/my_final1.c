#include "gpio.h"  // Make sure this includes the proper definitions and functions from gpio.c
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


void setup_sensors();
int read_sensor(int pin);
void update_display(int count);
void log_count(const char *filename, int count);
void trigger_led_for_sensor(int sensorState);
float measure_distance();

int main() {
    setup_sensors();
    activate_gpio_pin();
    display_on();

    int count = 0;
    int lastLoggedCount = -1;

    int lastState1 = 0, lastState2 = 0;
    int sensor1TriggeredFirst = 0, sensor2TriggeredFirst = 0;

while (1) {
    int sensor1 = read_sensor(PIR1);
    int sensor2 = read_sensor(PIR2);

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
        sensor2TriggeredFirst = 0; // Make sure to reset both flags
        trigger_led_for_sensor(2);
    }

    // Check for Sensor 2 -> Sensor 1 sequence
    if (sensor2TriggeredFirst && !lastState1 && sensor1) {
        if (count > 0) {
            count--;  // Decrease count only if greater than 0
        }
        // Reset flags after sequence completion
        sensor1TriggeredFirst = 0;
        sensor2TriggeredFirst = 0; // Make sure to reset both flags
        trigger_led_for_sensor(1);
    }

    // Update states for the next iteration
    lastState1 = sensor1;
    lastState2 = sensor2;

    // Update the display and log file if count has changed
    if (lastLoggedCount != count) {
        update_display(count);
        log_count("people_log.txt", count);
        lastLoggedCount = count;
    }

    // Distance measurement should occur regardless, but actions based on it only when count is 1
    float dist = measure_distance();
    if (count == 1) {
        // Check if the distance is between 30 and 40 cm
        if (dist >= 30 && dist <= 40) {
            printf("Optimal distance - between 30 to 40 cm - distance: %.2f cm\n", dist);
            set_value(1, LED_PIN_NUM);  // Turn the LED on
        } 
        else {
            printf("Object not at optimal distance: %.2f cm\n", dist);
            set_value(0, LED_PIN_NUM);  // Turn the LED off
        }
    }

    usleep(10000); // 0.1 seconds delay
}

}


void setup_sensors() {
    export_pin(PIR1);
    set_direction(IN, PIR1);
    export_pin(PIR2);
    set_direction(IN, PIR2);
    
    // Initialize GPIO pins
    export_pin(TRIG_PIN_NUM);
    export_pin(ECHO_PIN_NUM);
    export_pin(LED_PIN_NUM);

    // Set GPIO pins direction
    set_direction(OUT, TRIG_PIN_NUM);
    set_direction(IN, ECHO_PIN_NUM);
    set_direction(OUT, LED_PIN_NUM);  // Set the LED pin as output
}


int read_sensor(int pin) {
    char value_file_path[1024];
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", pin);
    FILE* value = fopen(value_file_path, "r");
    if (value == NULL) {
        perror("Failed to open the value file");
        exit(EXIT_FAILURE);
    }

    char val[2];
    if (fgets(val, 2, value) == NULL) {
        perror("Failed to read from the value file");
        fclose(value);
        exit(EXIT_FAILURE);
    }
    fclose(value);
    return atoi(val);
}


void update_display(int count) {
    uint8_t display_array[4] = {0};
    display_array[3] = count % 10;
    display_handle(display_array);
}


void log_count(const char *filename, int count) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Count: %d\n", count);
    fclose(file); // Close the file
}


void trigger_led_for_sensor(int sensorState) {
    // This is a placeholder function.
    // In real application, here you would activate the LED linked to the sensor.
    // But as per your constraint, we just simulate a delay representing the LED being on.
    if (sensorState) {
        usleep(5000); // LED on for 0.05 seconds
    }
}


// Function to measure distance
float measure_distance() {
    // Send a pulse to the sensor
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
