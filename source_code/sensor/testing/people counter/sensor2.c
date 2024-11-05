#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio"

void setup_gpio(const char *pin) {
    int fd;
    char path[100];

    // Export the pin
    fd = open(GPIO_EXPORT, O_WRONLY);
    write(fd, pin, strlen(pin));
    close(fd);

    // Set the pin direction to input
    snprintf(path, sizeof(path), GPIO_DIRECTION_PATH "%s/direction", pin);
    fd = open(path, O_WRONLY);
    write(fd, "in", 2);
    close(fd);
}

int read_gpio(const char *pin) {
    char value_str[3];
    char path[100];
    int fd;

    snprintf(path, sizeof(path), GPIO_VALUE_PATH "%s/value", pin);
    fd = open(path, O_RDONLY);
    read(fd, value_str, 3);
    close(fd);

    return (atoi(value_str));
}

int main() {
    const char *sensor_pins[] = {"23", "24", "25"};
    int i;
    time_t last_trigger_time[3] = {0, 0, 0};
    int debounce_seconds = 5; // Time in seconds to ignore subsequent triggers

    // Setup each sensor pin
    for (i = 0; i < 3; ++i) {
        setup_gpio(sensor_pins[i]);
    }

    // Main loop
    while (1) {
        for (i = 0; i < 3; ++i) {
            if (read_gpio(sensor_pins[i])) {
                time_t current_time = time(NULL);
                if (current_time - last_trigger_time[i] > debounce_seconds) {
                    printf("Motion detected on pin %s!\n", sensor_pins[i]);
                    last_trigger_time[i] = current_time;
                }
            }
        }
        usleep(100000); // 100 ms
    }

    return 0;
}

