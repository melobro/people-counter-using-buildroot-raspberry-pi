//NEW 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_ROOT "/sys/class/gpio"
#define EXPORT "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/unexport"

#define IN "in"
#define OUT "out"

#define LOW 0
#define HIGH 1

int export_pin(int num);
int unexport_pin(int num);
int set_direction(char *mode, int num);
int set_value(int val, int num);
int get_value(int num); // Add this line to declare the function
