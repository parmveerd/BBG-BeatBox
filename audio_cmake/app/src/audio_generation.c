
#include "audio_generation.h"
#include "audioMixer_template.h"
#include "hal/joystick.h"

#define DRUM_BASE_HARD "/mnt/remote/myApps/beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define DRUM_HI_HAT "/mnt/remote/myApps/beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define DRUM_SNARE "/mnt/remote/myApps/beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define ACC_DRUM_CYN "/mnt/remote/myApps/beatbox-wave-files/100056__menegass__gui-drum-cyn-hard.wav"
#define ACC_DRUM_TOM_HARD "/mnt/remote/myApps/beatbox-wave-files/100066__menegass__gui-drum-tom-mid-hard.wav"
#define ACC_DRUM_SNARE "/mnt/remote/myApps/beatbox-wave-files/100058__menegass__gui-drum-snare-hard.wav"

#define MAX_VOLUME 100
#define MIN_VOLUME 0
#define MAX_BPM 300
#define MIN_BPM 40

// Initial values
static int tempo = 120;
static int vol = 80;

// Hold drum beats and accelerometers beats
static wavedata_t drumBeats[3];
static wavedata_t accBeats[3];

// Initialize our six files
void files_init() {
    AudioMixer_readWaveFileIntoMemory(DRUM_BASE_HARD, &drumBeats[0]);
    AudioMixer_readWaveFileIntoMemory(DRUM_HI_HAT, &drumBeats[1]);
    AudioMixer_readWaveFileIntoMemory(DRUM_SNARE, &drumBeats[2]);
    AudioMixer_readWaveFileIntoMemory(ACC_DRUM_CYN, &accBeats[0]);
    AudioMixer_readWaveFileIntoMemory(ACC_DRUM_TOM_HARD, &accBeats[1]);
    AudioMixer_readWaveFileIntoMemory(ACC_DRUM_SNARE, &accBeats[2]);
}

// Free our six files
void files_free() {
    AudioMixer_freeWaveFileData(&drumBeats[0]);
    AudioMixer_freeWaveFileData(&drumBeats[1]);
    AudioMixer_freeWaveFileData(&drumBeats[2]);
    AudioMixer_freeWaveFileData(&accBeats[0]);
    AudioMixer_freeWaveFileData(&accBeats[1]);
    AudioMixer_freeWaveFileData(&accBeats[2]);
}

// Update our volume variable
void set_volume() {
    vol = AudioMixer_getVolume();
}

// Return volume function for UDP
int get_volume() {
    return vol;
}

// Return tempo function for UDP
int get_tempo() {
    return tempo;
}

// Standard rock beat based on the 8 half counts and checks for tempo change mid-beat
void standard_rock() {
    float temp;

    // count 1: hh(1), base(0)
    AudioMixer_queueSound(&drumBeats[1]);
    AudioMixer_queueSound(&drumBeats[0]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 1.5 hh(1)
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 2 hh(1), snare(2)
    AudioMixer_queueSound(&drumBeats[1]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 2.5 hh(1)
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 3 hh(1), base(0)
    AudioMixer_queueSound(&drumBeats[1]);
    AudioMixer_queueSound(&drumBeats[0]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 3.5 hh(1)
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 4 hh(1), snare(2)
    AudioMixer_queueSound(&drumBeats[1]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 4.5 hh(1)
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);
}

// Custom beat
void custom_rock() {
    float temp;

    // count 1: base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 1.5 base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 2 base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 2.5 base, hh
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 3 base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 3.5 base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 4 base, snare
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[2]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);

    // count 4.5 base, hh
    AudioMixer_queueSound(&drumBeats[0]);
    AudioMixer_queueSound(&drumBeats[1]);
    temp = ((60.0/tempo) / 2.0) * 1000.0;
    sleepForMs((int)temp);
}

// Play drum function
void play_drum(int x) {
    if (x >= 0 && x <= 2) {
        AudioMixer_queueSound(&drumBeats[x]);
    }
}

// Play accelerometer drum
void play_accdrum(int x) {
    if (x >= 0 && x <= 2) {
        AudioMixer_queueSound(&accBeats[x]);
    }
}

// Increase volume by 5 function
void vol_incr() {
    vol = AudioMixer_getVolume();

    if (vol >= 95) {
        vol = MAX_VOLUME;
    } else {
        vol = vol + 5;
    }

    AudioMixer_setVolume(vol);
}

// Decrease volume by 5 function
void vol_decr() {
    vol = AudioMixer_getVolume();

    if (vol <= 5) {
        vol = MIN_VOLUME;
    } else {
        vol = vol - 5;
    }

    AudioMixer_setVolume(vol);
}

// Increase tempo by 5 function
void tempo_incr() {
    if (tempo >= MAX_BPM - 5) {
        tempo = MAX_BPM;
    } else {
        tempo = tempo + 5;
    }
}

// Decrease tempo by 5 function
void tempo_decr() {
    if (tempo <= MIN_BPM + 5) {
        tempo = MIN_BPM;
    } else {
        tempo = tempo - 5;
    }
}

