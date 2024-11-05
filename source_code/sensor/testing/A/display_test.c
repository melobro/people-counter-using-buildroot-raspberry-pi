#include "display.h"

int main() {
    // Initialize the display
    activate_gpio_pin();
    display_on();

    // Display numbers 0 to 9 with a 1-second delay between each
    for (int i = 0; i <= 9; ++i) {
        uint8_t display_array[4] = {0};
        display_array[3] = display_segment(i); // Display the number on the rightmost segment
        display_handle(display_array);
        sleep(1); // 1-second delay
    }

	
    void set_gpio_value(int pin, int value) {
    char path[100];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    FILE* value_file = fopen(path, "w");
    if (value_file == NULL) {
        perror("Failed to open the value file for GPIO pin");
        printf("GPIO Pin: %d\n", pin);
        printf("Attempted Path: %s\n", path);
        exit(-1);
    }
    fprintf(value_file, "%d", value);
    fclose(value_file);
  }

	
    // Turn off the display
    display_off();
    deactivate_gpio_pin();

    return 0;
}

