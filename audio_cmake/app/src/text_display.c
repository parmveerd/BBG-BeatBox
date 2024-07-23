
#include "text_display.h"
#include "periodTimer.h"
#include "audioMixer_template.h"
#include "hal/joystick.h"
#include "beat.h"
#include "audio_generation.h"

static int *continueFlag3;
static pthread_t displayThread;

static double buffer_min;
static double buffer_max;
static double buffer_avg;
static long long buffer_num;
static Period_statistics_t buffer_stats;

static double acc_min;
static double acc_max;
static double acc_avg;
static long long acc_num;
static Period_statistics_t acc_stats;


// Initialize thread
void text_init(int* flag) {
    continueFlag3 = flag;

    pthread_create(&displayThread, NULL, display_text, NULL);
}

// Main thread to print every second
void* display_text() {
    while (*continueFlag3) {
        // Get stats for audio
        Period_getStatisticsAndClear(PERIOD_EVENT_BUFFER, &buffer_stats);
        buffer_min = buffer_stats.minPeriodInMs;
        buffer_max = buffer_stats.maxPeriodInMs;
        buffer_avg = buffer_stats.avgPeriodInMs;
        buffer_num = buffer_stats.numSamples;

        // Get stats for accelerometer
        Period_getStatisticsAndClear(PERIOD_EVENT_ACC, &acc_stats);
        acc_min = acc_stats.minPeriodInMs;
        acc_max = acc_stats.maxPeriodInMs;
        acc_avg = acc_stats.avgPeriodInMs;
        acc_num = acc_stats.numSamples;
    
        // Print all values to the terminal
        printf("M%d %dbpm vol:%d   Audio[%.3f, %.3f] avg %.3f/%lld  Accel[%.3f, %.3f] avg %.3f/%lld\n", get_mode(), get_tempo(), get_volume(), buffer_min, buffer_max, buffer_avg, buffer_num, acc_min, acc_max, acc_avg, acc_num);

        sleepForMs(1000); // Sleep for 1 second
    }
    
    return NULL;
}


// Join thread
void text_join() {
    pthread_join(displayThread, NULL);
}

// Call thread and cleanup
void text_cleanup() {
    text_join();
    Period_cleanup();
}



