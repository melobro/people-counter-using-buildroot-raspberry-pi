#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define GPIO_PATH "/sys/class/gpio/gpio17/"

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

int read_pir_sensor(char* pin) {
    char path[50];
    sprintf(path, "/sys/class/gpio/gpio%s/value", pin);
    FILE *value_file = fopen(path, "r");
    if (value_file == NULL) {
        perror("Error reading GPIO value");
        exit(EXIT_FAILURE);
    }
    char value;
    fread(&value, sizeof(char), 1, value_file);
    fclose(value_file);

    return value == '1';
}

int main() {
    int led_pin = 17; // GPIO 17
    int pir_pin = 17; // GPIO 17 for PIR sensor
    export_gpio(led_pin);
    set_direction(led_pin, 1); // Set GPIO as output
    export_gpio(pir_pin);
    set_direction(pir_pin, 0); // Set GPIO as input

    printf("PIR Sensor Test\n");

    int motion_detected = 0;

    while (1) {
        if (!motion_detected) {
            if (read_pir_sensor("17")) {
                motion_detected = 1;
                printf("Motion detected!\n");
                set_value(led_pin, 1); // Turn LED on
                sleep(2); // Wait for 2 seconds
                set_value(led_pin, 0); // Turn LED off
            }
        } else {
            if (!read_pir_sensor("17")) {
                motion_detected = 0;
            }
        }

        usleep(500000); // Adjust delay as needed
    }

    return 0;
}

