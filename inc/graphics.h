#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "prayer.h"

#define RESET_COLOUR    printf("\033[0m")
#define CRESET printf("\e[0m")
#define BG printf("\e[0;103m")
#define YEL printf("\e[1;93m")
#define WHT printf("\e[1;90m")
//#define RED printf("\e[1;97m")
#define RED printf("\e[1;97m")

int showTable(prayers_t prayer_times, date_t date);
#endif
