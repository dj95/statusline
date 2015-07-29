#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <iwlib.h>

#include "config.h"

int main();
char* get_date();
int get_battery(char*);
int get_backlight(char*, char*);
int get_network(char*);
int get_audio(char*);
int read_value(char*, char*);
