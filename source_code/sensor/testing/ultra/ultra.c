#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h> // For gettimeofday()

#define TRIG_PIN 25  // Set GPIO pin for HC-SR04 Trigger
#define ECHO_PIN 26  // Set GPIO pin for HC-SR04 Echo

void setup_hc_sr04();
float read_distance();
long get_pulse_length();

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
    set_value(TRIG_PIN, LOW);
    usleep(2);
    
    // Trigger the sensor by setting the trigPin high for 10 microseconds
    set_value(TRIG_PIN, HIGH);
    usleep(10);
    set_value(TRIG_PIN, LOW);
    
    long duration = get_pulse_length();
    
    // Calculate the distance
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    
    return distance;
}

long get_pulse_length() {
    struct timeval start, end;
    long startMicro, endMicro, duration;
    
    // Wait for echo to be LOW
    while (gpio_read(ECHO_PIN) == HIGH);
    
    // Wait for echo to start (go HIGH)
    while (gpio_read(ECHO_PIN) == LOW);
    gettimeofday(&start, NULL);
    
    // Wait for echo to end (go LOW again)
    while (gpio_read(ECHO_PIN) == HIGH);
    gettimeofday(&end, NULL);
    
    startMicro = (start.tv_sec * 1000000) + start.tv_usec;
    endMicro = (end.tv_sec * 1000000) + end.tv_usec;
    duration = endMicro - startMicro;
    
    return duration;
}

