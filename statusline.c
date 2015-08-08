/*
 * statusline for lemonbar
 *
 * © 2015 Daniel Jankowski
 * Licensed under the MIT/X Consortium License
 **/
#include "statusline.h"

char* ARROW_LEFT = "";
char* ARROW_RIGHT = "";
char* SEP_LEFT = "";
char* SEP_RIGHT = "";


int main() {
    savepoint("==========================");

    char buf_bat[200] = {0};
    char buf_back[200] = {0};
    char buf_net[200] = {0};
    char buf_audio[200] = {0};
    char buf_date[200] = {0};
    char buf_ws[300] = {0};
    savepoint("allocation");

    get_workspace(buf_ws);
    savepoint("workspace");

    get_battery(buf_bat);
    savepoint("battery");

    get_audio(buf_audio);
    savepoint("audio");

    get_date(buf_date);
    savepoint("date");

    if (get_network(buf_net) == 1) {
        get_backlight(buf_back, COLOR_BG_NET);
        savepoint("backlight");

        printf("%%{l}%s %%{r}%s %s %s %s %s%%{F%s}%%{B%s}\n", buf_ws, buf_audio, buf_net, buf_back, buf_bat, buf_date, COLOR_BG, COLOR_BG);
    } else {
        get_backlight(buf_back, COLOR_BG_AUDIO);
        savepoint("backlight");

        printf("%%{F%s}%%{B%s}%%{l}%s%%{F%s}%%{B%s} %%{r}%s %s %s %s%%{F%s}%%{B%s}\n",COLOR_BG, COLOR_BG,  buf_ws, COLOR_BG, COLOR_BG, buf_audio, buf_back, buf_bat, buf_date, COLOR_BG, COLOR_BG);
    }
    savepoint("print");
    savepoint("==========================");
    return 0;
}


int get_workspace(char* buf) {
    FILE* cmd;
    char workspaces[32][20] = {0};
    char ptr[200];
    char active_ws[6] = {0};
    char username[33] = {0};
    char hostname[32] = {0};
    char title[64] = {0};
    int count = 0;

    cmd = popen("i3-msg -t get_workspaces | jshon -a -e name -u -p -e focused -u  | sed 's/[0-9]*://g'", "r");
    while (fgets(ptr, 32, cmd) != 0) {
        strncpy(workspaces[count], strtok(ptr, "\n"), 20);
        count = count + 1;
    }
    pclose(cmd);

    cmd = popen("whoami", "r");
    if (fgets(ptr, 32, cmd) != 0) {
        strncpy(username, strtok(ptr, "\n"), 32);
    }
    pclose(cmd);

    read_value("/etc/hostname", hostname);

    cmd = popen("xtitle", "r");
    if (fgets(ptr, 60, cmd) != 0) {
        strncpy(title, ptr, 60);
    }
    pclose(cmd);

    for (int i=0; i < 32; i++) {
        if (workspaces[i][0] == 't') {
            strncpy(active_ws, workspaces[i-1], 5);
        }
    }

    if (strtok(title, "\n") == 0) {
        sprintf(buf, "%%{F%s}%%{B%s} %s@%s %%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %%{F%s}%%{B%s}%s%%{F%s}%%{B%s}", COLOR_FG_HOST, COLOR_BG_HOST, username, hostname, COLOR_BG_HOST, COLOR_BG_WS, SEP_RIGHT, COLOR_FG_WS, COLOR_BG_WS, active_ws, COLOR_BG_WS, COLOR_BG, SEP_RIGHT, COLOR_BG, COLOR_BG);
    } else {
        sprintf(buf, "%%{F%s}%%{B%s} %s@%s %%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %%{F%s}%%{B%s}%s", COLOR_FG_HOST, COLOR_BG_HOST, username, hostname, COLOR_BG_HOST, COLOR_BG_WS, SEP_RIGHT, COLOR_FG_WS, COLOR_BG_WS, active_ws, COLOR_BG_WS, COLOR_BG_TITLE, SEP_RIGHT, COLOR_FG_TITLE, COLOR_BG_TITLE, title, COLOR_BG_TITLE, COLOR_BG, SEP_RIGHT);
    }
    return 1;
}


