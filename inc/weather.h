#ifndef WEATHER_H
#define WEATHER_H

#include "main.h"

int check_last_api_access_weather(prayers_t *prayer_times);
int retrieve_weather(prayers_t *prayer_times);
#endif
