
#include "acc_drums.h"
#include "hal/accelerometer.h"
#include "hal/joystick.h"
#include "audio_generation.h"
#include "periodTimer.h"

#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

static int *continueFlag2;

static int val;

static unsigned char xl;
static unsigned char xh;
static unsigned char yl;
static unsigned char yh;
static unsigned char zl;
static unsigned char zh;

static pthread_t threadX;
static pthread_t threadY;
static pthread_t threadZ;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize threads for our three directions
void acc_drums_init(int* flag) {
    continueFlag2 = flag;

    val = get_desc();

    if (pthread_create(&threadX, NULL, drum_x, NULL) != 0){
        perror("Error creating direction thread\n");
    }
    if (pthread_create(&threadY, NULL, drum_y, NULL) != 0){
        perror("Error creating beat thread\n");
    }
    if (pthread_create(&threadZ, NULL, drum_z, NULL) != 0){
        perror("Error creating volume/tempo thread\n");
    }
}

// Thread for x axis
void* drum_x() {
    int16_t x;
    float value;
    while (*continueFlag2) {
        Period_markEvent(PERIOD_EVENT_ACC); // Keep track of the time for our print to the terminal

        xl = readI2cReg(val, OUT_X_L);
        xh = readI2cReg(val, OUT_X_H);
        // Caluclate x using this formula and turn it into a float
        x = (xh << 8) | (xl);
        value = (float)x / 16000;

        // Value of 2 worked best here (anything greater would not play drums)
        if (value < -2 || value > 2) {
            pthread_mutex_lock(&mutex);
            {
                play_accdrum(0);
            }
            pthread_mutex_unlock(&mutex);
        }
        sleepForMs(25);
    }

    return NULL;
}

// Thread for y axis
void* drum_y() {
    int16_t y;
    float num;
    while (*continueFlag2) {
        yl = readI2cReg(val, OUT_Y_L);
        yh = readI2cReg(val, OUT_Y_H);
        y = (yh << 8) | (yl);
        num = (float)y / 16000;

        if (num < -2 || num > 2) {
            pthread_mutex_lock(&mutex);
            {
                play_accdrum(1);
            }
            pthread_mutex_unlock(&mutex);
        }
        sleepForMs(30);
    }

    return NULL;
}

// Thread for z axis
void* drum_z() {
    int16_t z;
    float temp;
    while (*continueFlag2) {
        zl = readI2cReg(val, OUT_Z_L);
        zh = readI2cReg(val, OUT_Z_H);
        z = (zh << 8) | (zl);
        temp = (float)z / 16000;

        if (temp < -2 || temp > 2) {
            pthread_mutex_lock(&mutex);
            {
                play_accdrum(2);
            }
            pthread_mutex_unlock(&mutex);
        }
        sleepForMs(25);
    }

    return NULL;
}

// Join threads
void acc_drums_join() {
    pthread_join(threadX, NULL);
    pthread_join(threadY, NULL);
    pthread_join(threadZ, NULL);
}

// Call join threads and destroy our lock
void acc_drums_cleanup() {
    acc_drums_join();
    pthread_mutex_destroy(&mutex);
}