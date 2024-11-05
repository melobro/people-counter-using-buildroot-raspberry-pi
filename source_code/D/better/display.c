#include "display.h"

// Function to activate GPIO pins (CLK and DIO) for communication with the display
void activate_gpio_pin()
{
    // CLK PIN
    export_pin(CLK);        // Export CLK pin
    set_direction(OUT, CLK); // Set CLK pin direction as output
    set_value(LOW, CLK);     // Set CLK pin initial value to LOW

    // DIO PIN
    export_pin(DIO);        // Export DIO pin
    set_direction(OUT, DIO); // Set DIO pin direction as output
    set_value(LOW, DIO);     // Set DIO pin initial value to LOW
}

// Function to deactivate GPIO pins after communication with the display
void deactivate_gpio_pin()
{
    // CLK PIN
    set_value(LOW, CLK);    // Set CLK pin value to LOW
    set_direction(IN, CLK); // Set CLK pin direction as input
    unexport_pin(CLK);      // Unexport CLK pin

    // DIO PIN
    set_value(LOW, DIO);    // Set DIO pin value to LOW
    set_direction(IN, DIO); // Set DIO pin direction as input
    unexport_pin(DIO);      // Unexport DIO pin
}

// Functions to manipulate CLK and DIO pins for communication
void display_clk_high() { set_value(HIGH, CLK); }
void display_clk_low() { set_value(LOW, CLK); }
void display_dio_high() { set_value(HIGH, DIO); }
void display_dio_low() { set_value(LOW, DIO); }

// Function to start a communication command
void display_start_cmd()
{
    display_clk_high();   // Set CLK to HIGH
    display_dio_high();   // Set DIO to HIGH
    display_dio_low();    // Set DIO to LOW
    display_clk_low();    // Set CLK to LOW
}

// Function to end a communication command
void display_end_cmd()
{
    display_clk_low();    // Set CLK to LOW
    display_dio_low();    // Set DIO to LOW
    display_clk_high();   // Set CLK to HIGH
    display_dio_high();   // Set DIO to HIGH
}

// Function to read a bit and transmit it over CLK and DIO pins
void read_bit(int *tx_data)
{
    for (uint8_t j = 0; j < 0x08; j++) // Iterate through each bit
    {
        display_clk_low(); // Set CLK to LOW
        if (tx_data[j] == 1)
        {
            display_dio_high(); // Set DIO to HIGH if the bit is 1
        }
        else
        {
            display_dio_low(); // Set DIO to LOW if the bit is 0
        }
        display_clk_high(); // Set CLK to HIGH
    }
}

// Function to transmit a command to the display
void transmit_command(uint8_t *tx_data)
{
    int tx_byte[8] = {0}; // Array to store bits of the byte

    for (uint8_t i = 0; i < 0x08; i++) // Iterate through each bit of the byte
    {
        tx_byte[i] = (tx_data[0] & (0x01 << i)) && 1; // Extract individual bits
    }

    display_start_cmd(); // Start the communication command
    read_bit(tx_byte);   // Transmit the bits over CLK and DIO
    display_clk_low();   // Set CLK to LOW
    display_clk_high();  // Set CLK to HIGH

    if (tx_data[0] == 0x40 || tx_data[0] == 0x80 || tx_data[0] == 0x88)
    {
        display_end_cmd(); // End the communication command if specific conditions are met
    }
}

// Function to transmit data to the display
void transmit_data(uint8_t *tx_data, uint8_t data_length)
{
    int tx_byte[8] = {0}; // Array to store bits of the byte

    for (uint8_t i = 0; i < data_length; i++) // Iterate through each byte of data
    {
        for (int j = 0; j < 8; j++) // Iterate through each bit of the byte
        {
            tx_byte[j] = (tx_data[i] & (0x01 << j)) && 1; // Extract individual bits
        }

        read_bit(tx_byte);   // Transmit the bits over CLK and DIO
        display_clk_low();   // Set CLK to LOW
        display_clk_high();  // Set CLK to HIGH
    }
    display_end_cmd(); // End the communication command after transmitting all data
}

