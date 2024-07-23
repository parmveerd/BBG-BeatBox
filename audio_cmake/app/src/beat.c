
#include "beat.h"
#include "hal/joystick.h"
#include "audio_generation.h"
#include "audioMixer_template.h"

static int joystick_dir = NONE;
static int *continueFlag;
static int mode = 1;

static pthread_t pthread_direction;
static pthread_t pthread_mode;
static pthread_t pthread_vol;

// Initialize function to create threads and initialize files/joystick/volume
void beat_start(int *flag) {
    continueFlag = flag; // Thread loop condition
    
    init_gpio(); // Initialize joystick
    files_init(); // Initialize audio files
    set_volume(); // Set volume

    // Create threads here
    if (pthread_create(&pthread_direction, NULL, checkDirection, NULL) != 0){
        perror("Error creating direction thread\n");
    }
    if (pthread_create(&pthread_mode, NULL, change_beat, NULL) != 0){
        perror("Error creating beat thread\n");
    }
    if (pthread_create(&pthread_vol, NULL, volume_tempo, NULL) != 0){
        perror("Error creating volume/tempo thread\n");
    }
}

// Thread will keep checking direction of joystick and update our variable
void* checkDirection() {
    while (*continueFlag) {
        joystick_dir = direction_gpio();
    }

    return NULL;
}

// Thread will keep playing the correct beat so it does not ever stop
void* change_beat() {
    while (*continueFlag) {
        if (mode == 2) {
            // standard rock beat
            standard_rock();
        } else if (mode == 3) {
            // custom rock beat
            custom_rock();
        }
    }

    return NULL;
}

// Main thread that will keep changing mode/volume/tempo based on the direction of the joystick
// Not doing get direction in this thread since we sleep for 0.4s, so doing it in a different thread
// will allow us to get more values and pick up if a direction was selected mid-iteration or during sleep
// (sometimes input would not get selected if I did just one get direction call at the start of each iteration)
void* volume_tempo() {
    int check = 0;
    while (*continueFlag) {
        if (joystick_dir == CENTRE) {
            // Change mode variable for our change beat thread
            if (mode >= 3) {
                mode = 1;
            } else {
                mode++;
            }
        } else if (joystick_dir == UP) {
            // Increase volume by 5
            vol_incr();
        } else if (joystick_dir == DOWN) {
            // Decrease volume by 5
            vol_decr();
        } else if (joystick_dir == RIGHT) {
            // Increase tempo by 5 bpm
            tempo_incr();
        } else if (joystick_dir == LEFT) {
            // Decrease tempo by 5 bpm
            tempo_decr();
        } else {
            check = 1; // No input
        }

        if (check == 0) {
            sleepForMs(400); // Give user some time to let go of the joystick
        } else {
            check = 0; // No sleep if no input
        }
    }

    return NULL;
}

// Get mode for UDP
int get_mode() {
    return mode;
}

// Change mode for UDP
void change_mode(int m) {
    if (m >= 1 && m <= 3) {
        mode = m;
    }
}

// Join threads
void beat_join() {
    pthread_join(pthread_direction, NULL);
    pthread_join(pthread_mode, NULL);
    pthread_join(pthread_vol, NULL);
}

// Call join threads, free files and clean audiomixer
void beat_cleanup() {
    beat_join();
    files_free();
    AudioMixer_cleanup();
}