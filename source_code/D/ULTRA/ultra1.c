#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <time.h>

#define TRIG_PIN 17 // GPIO17
#define ECHO_PIN 18 // GPIO18

unsigned long micros() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

int main() {
    int trigger_fd, echo_fd;
    char buffer[40]; // Increased buffer size to accommodate longer strings
    
    // Export GPIO pins
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);
    sprintf(buffer, "%d", TRIG_PIN);
    write(export_fd, buffer, sizeof(buffer));
    sprintf(buffer, "%d", ECHO_PIN);
    write(export_fd, buffer, sizeof(buffer));
    close(export_fd);
    
    // Set direction
    sprintf(buffer, "/sys/class/gpio/gpio%d/direction", TRIG_PIN);
    trigger_fd = open(buffer, O_WRONLY);
    write(trigger_fd, "out", 4);
    close(trigger_fd);
    
    sprintf(buffer, "/sys/class/gpio/gpio%d/direction", ECHO_PIN);
    echo_fd = open(buffer, O_RDONLY);
    close(echo_fd);
    
    // Main loop
    while(1) {
        // Trigger pulse
        sprintf(buffer, "/sys/class/gpio/gpio%d/value", TRIG_PIN);
        trigger_fd = open(buffer, O_WRONLY);
        write(trigger_fd, "1", 2);
        usleep(1); // 10 microseconds
        trigger_fd = open(buffer, O_WRONLY);
        write(trigger_fd, "0", 2);
        close(trigger_fd);

        usleep(2000); // Delay for 0.00146 seconds before starting echo measurement

        // Measure echo time
        unsigned long startTime, travelTime;
        sprintf(buffer, "/sys/class/gpio/gpio%d/value", ECHO_PIN);
        echo_fd = open(buffer, O_RDONLY);
        while(read(echo_fd, &buffer, 1) == 0); // Wait for echo to start
        startTime = micros();
        while(read(echo_fd, &buffer, 1) != 0); // Wait for echo to end
        travelTime = micros() - startTime;
        
        // Calculate distance (cm)
        float distance = travelTime / 58.0;
        
        if(distance < 100) { // Within 1 meter
            printf("Object detected within 1 meter. Distance: %.2f cm\n", distance);
        } else {
            printf("Object at: %.2f cm\n", distance);
        }
        
        close(echo_fd);
        
        // Delay for 1 second
        usleep(1000000); // 1 second
    }
    
    return 0;
}

