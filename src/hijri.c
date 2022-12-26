
#include "main.h"


// Checks if conditions are met for calling the hijri API 
// If the return value is 0 this indicates that API access is allowed
// If the return value is other than 0 this mean that no API call should be made
int check_last_api_access_hijri(prayers_t *prayer_times){

    char filepath[] = "/home/strayker/.config/hijri_cal/last_access\0";

    FILE *fp = fopen(filepath, "a+");

    // Error handling checking valid file pointer
    if(fp == NULL){

        fprintf(stderr, "ERROR Reading hijri last_access File");
        return -1;
    }

    
    //Check if file is empty
    //If it is empty overwrite it with the current time and exit 
    struct stat st;
    if((stat(filepath, &st) == 0) && (st.st_size == 0)){

        WHT;
        printf("Info: hijri last_access File was not found\n");
        printf("Info: new hijri last_access File is being generated, API_CALL will be initiated\n");
        RESET_COLOUR;

        // Overwrite the content
        fprintf(fp, "%d:%d", 
                prayer_times->current_date.current_hour,
                prayer_times->current_date.current_min);


        fclose(fp); fp = NULL;
        // If the file has not been created before the current time is written to it and 0 is returned meaning api access is permitted
        return 0;
    }

    // Read File
    char buff[100]; fgets(buff, sizeof(buff), fp);
    int hour, min, diff;

    // Parse hour and min from file
    sscanf(buff, "%d:%d", &hour, &min);

    // Determine Difference is greater than XX min
    diff = abs((prayer_times->current_date.current_hour * 60 + prayer_times->current_date.current_min) - (hour * 60 + min));

    // If the difference is smaller than the API intervall do nothing and exist
    if(diff <= API_INTERVALL_hijri){

        fclose(fp); fp = NULL;
        return -1;
    }

    // If the diff is bigger than the API Intervall than overwrite the file by repositioning the pointer and overwrite current time
    // reposition the file pointer /to the beginning of the file
    //(void)fseek(fp, 0, SEEK_SET) ;
    fp = fopen(filepath, "w+");

    // Overwrite the content
    fprintf(fp, "%d:%d", 
            prayer_times->current_date.current_hour,
            prayer_times->current_date.current_min);
                
    fclose(fp); fp = NULL;
    return 0;
}

int retrieve_hijri_date(prayers_t *prayer_times){


    char hijri_info_path[] = "/home/strayker/.config/hijri_cal/info\0";
    FILE *hijri_info_ptr;

    if(check_last_api_access_hijri(prayer_times) == 0){
        
        // Retrieve the return value of the weather process
        char buff[20];
        FILE *fp = popen("hijra_cal", "r");
        fgets(buff, sizeof(buff), fp); pclose(fp); fp = NULL;
    
        WHT;
        printf("Info: Hijri API Call initiated, minimum API period exceeded\n");
        RESET_COLOUR;
        
        // If API Call is valid create the file with rw permissions and 
        hijri_info_ptr = fopen(hijri_info_path, "w+");
        // Write the get_weather process output to info file 
        fprintf(hijri_info_ptr, "%s", buff); fclose(hijri_info_ptr); hijri_info_ptr = NULL;

        // reassign file pointer to same file but with read permission
        hijri_info_ptr = fopen(hijri_info_path, "r+");

    }else{

        // Create file pointer to info file in order to read it
        hijri_info_ptr = fopen(hijri_info_path, "a+");

        // Error handling checking valid file pointer
        if(hijri_info_ptr == NULL){

            fprintf(stderr, "ERROR Reading hijra info File");
            return -1;
        }

        struct stat st;
        if((stat(hijri_info_path, &st) == 0) && (st.st_size == 0)){

            WHT;
            printf("\nInfo: hijri info file was not found\n");
            printf("\nInfo: hijri API call needs to be manually initiated - \nExecute: prayer_times HIJRI_API_CALL\n");
            RESET_COLOUR;

            // Overwrite the content
            fprintf(hijri_info_ptr, "11-11-1111");

            fclose(hijri_info_ptr); hijri_info_ptr = NULL;
            hijri_info_ptr = fopen(hijri_info_path, "r+");
        }
    }

    
    char buff[20];
    fgets(buff, sizeof(buff), hijri_info_ptr); fclose(hijri_info_ptr); hijri_info_ptr = NULL;

    sscanf(buff, "%d-%d-%d", 
            &prayer_times->current_date.hijri_day,
            &prayer_times->current_date.hijri_month,
            &prayer_times->current_date.hijri_year);

    return 0;
}



