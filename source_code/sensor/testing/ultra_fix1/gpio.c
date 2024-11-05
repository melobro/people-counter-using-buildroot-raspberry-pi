#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BUFFER_MAX 3
#define DIRECTION_MAX 35
#define VALUE_MAX 30
#define EXPORT_FILE "/sys/class/gpio/export"
#define UNEXPORT_FILE "/sys/class/gpio/unexport"

// Function to export a GPIO pin
void export_pin(int pin) {
    FILE *export_file = fopen(EXPORT_FILE, "w");
    if (export_file == NULL) {
        perror("Error exporting pin");
        exit(EXIT_FAILURE);
    }
    fprintf(export_file, "%d", pin);
    fclose(export_file);
}

// Function to set the direction of a GPIO pin
void set_direction(int direction, int pin) {
    char buffer[DIRECTION_MAX];
    snprintf(buffer, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    FILE *direction_file = fopen(buffer, "w");
    if (direction_file == NULL) {
        perror("Error setting direction");
        exit(EXIT_FAILURE);
    }
    if (direction == OUT)
        fprintf(direction_file, "out");
    else if (direction == IN)
        fprintf(direction_file, "in");
    fclose(direction_file);
}

// Function to write a value to a GPIO pin
void write_value(int value, int pin) {
    char buffer[VALUE_MAX];
    snprintf(buffer, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    FILE *value_file = fopen(buffer, "w");
    if (value_file == NULL) {
        perror("Error writing value");
        exit(EXIT_FAILURE);
    }
    if (value == LOW)
        fprintf(value_file, "0");
    else if (value == HIGH)
        fprintf(value_file, "1");
    fclose(value_file);
}

// Function to read pulse length from a GPIO pin
long read_pulse_length(int pin) {
    struct timeval start, end;
    long micros;

    FILE *value_file;
    char value[BUFFER_MAX];
    char buffer[VALUE_MAX];
    snprintf(buffer, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);

    value_file = fopen(buffer, "r");
    if (value_file == NULL) {
        perror("Error reading value");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL);
    fseek(value_file, 0, SEEK_SET);
    fread(value, sizeof(char), BUFFER_MAX, value_file);
    while (strncmp(value, "1", 1) != 0) {
        fseek(value_file, 0, SEEK_SET);
        fread(value, sizeof(char), BUFFER_MAX, value_file);
    }
    gettimeofday(&end, NULL);
    fclose(value_file);

    micros = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));

    return micros;
}

