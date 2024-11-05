#ifndef GPIO_H
#define GPIO_H

// Define constants for pin direction
#define IN  0
#define OUT 1

// Define constants for pin values
#define LOW  0
#define HIGH 1

// Function prototypes
void export_pin(int pin);
void set_direction(int direction, int pin);
void write_value(int value, int pin);
long read_pulse_length(int pin);

#endif /* GPIO_H */

