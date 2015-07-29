#include "statusline.h"

char* ARROW_LEFT = "";
char* ARROW_RIGHT = "";
char* SEP_LEFT = "";
char* SEP_RIGHT = "";

int main() {
    //while (1) { 
        char buf_bat[200];
        char buf_back[200];
        char buf_net[200];
        char buf_audio[200];
       
        get_battery(buf_bat);
        if (get_network(buf_net) == 1) {
            get_backlight(buf_back, COLOR_BG_NET);
            get_audio(buf_audio);
            printf("%{r} %s %s %s %s %s\n", buf_audio, buf_net,buf_back, buf_bat, get_date());
        } else {
            get_backlight(buf_back, COLOR_BG_AUDIO);
            get_audio(buf_audio);
            printf("%{r} %s %s %s %s\n", buf_audio, buf_back, buf_bat, get_date());
        }
        
        //usleep(500);
    //}
    return 0;
}

int get_network(char* buf) {
    
    int percent = 45;
    char* status_icon = "Wifi placeholder";
    
    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %d%% ", COLOR_BG_NET, COLOR_BG_AUDIO, SEP_LEFT, COLOR_FG_NET, COLOR_BG_NET, status_icon, percent);
    
    return 0;
}

int get_audio(char* buf) {
    FILE* cmd;
    char vol[10];
    char* percent = "65";
    
    //cmd = popen("amixer get Master | grep 'Front Left:' | sed -r 's/\s*Front Left: Playback [0-9]* \[//' | sed -r 's/\] \[.*//' | sed -r 's/\s*//g'", "r");

    //while (fgets(vol, 10, cmd) != 0) {
    //    printf("%s", vol);    
    //}
   
    //pclose(cmd);

    char* status_icon = "Volume placeholder";
    
    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %s%% ", COLOR_BG_AUDIO, COLOR_BG, SEP_LEFT, COLOR_FG_AUDIO, COLOR_BG_AUDIO, status_icon, percent);
    
    return 1;
}

int get_backlight (char* buf, char* color) {
    char val[200];
    char status[4];
    char *status_icon;
    char* ptr;
    read_value("/sys/class/backlight/intel_backlight/max_brightness", val);
    int max_level = strtol(val, &ptr, 10);
    read_value("/sys/class/backlight/intel_backlight/actual_brightness", val);
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
    char* ptr;
    read_value("/sys/class/power_supply/BAT0/energy_full", val);
    int max_level = strtol(val, &ptr, 10) / 10000;
    read_value("/sys/class/power_supply/BAT0/energy_now", val);
    int lvl = strtol(val, &ptr, 10) / 10000;
    float per =  ((float) lvl) / ((float) max_level);
    int percent = (int) (per * 100);
    
    read_value("/sys/class/power_supply/BAT0/status", val);
    for (int i=0; i < 3; i++) {
        status[i] = val[i];
    }
    status[3] = '\0';
    printf(status);
    ptr = "Cha";
    if (status[0] == 'C') {
        status_icon = "";
        printf("test");
    } else {
        if (percent < 80) {
            status_icon = "";
        } else if (percent < 50) {
            status_icon = "";
        } else if (percent < 15) {
            status_icon = "";
        }
    }

    sprintf(buf, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %d%%", COLOR_BG_BAT, COLOR_BG_BACK, SEP_LEFT, COLOR_FG_BAT, COLOR_BG_BAT,status_icon, percent);
    return 1;
}

int read_value(char* dir, char* line) {
    FILE* fp = fopen(dir, "r"); 
    fscanf(fp, "%[^\n]", line);
    fclose(fp);
    return 1;
}

char* get_date() {
    time_t current_time;
    char* time_string;
   
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};
    char *days[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

    current_time = time(NULL);
    struct tm tm = *localtime(&current_time);

    if (current_time == ((time_t) - 1)) {
        printf("Error");
        return "-1";
    }

    time_string = ctime(&current_time);

    sprintf(time_string, "%%{F%s}%%{B%s}%s%%{F%s}%%{B%s} %s %s %d %s %s %02d:%02d:%02d %%{B%s}", COLOR_BG_DATE, COLOR_BG_BAT, SEP_LEFT, COLOR_FG_DATE, COLOR_BG_DATE, days[tm.tm_wday], ARROW_LEFT,tm.tm_mday, months[tm.tm_mon], ARROW_LEFT, tm.tm_hour, tm.tm_min, tm.tm_sec, COLOR_BG);

    return time_string;
}