int get_audio(char* buf) {
    FILE* cmd = {0};
    char vol[64] = {0};
    char percent[3] = "65";
    char stat[3] = {0};
    char* status_icon;
    char info[64] = {0};

    cmd = popen("amixer get Master | grep 'Front Left:' | sed -r 's/\\s*Front Left: Playback [0-9]* \\[//' | sed -r 's/\\] \\[.*//' | sed -r 's/\\s*//g'", "r");
    while (fgets(vol, 10, cmd) != 0) {
        strncpy(percent, vol, 3);
    }
    pclose(cmd);

    cmd = popen("amixer get Master | grep 'Front Left:' | sed -r 's/\\s*Front Left: Playback [0-9]* \\[[0-9]*%\\] \\[//' | sed -r 's/\\]\\s*//'", "r");
    while (fgets(vol, 10, cmd) != 0) {
        strncpy(stat, vol, 3);
    }
    pclose(cmd);

    //cmd = popen("mpc status | perl -ne 'if (/\\[playing\\]/) {CORE::say (\"(\", `mpc current|tr -d \"\\n\"`, \")\")}'", "r");
    if(DEBUG) {
        cmd = popen("mpc current", "r");
    } else {
        cmd = popen("mpc current 2>/dev/null", "r");
    }

    while (fgets(vol, 40, cmd) != 0) {
        strncpy(info, vol, 40);
    }
    pclose(cmd);


    if (stat[1] == 'n') {
        status_icon = "";
    } else {
        status_icon = "";
    }


    if ((info[0] == 'm' && info[1] == 'p' && info[2] == 'd') || (info[0] == 0)) {
        sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %s% ", COLOR_BG_AUDIO, COLOR_BG, SEP_LEFT, COLOR_FG_AUDIO, COLOR_BG_AUDIO, status_icon, percent);
    } else {
        sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %s (%s)", COLOR_BG_AUDIO, COLOR_BG, SEP_LEFT, COLOR_FG_AUDIO, COLOR_BG_AUDIO, status_icon, percent, strtok(info, "\n"));
    }

    return 1;
}


int get_network(char* buf) {
    char ip_wifi[32] = {0};
    char ip_eth[32] = {0};
    FILE* cmd = {0};
    char bf[64] = {0};
    char status[200] = {0};
    char essid[100] = {0};
    int on = 1;

    if(!strcmp(DEVICE_WLAN, "")) {
        char cmd_wlan[75 + 10];
        sprintf(cmd_wlan, "ip -f inet -o addr show %s|cut -d\\  -f 7 | cut -d/ -f 1 | sed -re 's/\\n//g'", DEVICE_WLAN);
        cmd = popen(cmd_wlan, "r");
        while (fgets(bf, 30, cmd) != 0) {
            strncpy(ip_wifi, bf, 30);
        }
        pclose(cmd);
    }

    if(!strcmp(DEVICE_LAN, "")) {
        char cmd_lan[75 + 10];
        sprintf(cmd_lan, "ip -f inet -o addr show %s|cut -d\\  -f 7 | cut -d/ -f 1 | sed -re 's/\\n//g'", DEVICE_LAN);
        cmd = popen(cmd_lan, "r");
        while (fgets(bf, 30, cmd) != 0) {
            strncpy(ip_eth, bf, 30);
        }
        pclose(cmd);
    }

    if (ip_wifi[0] != 0) {
        cmd = popen("iwgetid -r", "r");
        while (fgets(bf, 50, cmd) != 0) {
            strncpy(essid, bf, 50);
        }
        pclose(cmd);
    }

    if (ip_eth[0] == 0 && ip_wifi[0] != 0) {
        sprintf(status, " %s (%s)", strtok(ip_wifi, "\n"), strtok(essid, "\n"));
    } else if (ip_eth[0] != 0 && ip_wifi[0] == 0) {
        sprintf(status, " %s", strtok(ip_eth, "\n"));
    } else if (ip_eth[0] != 0 && ip_wifi[0] != 0) {
        sprintf(status, " %s (%s) %s  %s", strtok(ip_wifi, "\n"), strtok(essid, "\n"), ARROW_LEFT, strtok(ip_eth, "\n"));
    } else if (ip_eth[0] == 0 && ip_wifi[0] == 0) {
        sprintf(status, "");
        on = 0;
    }

    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s", COLOR_BG_NET, COLOR_BG_AUDIO, SEP_LEFT, COLOR_FG_NET, COLOR_BG_NET, status);

    return on;
}


