/**
 * @file display.h
 * 
 * @brief Header file for Course CE31 Embedded System (modified) 
 * @version 0.1
 * @date 2023-12-04
 */

#include <stdint.h>
#include "gpio.h"

/**
 * @brief GPIO pin assignments for CLK and DIO
 */
#define CLK 23
#define DIO 24

/**
 * @brief Commands used to manipulate the display
 */
#define Command_1     0x40
#define Command_2     0xc0
#define Command_3_on  0x88
#define Command_3_off 0x80

/**
 * @brief Hexadecimal data for the representation of numbers and characters on the display
 */
#define zero  0x3f
#define one   0x06
#define two   0x5b
#define three 0x4f
#define four  0x66
#define five  0x6d
#define six   0x7d
#define seven 0x07
#define eight 0x7f
#define nine  0x6f
#define minus 0x40
#define E     0x79
#define r     0x50

/**
 * @brief Functions to activate/deactivate the required GPIO pins 
 */
void activate_gpio_pin();
void deactivate_gpio_pin();

/**
 * @brief Functions to change the GPIO pin mode (High|Low)
 */
void display_clk_high();
void display_clk_low();
void display_dio_high();
void display_dio_low();

/**
 * @brief Functions to start and stop commands
 */
void display_start_cmd();
void display_end_cmd();

/**
 * @brief Functions to transmit data/commands using rising and falling edges
 * @*txdata Pointer to an array of intiger
 * @data_lengt Lenght of the array
 */
void read_bit(int *tx_data);
void transmit_command(uint8_t *tx_data);
void transmit_data(uint8_t *tx_data, uint8_t data_length);

/**
 * @brief Test functions to turn on and off the display
 */
void display_on();
void display_off();

/**
 * @brief Function to convert numbers for correct display representation
 * @number number which will be represented on the display
 */
uint8_t display_segment(uint8_t number);

/**
 * @brief Function to manipulate the display
 */
void display_handle();

/**
 * @brief Functions to handle the array for the display
 * @array[4] Array which holds the Data shown on the 7-Segment-Display
 * @num The location in the array where the changes will be done
 */
void reset_display(uint8_t array[4]);
void incr_display(uint8_t array[4], int num);
void decr_display(uint8_t array[4], int num);

/**
* @brief Funtction to show an Error when 2 people want to enter simultaneously
* @array[4] Array which holds the Data shown on the 7-Segment-Display
*/

void cnt_error(uint8_t array[4]);
