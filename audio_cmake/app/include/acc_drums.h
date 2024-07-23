
#ifndef ACC_DRUMS_H
#define ACC_DRUMS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdatomic.h>

void acc_drums_init(int*);
void* drum_x();
void* drum_y();
void* drum_z();
void acc_drums_join();
void acc_drums_cleanup();


#endif