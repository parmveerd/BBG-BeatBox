
#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdatomic.h>

void text_init(int*);
void* display_text();
void text_join();
void text_cleanup();

#endif