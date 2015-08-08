#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <iwlib.h>
#include <fcntl.h>

#ifdef OSSCONTROL
#define MIXER_DEV "/dev/dsp"

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <stdio.h>
#else
#include <alsa/asoundlib.h>
#endif

#include "config.h"

int main();
int get_workspace(char*);
int get_audio(char*);
int get_network(char*);
int get_backlight(char*, char*);
int get_battery(char*);
int get_date(char*);
int read_value(char*, char*);
int file_exists(char*);
