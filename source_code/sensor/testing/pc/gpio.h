// gpio.h

#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> // For open() flags

// GPIO file paths
#define EXPORT "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio"

// Pin Modes
#define IN "in"
#define OUT "out"

// Pin Voltage Modes
#define LOW 0
#define HIGH 1

// Function prototypes
void setup_gpio(const char *pin, const char *direction);
int read_gpio(const char *pin);

#endif // GPIO_H

