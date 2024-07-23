#ifndef AUDIO_GENERATION_H
#define AUDIO_GENERATION_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

void files_init();
void files_free();
void set_volume();
int get_volume();
int get_tempo();
void standard_rock();
void custom_rock();
void play_drum(int);
void play_accdrum(int);
void vol_incr();
void vol_decr();
void tempo_incr();
void tempo_decr();

#endif