#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIR_PIN "17" // GPIO 17

int main(void) {
    // Export GPIO pin
    FILE *export_file = fopen("/sys/class/gpio/export", "w");
    if (export_file == NULL) {
        perror("Error exporting GPIO pin");
        exit(EXIT_FAILURE);
    }
    fprintf(export_file, PIR_PIN);
    fclose(export_file);
    
    // Set GPIO pin direction to input
    FILE *direction_file = fopen("/sys/class/gpio/gpio" PIR_PIN "/direction", "w");
    if (direction_file == NULL) {
        perror("Error setting GPIO direction");
        exit(EXIT_FAILURE);
    }
    fprintf(direction_file, "in");
    fclose(direction_file);

    printf("PIR Sensor Test:\n");

    while(1) {
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
            printf("Motion-17 detected!\n");
        } else {
            printf("No motion\n");
        }

        usleep(500000); // Adjust delay as needed
    }

    return 0;
}

