#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h" 

#define LED_PIN 16 
#define PIR_PIN 16 

// Function to setup GPIO pins for the PIR sensor and LED
void setup_sensors() {
    export_pin(LED_PIN);       
    set_direction("out", LED_PIN); 
    export_pin(PIR_PIN);       
    set_direction("in", PIR_PIN); 
}

// Function to read the sensor state
int read_sensor(int pin) {
    char value_file_path[1024];
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", pin);
    FILE* value_file = fopen(value_file_path, "r"); // Open the value file for reading
    if (value_file == NULL) {
        printf("Failed to open the value file\n"); 
        exit(-1); 
    }
    char value;
    fread(&value, 1, 1, value_file); 
    fclose(value_file); 
    return value == '1'; 
}

int main() {
    setup_sensors(); 

    printf("PIR Sensor Test\n");

    int motion_detected = 0;

    while (1) {
        if (!motion_detected) {
            if (read_sensor(PIR_PIN)) { // Check if motion is detected
                motion_detected = 1;
                printf("Motion-16 detected!\n");
                set_value(1, LED_PIN); 
                sleep(0.25); 
                set_value(0, LED_PIN); 
            }
        } else {
            if (!read_sensor(PIR_PIN)) { 
                motion_detected = 0;
            }
        }
        usleep(500000); 
    }

    return 0;
}

