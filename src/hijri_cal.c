#include "../inc/prayer.h"

void retrieve_hijri_date(prayers_t *prayer_times){

    char buff[20];
    FILE *fp = popen("hijra_cal", "r");
    fgets(buff, sizeof(buff), fp);

    sscanf(buff, "%d-%d-%d", 
            &prayer_times->current_date.hijri_day,
            &prayer_times->current_date.hijri_month,
            &prayer_times->current_date.hijri_year);

    pclose(fp); fp = NULL;
}
