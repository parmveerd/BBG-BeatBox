#include <stdio.h>
#include "hal/joystick.h"
#include "hal/accelerometer.h"
#include "audioMixer_template.h"
#include "beat.h"
#include "audio_generation.h"
#include "acc_drums.h"
#include "text_display.h"
#include "periodTimer.h"
#include "udp_server.h"

static int flag = 1;

int main() {

	// Initialize period, audiomixer and accelerometer
	Period_init();
	AudioMixer_init();
	init_acc();

	// Call initialization functions for the functions that will have threads
	beat_start(&flag);
	acc_drums_init(&flag);
	text_init(&flag);
	udp_init(&flag);

	// Call clean up functions for our functions to join threads and free memory
	beat_cleanup();
	acc_drums_cleanup();
	text_cleanup();
	udp_cleanup();

	return 0;
}