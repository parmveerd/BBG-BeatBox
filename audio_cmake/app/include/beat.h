#ifndef BEAT_H
#define BEAT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdatomic.h>

void beat_start(int*);
void* checkDirection();
void* change_beat();
void* volume_tempo();
int get_mode();
void change_mode(int);
void beat_join();
void beat_cleanup();


#endif