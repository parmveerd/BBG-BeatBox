
#include "udp_server.h"
#include "beat.h"
#include "audio_generation.h"
#include "periodTimer.h"

#define PORT 12345
#define BUFFER_SIZE 1024

static pthread_t udpThread;
static int *continueFlag4;

static int socketDescriptor;
static char buff[BUFFER_SIZE];

struct sysinfo info;
static int start_time;

// Function called by main to initialize our time and create a thread
int udp_init(int* flag) {
    continueFlag4 = flag;

    // Store the time once program has started so we can calculate how long its been running for
    sysinfo(&info);
    start_time = info.uptime;

    if (rec_init() < 0) {
        perror("Error creating the UDP socket!\n");
        return -1;
    }

    pthread_create(&udpThread, NULL, udp_check, NULL);

    return 0;
}

// Main thread loop function to look for inputs from the website
void* udp_check() {
    struct sockaddr_in sinClient;
    unsigned int sin_len = sizeof(sinClient);

    while (*continueFlag4) {
        int bytes_rec = recvfrom(socketDescriptor, buff, BUFFER_SIZE - 1, 0, (struct sockaddr *)&sinClient, &sin_len); // Receive 

        // Error check to make sure size is at least 1
        if (bytes_rec < 0) {
            perror("Error with UDP socket receiving message.\n");
            fflush(stdout);
            continue;
        }

        buff[bytes_rec] = '\0';

        // Check for which value it is
        if (strcmp(buff, "none") == 0) {
            if (get_mode() != 1) {
                change_mode(1);
            }
        } else if (strcmp(buff, "rock1") == 0) {
            if (get_mode() != 2) {
                change_mode(2);
            }
        } else if (strcmp(buff, "rock2") == 0) {
            if (get_mode() != 3) {
                change_mode(3);
            }
        } else if (strcmp(buff, "vol_decr") == 0) {
            vol_decr();
        } else if (strcmp(buff, "vol_incr") == 0) {
            vol_incr();
        } else if (strcmp(buff, "tempo_decr") == 0) {
            tempo_decr();
        } else if (strcmp(buff, "tempo_incr") == 0) {
            tempo_incr();
        } else if (strcmp(buff, "hihat") == 0) {
            play_drum(1);
        } else if (strcmp(buff, "snare") == 0) {
            play_drum(2);
        } else if (strcmp(buff, "base") == 0) {
            play_drum(0);
        } else if (strcmp(buff, "update") == 0) {
            // Get updated string, put inside buffer and free temp
            char* temp = update_time();
            memset(buff, 0, BUFFER_SIZE);
            strcpy(buff, temp);
            free(temp); // Free malloc
        } else if (strcmp(buff, "stop") == 0) {
            *continueFlag4 = 0; // Stop all threads in our program so program can be terminated
        }

        // Send back string
        sendto(socketDescriptor, buff, strlen(buff), 0, (struct sockaddr *)&sinClient, sin_len);
    }

    close(socketDescriptor);

    return NULL;
}

// Update time, volume, tempo so we can put it into one string
char* update_time() {
    // Get updated values for tempo and volume
    int t = get_tempo();
    int v = get_volume();
    char temp[64];

    sysinfo(&info); // Get updated time

    int curr_time = info.uptime - start_time; // Calculate how long program has been running for

    // Find time in hours, mins and secs
    int hour = (curr_time / 3600);
    int min = (curr_time - (3600*hour)) / 60;
    int sec = (curr_time - (3600*hour) - (60*min));

    // Put all our values into one string so we can split using space in js code
    memset(temp, 0, sizeof(char)*64);
    snprintf(temp, 64, "values %d %d %d %d:%d:%d", get_mode(), v, t, hour, min, sec);

    // Need to malloc in order to send back
    char* result = malloc(strlen(temp) + 1);
    if (result != NULL) {
        strcpy(result, temp);
    }

    return result;
}

// Initialize receiver (same function used in my a2)
int rec_init() {
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    if (socketDescriptor == -1) {
        close(socketDescriptor);
        perror("Error with initializing socket.\n");
        return -1;
    }

    if (bind(socketDescriptor, (struct sockaddr *)&sin, sizeof(sin))) {
        close(socketDescriptor);
        perror("Error with socket.\n");
        return -1;
    }

    return 0;
}

// Join thread
void udp_cleanup() {
    pthread_join(udpThread, NULL);
}