// Function to turn on the display
void display_on()
{
    uint8_t buffer[4] = {0}; // Buffer for data
    uint8_t command[1] = {0}; // Buffer for command

    activate_gpio_pin(); // Activate GPIO pins for communication

    command[0] = Command_1;
    transmit_command(command); // Transmit Command 1
    command[0] = Command_2;
    transmit_command(command); // Transmit Command 2
    transmit_data(buffer, 4);   // Transmit buffer data
    command[0] = Command_3_on;
    transmit_command(command); // Transmit Command 3 (Turn on display)
}

// Function to turn off the display
void display_off()
{
    uint8_t buffer[4] = {0}; // Buffer for data
    uint8_t command[1] = {0}; // Buffer for command

    command[0] = Command_1;
    transmit_command(command); // Transmit Command 1
    command[0] = Command_2;
    transmit_command(command); // Transmit Command 2
    transmit_data(buffer, 4);   // Transmit buffer data
    command[0] = Command_3_off;
    transmit_command(command); // Transmit Command 3 (Turn off display)

    deactivate_gpio_pin(); // Deactivate GPIO pins after communication
}

// Function to handle the display based on an array of 4 values
void display_handle(uint8_t array[4])
{
    uint8_t local_arr[4] = {0}; // Local array for processed values
    uint8_t command[1] = {0};   // Buffer for command
    uint8_t on = 0x88;           // Command to turn on display

    local_arr[3] = display_segment(array[3]); // Process and store values
    if (array[2] > 0 || array[1] > 0)
    {
        local_arr[2] = display_segment(array[2]);
    }
    if (array[1] > 0 || array[0] > 0)
    {
        local_arr[1] = display_segment(array[1]);
    }
    if (array[0] > 0)
    {
        local_arr[0] = display_segment(array[0]);
    }

    command[0] = Command_1;
    transmit_command(command); // Transmit Command 1
    command[0] = Command_2;
    transmit_command(command); // Transmit Command 2
    transmit_data(local_arr, 4); // Transmit processed data
    transmit_command(&on); // Transmit Command to turn on display
}



// Function to convert numbers to their display segments

uint8_t display_segment(uint8_t data)
{
    switch (data)
    {
    case 0x00:
            return zero;
            break;
    case 0x01:
            return one;
            break;
    case 0x02:
            return two;
            break;
    case 0x03:
            return three;
            break;
    case 0x04:
            return four;
            break;
    case 0x05:
            return five;
            break;
    case 0x06:
            return six;
            break;
    case 0x07:
            return seven;
            break;
    case 0x08:
            return eight;
            break;
    case 0x09:
            return nine;
            break;
    case 0x79:
            return E;
            break;
    case 0x50:
            return r;
            break;
    }
    return zero; // Default to zero if the input is not recognized
}

// Function to reset the display values to zero
void reset_display(uint8_t array[4])
{
    for(int i=0;i<3;++i)
    {
        array[i]=0; // Set each element in the array to zero
    }
}

// Function to increment the display value at a specific position
void incr_display(uint8_t array[4],int num)
{
    
    if(num>3 || num<0)
    {
        return;
    }

    //check if highest possible number is shown on display
    if(array[0]>=9 && array[1]>=9  && array[2]>=9 && array[3]>=9)
    {
        reset_display(array);
        return;
    }

    //increment number shown on display
    if(array[num]<9 && array[num]>=0)
    {
        array[num]++;
    }
    else if(array[num]==9)
    {
        array[num]=0;
        incr_display(array,num-1);
    }
    return;
}

// Function to decrement the display value at a specific position
void decr_display(uint8_t array[4],int num)
{
    
    if(num>3 || num<0)
    {
        return;
    }

    //check if lowest possible number or highest possible number is shown on display
    if((array[0]==0 && array[1]==0  && array[2]==0 && array[3]==0))
    {   
        array[0]=E;
        array[1]=r;
        array[2]=r;
        array[3]=1;

        return;
    }

    //decrement number shown on display
    if(array[num]==0)
    {
        array[num]=9;
        decr_display(array,num-1);
    }
    else
    {
        array[num]--;
    }
    
    return;
}

//Function to show an Error if more than 2 people want to enter simultaneously
void cnt_error(uint8_t array[4])
{

	array[0]=E;
	array[1]=r;
	array[2]=r;
	array[3]=2;
	return;
}