int get_backlight (char* buf, char* color) {
    char val[200];
    char status[4];
    char *status_icon;
    char *ptr;
    char bl_path[45 + 10];
    char bl_bright[55 + 17];

    sprintf(bl_path, "/sys/class/backlight/%s_backlight", BACKLIGHT_TYPE);

    sprintf(bl_bright, "%s/%s", bl_path, "max_brightness");
    read_value(bl_bright, val);
    int max_level = strtol(val, &ptr, 10);

    sprintf(bl_bright, "%s/%s", bl_path, "actual_brightness");
    read_value(bl_bright, val);
    int lvl = strtol(val, &ptr, 10);

    float per =  ((float) lvl) / ((float) max_level);
    int percent = (int) (per * 100);

    status_icon = "";

    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %d", COLOR_BG_BACK, color, SEP_LEFT, COLOR_FG_BACK, COLOR_BG_BACK, status_icon, percent);
    return 1;
}


int get_battery(char* buf) {
    char val[200];
    char status[4];
    char *status_icon;
    char *ptr;
    char bat_path_full[40];
    char bat_path_now[40];
    char bat_path_status[40];
    char *bat_path_base = "/sys/class/power_supply/BAT0";
    char bat_type_name[7];
    char bat_path_buf[50];
    sprintf(bat_path_buf, "%s/status", bat_path_base);

    if(file_exists("/sys/class/power_supply/BAT0/energy_full") == 1) {
        strcpy(bat_type_name, "energy");
    } else {
        strcpy(bat_type_name, "charge");
    }

    sprintf(bat_path_full, "%s/%s_full", bat_path_base, bat_type_name);
    sprintf(bat_path_now, "%s/%s_now", bat_path_base, bat_type_name);
    sprintf(bat_path_status, "%s/status", bat_path_base);

    read_value(bat_path_full, val);
    if(DEBUG) fprintf(stderr, "%s = %s\n", bat_path_full, val);

    int max_level = strtol(val, &ptr, 10) / 10000;
    read_value(bat_path_now, val);
    if(DEBUG) fprintf(stderr, "%s = %s\n", bat_path_now, val);

    int lvl = strtol(val, &ptr, 10) / 10000;
    float per =  ((float) lvl) / ((float) max_level);
    int percent = (int) (per * 100);

    read_value(bat_path_status, val);
    for (int i=0; i < 3; i++) {
        status[i] = val[i];
    }
    status[3] = '\0';
    if (status[0] == 'C') {
        status_icon = "";
    } else {
        status_icon = "";
        if (percent < 80) {
            status_icon = "";
        } else if (percent < 50) {
            status_icon = "";
        } else if (percent < 30) {
            status_icon = "";
        }
    }

    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %d%%", COLOR_BG_BAT, COLOR_BG_BACK, SEP_LEFT, COLOR_FG_BAT, COLOR_BG_BAT,status_icon, percent);
    return 1;
}


int get_date(char* buf) {
    time_t current_time;
    char* time_string;

    char *months[] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};
    char *days[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

    current_time = time(NULL);
    struct tm tm = *localtime(&current_time);

    if (current_time == ((time_t) - 1)) {
        printf("Error");
        return -1;
    }

    time_string = ctime(&current_time);

    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %s %d %s %s %02d:%02d:%02d %%{B%s}", COLOR_BG_DATE, COLOR_BG_BAT, SEP_LEFT, COLOR_FG_DATE, COLOR_BG_DATE, days[tm.tm_wday], ARROW_LEFT,tm.tm_mday, months[tm.tm_mon], ARROW_LEFT, tm.tm_hour, tm.tm_min, tm.tm_sec, COLOR_BG);

    return 1;
}


int read_value(char* dir, char* line) {
    FILE* fp = fopen(dir, "r");
    if(fp == NULL) {
        return -1;
    }
    fscanf(fp, "%[^\n]", line);
    fclose(fp);
    return 1;
}


int file_exists(char *file) {
    FILE* fp = fopen(file, "r");
    if(fp == NULL)
        return -1;

    fclose(fp);
    return 1;
}


uint64_t get_time_stamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}


void savepoint(const char* text) {
#if PROFILE > 0
    uint64_t tmp = get_time_stamp() - lastSave;
    fprintf(stderr, "[%15s] %ulll ns\n", text, tmp);
    lastSave = get_time_stamp();
#endif
}

