#include "display.h"

/**
 * @brief Initial array for the display.
 */
uint8_t display_array[4] = {0, 0, 0, 0};

/**
 * @brief Main function to test the 7-Segment Display.
 * 
 * @return 0 on successful program execution.
 */

int main() {
    // Print a message to indicate the start of the program
    printf("Tests for the 7-Segment Display\n");

    // Pause execution for 1 second
    sleep(1);

    // Turn on the display
    display_on();

    // Pause execution for 1 second
    sleep(1);

    // Display the initial array on the display
    display_handle(display_array);

    // Pause execution for 1 second
    sleep(1);


    // Perform a sequence of increments and display updates
    printf("Increment Display Counter\n");
    int cnt = 1;
    while (cnt <= 5) {
        // Decrement the value in the third position of the array
        incr_display(display_array, 3);

        // Display the updated array on the display
        display_handle(display_array);

        // Pause execution for 1 second
        sleep(1);

        // Increment the counter
        cnt++;
    }


    //Test for decrementing the Display Counter
    printf("Decrement Display Counter\n");
    cnt=1;
        while (cnt <= 6) {
        // Decrement the value in the third position of the array
        decr_display(display_array, 3);

        // Display the updated array on the display
        display_handle(display_array);

        // Pause execution for 1 second
        sleep(1);

        // Increment the counter
        cnt++;
    }

    // Turn off the display
    display_off();

    // Return 0 to indicate successful program execution
    return 0;
}

