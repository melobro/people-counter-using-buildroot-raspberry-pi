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

void log_event(const char *filename, const char *event) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline

    fprintf(file, "[%s] %s\n", time_str, event);
    fclose(file);
}

int main() {
    const char *sensor_pins[] = {"23", "24", "25"};
    const char *log_filename = "people_counter_log.txt";

    for (int i = 0; i < 3; ++i) {
        setup_gpio(sensor_pins[i], "in");
    }

    int last_state[3] = {0, 0, 0};
    int current_state[3];

    while (1) {
        for (int i = 0; i < 3; ++i) {
            current_state[i] = read_gpio(sensor_pins[i]);
        }

        // Simple logic to detect entry or exit
        if (current_state[0] && !last_state[0]) {
            log_event(log_filename, "Person entered");
        } else if (current_state[2] && !last_state[2]) {
            log_event(log_filename, "Person exited");
       
	    }

    // Update last states
    for (int i = 0; i < 3; ++i) {
        last_state[i] = current_state[i];
    }

    usleep(100000); // 100 ms delay to prevent rapid re-triggering
}

return 0;
}
