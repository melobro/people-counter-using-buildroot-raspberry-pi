#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_PATH "/sys/class/gpio/gpio16/"

void export_gpio(int pin) {
    int fd_export = open("/sys/class/gpio/export", O_WRONLY);
    if (fd_export == -1) {
        perror("Error exporting GPIO");
        exit(1);
    }

    dprintf(fd_export, "%d", pin);

    close(fd_export);
}

void set_direction(int pin, int direction) {
    char path[50];
    sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
    int fd_direction = open(path, O_WRONLY);
    if (fd_direction == -1) {
        perror("Error setting GPIO direction");
        exit(1);
    }

    if (direction)
        dprintf(fd_direction, "out");
    else
        dprintf(fd_direction, "in");

    close(fd_direction);
}

void set_value(int pin, int value) {
    char path[50];
    sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
    int fd_value = open(path, O_WRONLY);
    if (fd_value == -1) {
        perror("Error setting GPIO value");
        exit(1);
    }

    if (value)
        dprintf(fd_value, "1");
    else
        dprintf(fd_value, "0");

    close(fd_value);
}

int main() {
    int pin = 16; // GPIO 17
    export_gpio(pin);
    set_direction(pin, 1); // Set GPIO as output

    while (1) {
        set_value(pin, 1); // Turn LED on
        sleep(1); // Wait for 1 second
        set_value(pin, 0); // Turn LED off
        sleep(1); // Wait for 1 second
    }

    return 0;
}

