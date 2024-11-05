// gpio.c

#include "gpio.h"

void setup_gpio(const char *pin, const char *direction) {
    int fd;
    char path[100];

    // Export the pin
    fd = open(EXPORT, O_WRONLY);
    write(fd, pin, strlen(pin));
    close(fd);

    // Set the pin direction
    snprintf(path, sizeof(path), GPIO_DIRECTION_PATH "%s/direction", pin);
    fd = open(path, O_WRONLY);
    write(fd, direction, strlen(direction));
    close(fd);
}

int read_gpio(const char *pin) {
    char value_str[3];
    char path[100];
    int fd;

    // Read the pin value
    snprintf(path, sizeof(path), GPIO_VALUE_PATH "%s/value", pin);
    fd = open(path, O_RDONLY);
    read(fd, value_str, 3);
    close(fd);

    return atoi(value_str);
}

