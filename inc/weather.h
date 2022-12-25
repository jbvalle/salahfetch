#ifndef WEATHER_H
#define WEATHER_H

#include "prayer.h"

// Intervall Api Access
#define API_INTERVALL 40


int check_last_api_access(prayers_t *prayer_times);
int retrieve_weather(prayers_t *prayer_times);
#endif
