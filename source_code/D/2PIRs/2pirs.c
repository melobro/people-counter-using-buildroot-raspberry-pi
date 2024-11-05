#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIR_PIN_1 "17" // GPIO 17
#define PIR_PIN_2 "27" // GPIO 27

int main(void) {
    // Export GPIO pins
    FILE *export_file_1 = fopen("/sys/class/gpio/export", "w");
    if (export_file_1 == NULL) {
        perror("Error exporting GPIO pin 1");
        exit(EXIT_FAILURE);
    }
    fprintf(export_file_1, PIR_PIN_1);
    fclose(export_file_1);

    FILE *export_file_2 = fopen("/sys/class/gpio/export", "w");
    if (export_file_2 == NULL) {
        perror("Error exporting GPIO pin 2");
        exit(EXIT_FAILURE);
    }
    fprintf(export_file_2, PIR_PIN_2);
    fclose(export_file_2);
    
    // Set GPIO pins direction to input
    FILE *direction_file_1 = fopen("/sys/class/gpio/gpio" PIR_PIN_1 "/direction", "w");
    if (direction_file_1 == NULL) {
        perror("Error setting GPIO 1 direction");
        exit(EXIT_FAILURE);
    }
    fprintf(direction_file_1, "in");
    fclose(direction_file_1);

    FILE *direction_file_2 = fopen("/sys/class/gpio/gpio" PIR_PIN_2 "/direction", "w");
    if (direction_file_2 == NULL) {
        perror("Error setting GPIO 2 direction");
        exit(EXIT_FAILURE);
    }
    fprintf(direction_file_2, "in");
    fclose(direction_file_2);

    printf("PIR Sensors Test\n");

    while(1) {
        // Read GPIO pin 1 value
        FILE *value_file_1 = fopen("/sys/class/gpio/gpio" PIR_PIN_1 "/value", "r");
        if (value_file_1 == NULL) {
            perror("Error reading GPIO 1 value");
            exit(EXIT_FAILURE);
        }
        char value_1;
        fread(&value_1, sizeof(char), 1, value_file_1);
        fclose(value_file_1);

        // Read GPIO pin 2 value
        FILE *value_file_2 = fopen("/sys/class/gpio/gpio" PIR_PIN_2 "/value", "r");
        if (value_file_2 == NULL) {
            perror("Error reading GPIO 2 value");
            exit(EXIT_FAILURE);
        }
        char value_2;
        fread(&value_2, sizeof(char), 1, value_file_2);
        fclose(value_file_2);

        if (value_1 == '1') {
            printf("Motion detected on Sensor 1!\n");
        }

        if (value_2 == '1') {
            printf("Motion detected on Sensor 2!\n");
        }

        usleep(500000); // Adjust delay as needed
    }

    return 0;
}

