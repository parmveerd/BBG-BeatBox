
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

enum direction{NONE, UP, RIGHT, DOWN, LEFT, CENTRE};

void sleepForMs(long long);
void init_gpio();
void export_gpio(int);
int direction_gpio();
char readFile(char*);

#endif