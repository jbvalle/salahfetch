#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "prayers.h"
#include "weather.h"
#include "hijri.h"
#include "graphics.h"

#define RESET_COLOUR    printf("\033[0m")
#define CRESET printf("\e[0m")
#define BG printf("\e[0;103m")
#define YEL printf("\e[1;93m")
#define WHT printf("\e[1;90m")
//#define RED printf("\e[1;97m")
#define RED printf("\e[1;97m")

// Intervall Api Access
#define API_INTERVALL 40

#define API_INTERVALL_hijri 40
#endif
