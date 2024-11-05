/**
 * @file gpio.h
 * 
 * @brief GPIO pin manipulation interface copied from the provided exercise source.
 * 
 * Source: https://buyzero.de/blogs/news/alles-uber-gpio-pins
 * 
 * @version 0.1
 * @date 2023-12-04
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Directories for the Files to control the GPIOs of the Pi.
 */
#define GPIO_ROOT "/sys/class/gpio"
#define EXPORT "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/unexport"

/**
 * @brief Pin Modes.
 */
#define IN "in"
#define OUT "out"

/**
 * @brief Pin Voltage Modes.
 * 0 -> Low (no Voltage)
 * 1 -> High (Voltage)
 */
#define LOW 0
#define HIGH 1

/**
 * @brief Create Directory to manipulate the GPIO Pin.
 * 
 * @param num Number of the GPIO Pin.
 * @return Returns 0 if succeeded.
 */
int export_pin(int num);

/**
 * @brief Delete Directory to manipulate the GPIO Pin.
 * 
 * @param num Number of the GPIO Pin.
 * @return Returns 0 if succeeded.
 */
int unexport_pin(int num);

/**
 * @brief Set the direction of the GPIO Pin (Read/Input | Write/Output).
 * 
 * @param mode Value can be IN or OUT.
 * @param num Number of the GPIO Pin.
 * @return Returns 0 if succeeded.
 */
int set_direction(char *mode, int num);

/**
 * @brief Set the value of the GPIO Pin.
 * 
 * @param val Value can be LOW or HIGH.
 * @param num Number of the GPIO Pin.
 * @return Returns 0 if succeeded.
 */
int set_value(int val, int num);



