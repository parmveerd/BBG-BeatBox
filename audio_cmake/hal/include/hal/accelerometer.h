#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

void init_acc();
int get_desc();
int initI2cBus(char*, int);
void writeI2cReg(int, unsigned char, unsigned char);
unsigned char readI2cReg(int, unsigned char);
void runCommand(char*);


#endif