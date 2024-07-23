#include "hal/joystick.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


#define JOYSTICK_UP "/sys/class/gpio/gpio26/value"
#define JOYSTICK_RIGHT "/sys/class/gpio/gpio47/value"
#define JOYSTICK_DOWN "/sys/class/gpio/gpio46/value"
#define JOYSTICK_LEFT "/sys/class/gpio/gpio65/value"
#define JOYSTICK_CENTRE "/sys/class/gpio/gpio27/value"

// Sleep function from class code
void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

// Export gpio
void init_gpio() {
    int nums[] = {26, 47, 46, 65, 27};
    
    for (int i = 0; i < 5; i++) {
        export_gpio(nums[i]);
        sleepForMs(200);
    }
}

// Function to make sure all directions of the joystick are configured (function from my a1)
void export_gpio(int num) {
    char buffer[50];
    sprintf(buffer, "/sys/class/gpio/gpio%d", num);

    struct stat st;
    if (stat(buffer, &st) == -1) {
        FILE *fp = fopen("/sys/class/gpio/export", "w");
        
        if (fp == NULL) {
            perror("Error with opening export files.\n");
            exit(EXIT_FAILURE);
        }

        fprintf(fp, "%d", num);
        fclose(fp);
    }
}

// Find direction of the joystick
int direction_gpio() {
    char temp = readFile(JOYSTICK_UP);
    if (temp == '0') {
        return UP;
    }

    temp = readFile(JOYSTICK_RIGHT);
    if (temp == '0') {
        return RIGHT;
    }

    temp = readFile(JOYSTICK_DOWN);
    if (temp == '0') {
        return DOWN;
    }

    temp = readFile(JOYSTICK_LEFT);
    if (temp == '0') {
        return LEFT;
    }

    temp = readFile(JOYSTICK_CENTRE);
    if (temp == '0') {
        return CENTRE;
    }

    return NONE;
}

// Helper function to read files
char readFile(char* file) {
    char buff[3];
    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        printf("Cannot open file.\n");
    }

    fgets(buff, 3, fp);
    fclose(fp);

    return buff[0];
}