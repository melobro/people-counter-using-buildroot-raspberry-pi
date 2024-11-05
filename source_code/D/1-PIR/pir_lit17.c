#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIR_PIN "17" // GPIO 16

int main(void) {
    // Export GPIO pin
    FILE *export_file = fopen("/sys/class/gpio/export", "w");
    if (export_file == NULL) {
        perror("Error exporting GPIO pin");
        exit(EXIT_FAILURE);
    }
    fprintf(export_file, PIR_PIN);
    fclose(export_file);
    
    printf("PIR Sensor and LED Test:\n");

    while(1) {
        // Set GPIO pin direction to input to read PIR sensor value
        FILE *direction_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/direction", "w");
        if (direction_file == NULL) {
            perror("Error setting GPIO direction");
            exit(EXIT_FAILURE);
        }
        fprintf(direction_file, "in");
        fclose(direction_file);

        // Read GPIO pin value
        FILE *value_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/value", "r");
        if (value_file == NULL) {
            perror("Error reading GPIO value");
            exit(EXIT_FAILURE);
        }
        char value;
        fread(&value, sizeof(char), 1, value_file);
        fclose(value_file);

        if (value == '1') {
            printf("Motion-17 detected! .025-Lighting up LED.\n");

            // Set GPIO pin direction to output to turn on the LED
            direction_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/direction", "w");
            if (direction_file == NULL) {
                perror("Error setting GPIO direction");
                exit(EXIT_FAILURE);
            }
            fprintf(direction_file, "out");
            fclose(direction_file);

            // Write to GPIO pin to turn the LED on
            value_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/value", "w");
            if (value_file == NULL) {
                perror("Error writing GPIO value");
                exit(EXIT_FAILURE);
            }
            fprintf(value_file, "1");
            fclose(value_file);

            // Wait for 0.15 seconds
            usleep(150000);

            // Write to GPIO pin to turn the LED off
            value_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/value", "w");
            if (value_file == NULL) {
                perror("Error writing GPIO value");
                exit(EXIT_FAILURE);
            }
            fprintf(value_file, "0");
            fclose(value_file);
        } else {
            printf("No motion\n");
        }

        usleep(150000); // Adjust delay as needed
    }

    return 0;
}

