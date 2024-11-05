#include "gpio.h"

/**
 * @brief Export a GPIO pin.
 * 
 * @param num GPIO pin number to export.
 * @return 0 on success, -1 on failure.
 */
int export_pin(int num) {
    FILE* export_file = fopen(EXPORT, "w"); /**< Open the export file for writing */
    if (export_file == NULL) {
        printf("Failed to open the export file\n"); /**< Print an error message if the file opening fails */
        exit(-1); /**< Exit the program with an error code */
    }
    fprintf(export_file, "%d", num); /**< Write the GPIO pin number to the export file */
    fclose(export_file); /**< Close the export file */
    return 0; /**< Return 0 to indicate success */
}

/**
 * @brief Unexport a GPIO pin.
 * 
 * @param num GPIO pin number to unexport.
 * @return 0 on success, -1 on failure.
 */
int unexport_pin(int num) {
    FILE* unexport_file = fopen(UNEXPORT, "w"); /**< Open the unexport file for writing */
    if (unexport_file == NULL) {
        printf("Failed to open the unexport file\n"); /**< Print an error message if the file opening fails */
        exit(-1); /**< Exit the program with an error code */
    }
    fprintf(unexport_file, "%d", num); /**< Write the GPIO pin number to the unexport file */
    fclose(unexport_file); /**< Close the unexport file */
    return 0; /**< Return 0 to indicate success */
}

/**
 * @brief Set the direction of a GPIO pin.
 * 
 * @param mode Direction mode ("in" or "out").
 * @param num GPIO pin number.
 * @return 0 on success, -1 on failure.
 */
int set_direction(char *mode, int num) {
    char direction_file_path[1024]; /**< Path to the direction file */
    snprintf(direction_file_path, sizeof(direction_file_path), "/sys/class/gpio/gpio%d/direction", num);
    FILE* direction = fopen(direction_file_path, "w"); /**< Open the direction file for writing */
    if (direction == NULL) {
        printf("Failed to open the direction file\n"); /**< Print an error message if the file opening fails */
        exit(-1); /**< Exit the program with an error code */
    }
    fputs(mode, direction); /**< Write the specified mode (input or output) to the direction file */
    fclose(direction); /**< Close the direction file */
    return 0; /**< Return 0 to indicate success */
}

/**
 * @brief Set the value of a GPIO pin.
 * 
 * @param val Value to set (0 or 1).
 * @param num GPIO pin number.
 * @return 0 on success, -1 on failure.
 */
int set_value(int val, int num) {
    char value_file_path[1024]; /**< Path to the value file */
    snprintf(value_file_path, sizeof(value_file_path), "/sys/class/gpio/gpio%d/value", num);
    FILE* value = fopen(value_file_path, "w"); /**< Open the value file for writing */
    if (value == NULL) {
        printf("Failed to open the value file\n"); /**< Print an error message if the file opening fails */
        exit(-1); /**< Exit the program with an error code */
    }
    fprintf(value, "%d", val); /**< Write the specified value (0 or 1) to the value file */
    fclose(value); /**< Close the value file */
    return 0; /**< Return 0 to indicate success */
}
