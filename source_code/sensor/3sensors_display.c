/**
 * @file main_3sensors.c
 * @brief People counter using PIR sensors
 *
 * This program counts the number of people entering or exiting a room using PIR sensors.
 * The count is logged to a file with timestamps.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Define GPIO paths
#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio"

/**
 * @brief Set up a GPIO pin
 * 
 * @param pin GPIO pin number as a string
 * @param direction Direction of the GPIO pin ("in" or "out")
 */
 
// Function to log sensor states
void log_sensor_states(const char *filename, int value1, int value2, int value3) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening sensor log file");
        return;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    fprintf(file, "[%s] Sensor States - 1: %d, 2: %d, 3: %d\n", time_str, value1, value2, value3);
    fclose(file);
}
 
void setup_gpio(const char *pin, const char *direction) {
    int fd;
    char path[100];

    fd = open(GPIO_EXPORT, O_WRONLY);
    write(fd, pin, strlen(pin));
    close(fd);

    snprintf(path, sizeof(path), GPIO_DIRECTION_PATH "%s/direction", pin);
    fd = open(path, O_WRONLY);
    write(fd, direction, strlen(direction));
    close(fd);
}

/**
 * @brief Read a GPIO pin value
 * 
 * @param pin GPIO pin number as a string
 * @return int GPIO value (0 or 1)
 */
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

/**
 * @brief Log the count to a file
 * 
 * @param filename Name of the log file
 * @param count Current people count
 * @param direction Direction of movement ("Entering" or "Exiting")
 */
void log_count(const char *filename, int count, const char *direction) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    fprintf(file, "[%s] %s: Count is now %d\n", time_str, direction, count);
    fclose(file);
}

/**
 * @brief Main application entry point
 * 
 * @return int Exit status
 */
int main() {
    const char *sensor_pins[] = {"23", "24", "25"};
    int count = 0;
    const char *log_filename = "people_count_log.txt";
    const char *sensor_log_filename = "sensor_activity_log.txt";

    // Setup each sensor pin
    for (int i = 0; i < 3; ++i) {
        setup_gpio(sensor_pins[i], "in");
    }

    // Main loop
    while (1) {
        int value1 = read_gpio(sensor_pins[0]);
        int value2 = read_gpio(sensor_pins[1]);
        int value3 = read_gpio(sensor_pins[2]);

        // Log sensor states for debugging
        log_sensor_states(sensor_log_filename, value1, value2, value3);

        // Improved logic to determine direction and count people
        // This logic needs to be refined based on your specific sensor setup and requirements
        if (value1 && !value2 && !value3) {
            count++;
            printf("Entering: Count is now %d\n", count);
            log_count(log_filename, count, "Entering");
        } else if (!value1 && !value2 && value3) {
            if (count > 0) { // Only decrement if count is above zero
                count--;
                printf("Exiting: Count is now %d\n", count);
                log_count(log_filename, count, "Exiting");
            }
        }

        usleep(100000); // Adjusted debounce delay
    }

    return 0;
}
