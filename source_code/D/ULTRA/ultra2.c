#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> // for clock_gettime

// Define GPIO pin numbers
#define TRIGGER_PIN 17
#define ECHO_PIN 18

// Function to initialize ultrasonic sensor
void initUltrasonicSensor() {
    // Export GPIO pins
    FILE *export_file = fopen("/sys/class/gpio/export", "w");
    fprintf(export_file, "%d", TRIGGER_PIN);
    fprintf(export_file, "%d", ECHO_PIN);
    fclose(export_file);
    
    // Set trigger pin as output and echo pin as input
    FILE *direction_file = fopen("/sys/class/gpio/gpio18/direction", "w");
    fprintf(direction_file, "out");
    fclose(direction_file);
    
    direction_file = fopen("/sys/class/gpio/gpio24/direction", "w");
    fprintf(direction_file, "in");
    fclose(direction_file);
    
    // Ensure trigger pin is low initially
    FILE *trigger_file = fopen("/sys/class/gpio/gpio18/value", "w");
    fprintf(trigger_file, "0");
    fclose(trigger_file);
}

// Function to get current time in microseconds
unsigned long micros() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (unsigned long)ts.tv_sec * 1000000 + (unsigned long)ts.tv_nsec / 1000;
}

// Function to get distance measurement from ultrasonic sensor in centimeters
float getDistance() {
    long duration;
    float distance;
    FILE *echo_file;
    
    // Trigger ultrasonic sensor
    FILE *trigger_file = fopen("/sys/class/gpio/gpio18/value", "w");
    fprintf(trigger_file, "1");
    fclose(trigger_file);
    usleep(10);  // 10 microseconds
    trigger_file = fopen("/sys/class/gpio/gpio18/value", "w");
    fprintf(trigger_file, "0");
    fclose(trigger_file);

    // Measure the duration of the echo pulse
    while ((echo_file = fopen("/sys/class/gpio/gpio24/value", "r")) == NULL);
    char echo_value;
    while ((echo_value = fgetc(echo_file)) == '0');
    long startTime = micros();
    while ((echo_value = fgetc(echo_file)) == '1');
    long endTime = micros();
    fclose(echo_file);

    // Calculate distance in centimeters
    duration = endTime - startTime;
    distance = (float)duration * 0.034 / 2.0; // Speed of sound is 343 m/s = 0.034 cm/us
    return distance;
}

int main() {
    // Initialize ultrasonic sensor
    initUltrasonicSensor();

    while (1) {
        // Wait 2 seconds
        sleep(2);
        
        // Get the distance in centimeters
        float distance = getDistance();

        // Print the distance information to the screen
        printf("Distance: %.2f cm\n", distance);
    }

    return 0;
}

