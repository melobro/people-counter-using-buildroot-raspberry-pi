#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int export_pin(int num) {
    FILE *file = fopen(EXPORT, "w");
    if (file == NULL) {
        perror("Error exporting GPIO pin");
        return -1;
    }
    fprintf(file, "%d", num);
    fclose(file);
    return 0;
}


int unexport_pin(int num) {
    FILE *file = fopen(UNEXPORT, "w");
    if (file == NULL) {
        perror("Error unexporting GPIO pin");
        return -1;
    }
    fprintf(file, "%d", num);
    fclose(file);
    return 0;
}


int set_direction(char *mode, int num) {
    char path[35];
    snprintf(path, 35, "%s/gpio%d/direction", GPIO_ROOT, num);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error setting GPIO direction");
        return -1;
    }
    fprintf(file, "%s", mode);
    fclose(file);
    return 0;
}


int set_value(int val, int num) {
    char path[35];
    snprintf(path, 35, "%s/gpio%d/value", GPIO_ROOT, num);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error writing GPIO value");
        return -1;
    }
    fprintf(file, "%d", val);
    fclose(file);
    
    
    return 0;
}


int get_value(int num) {
    char path[35];
    char value_str[3];
    int value;

    snprintf(path, 35, "%s/gpio%d/value", GPIO_ROOT, num);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error reading GPIO value");
        return -1; // Return -1 to indicate the error
    }
    if (fgets(value_str, 3, file) == NULL) {
        perror("Failed to read value");
        fclose(file);
        return -1;
    }
    fclose(file);
    value = atoi(value_str);
    return value; // Return the read value
}

