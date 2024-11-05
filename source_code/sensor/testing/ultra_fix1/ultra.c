#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRIG_PIN 25  // Set GPIO pin for HC-SR04 Trigger
#define ECHO_PIN 26  // Set GPIO pin for HC-SR04 Echo

void setup_hc_sr04();
float read_distance();

int main() {
    setup_hc_sr04();

    while (1) {
        float distance = read_distance();
        
        // Check if the distance is within 1 meter
        if (distance <= 100.0) {
            printf("too near\n");
        }
        
        usleep(100000); // Delay a short time before next measurement
    }

    return 0;
}

void setup_hc_sr04() {
    export_pin(TRIG_PIN);
    set_direction(OUT, TRIG_PIN);
    export_pin(ECHO_PIN);
    set_direction(IN, ECHO_PIN);
}

float read_distance() {
    // Ensure trigger pin is low
    write_value(LOW, TRIG_PIN);
    usleep(2);
    
    // Trigger the sensor by setting the trigPin high for 10 microseconds
    write_value(HIGH, TRIG_PIN);
    usleep(10);
    write_value(LOW, TRIG_PIN);
    
    // Read the signal from the echo pin, measure sound wave travel time in microseconds
    long duration = read_pulse_length(ECHO_PIN);
    
    // Calculate the distance
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    
    return distance;
}

