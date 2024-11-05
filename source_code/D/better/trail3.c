#include "gpio.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PIR1 16
#define PIR2 6
#define TRIG_PIN_NUM 17
#define ECHO_PIN_NUM 18
#define LED_PIN_NUM 27

int fd_sensor1 = -1, fd_sensor2 = -1;
int ultrasonic_active = 0;

void setup_sensors();
int read_sensor(int fd);
void update_display(int count);
void trigger_led_for_sensor(int sensorState);
float measure_distance();
void open_sensor_files();

int main() {
    setup_sensors();
    open_sensor_files();
    activate_gpio_pin();
    display_on();

    int count = 0;
    int lastState1 = 0, lastState2 = 0;
    int sensor1TriggeredFirst = 0, sensor2TriggeredFirst = 0;

    while (1) {
        int sensor1 = read_sensor(fd_sensor1);
        int sensor2 = read_sensor(fd_sensor2);

        if (!lastState1 && sensor1) {
            sensor1TriggeredFirst = 1;
            trigger_led_for_sensor(1);
            ultrasonic_active = 1;  // Start ultrasonic sensor when PIR1 is triggered
        }

        if (!lastState2 && sensor2) {
            sensor2TriggeredFirst = 1;
            trigger_led_for_sensor(2);
            ultrasonic_active = 0;  // Stop ultrasonic sensor when PIR2 is triggered
        }

        if (sensor1TriggeredFirst && !lastState2 && sensor2) {
            count++;
            sensor1TriggeredFirst = 0;
            sensor2TriggeredFirst = 0;
            trigger_led_for_sensor(2);
        }

        if (sensor2TriggeredFirst && !lastState1 && sensor1) {
            count--;
            sensor1TriggeredFirst = 0;
            sensor2TriggeredFirst = 0;
            trigger_led_for_sensor(1);
        }

        lastState1 = sensor1;
        lastState2 = sensor2;

        update_display(count);

        // Activate HC-SR04 sensor based on ultrasonic_active flag
        if (ultrasonic_active) {
            float dist = measure_distance();
            if(count==0){
                if (dist >= 30 && dist <= 40) {
                    printf("Optimal distance - between 30 to 40 cm - distance: %.2f cm\n", dist);
                    set_value(1, LED_PIN_NUM);
                } else {
                    printf("Object not at optimal distance: %.2f cm\n", dist);
                    set_value(0, LED_PIN_NUM);
                }
            }
        }

        usleep(10000); // 0.1 seconds delay
    }

    close(fd_sensor1);
    close(fd_sensor2);
}

void setup_sensors() {
    export_pin(PIR1);
    set_direction(IN, PIR1);
    export_pin(PIR2);
    set_direction(IN, PIR2);
    export_pin(TRIG_PIN_NUM);
    set_direction(OUT, TRIG_PIN_NUM);
    export_pin(ECHO_PIN_NUM);
    set_direction(IN, ECHO_PIN_NUM);
    export_pin(LED_PIN_NUM);
    set_direction(OUT, LED_PIN_NUM);
}

void open_sensor_files() {
    char value_file_path[128];

    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", PIR1);
    fd_sensor1 = open(value_file_path, O_RDONLY);
    if (fd_sensor1 < 0) {
        perror("Failed to open the value file for sensor 1");
        exit(EXIT_FAILURE);
    }

    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", PIR2);
    fd_sensor2 = open(value_file_path, O_RDONLY);
    if (fd_sensor2 < 0) {
        perror("Failed to open the value file for sensor 2");
        close(fd_sensor1);
        exit(EXIT_FAILURE);
    }
}

int read_sensor(int fd) {
    char val[3];
    lseek(fd, 0, SEEK_SET);
    if (read(fd, val, 2) != 2) {
        perror("Failed to read from the value file");
        return -1;
    }
    val[2] = '\0';
    return atoi(val);
}

void update_display(int count) {
    uint8_t display_array[4] = {0};
    display_array[3] = count % 10;
    display_handle(display_array);
}

void trigger_led_for_sensor(int sensorState) {
    if (sensorState) {
        usleep(5000);
    }
}

float measure_distance() {
    if (!ultrasonic_active) {
        return 0.0; // Return zero if ultrasonic sensor is not active
    }
    set_value(1, TRIG_PIN_NUM);
    usleep(10);
    set_value(0, TRIG_PIN_NUM);

    while (!get_value(ECHO_PIN_NUM));

    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    while (get_value(ECHO_PIN_NUM));
    gettimeofday(&endTime, NULL);

    long travelTime = (endTime.tv_sec - startTime.tv_sec) * 1000000L + (endTime.tv_usec - startTime.tv_usec);
    float distance = travelTime / 58.0;
    return distance;
}

