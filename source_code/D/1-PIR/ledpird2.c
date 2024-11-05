#include "gpio.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// GPIO pin definitions
#define PIR1 16
#define PIR2 6

void setup_sensors();
int read_sensor(int pin);
void update_display(int count);
void log_count(const char *filename, int count);
void trigger_led_for_sensor(int sensorState);

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

    // Detect sensor 1
    if (!lastState1 && sensor1) {
        sensor1TriggeredFirst = 1;
        trigger_led_for_sensor(1);
    }

    // Detect sensor 2
    if (!lastState2 && sensor2) {
        sensor2TriggeredFirst = 1;
        trigger_led_for_sensor(2);
    }

    // Check for 1 2 sequence
    if (sensor1TriggeredFirst && !lastState2 && sensor2) {
        if (count < 10) {
            count++;  // Increase count only if less than 10
        }
        // Reset flags after sequence completion
        sensor1TriggeredFirst = 0;
        sensor2TriggeredFirst = 0; // Make sure to reset both flags
        trigger_led_for_sensor(2);
    }

    // Check for 2 1 sequence
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
        usleep(10000); // 0.1 seconds delay
}

}


void setup_sensors() {
    export_pin(PIR1);
    set_direction(IN, PIR1);
    export_pin(PIR2);
    set_direction(IN, PIR2);
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
    if (sensorState) {
        usleep(5000); // LED on for 0.05 seconds
    }
}

