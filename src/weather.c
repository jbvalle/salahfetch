#include "main.h"


// Checks if conditions are met for calling the weather API 
// If the return value is 0 this indicates that API access is allowed
// If the return value is other than 0 this mean that no API call should be made
int check_last_api_access_weather(prayers_t *prayer_times){

    // Check if internet connectivity exists
    int internet_conn = system("ping -c 1 google.com > /dev/null 2>&1");
    // If return_val = 0 -> internet connectivity exists , otherwise not and process terminates
    int return_val = (internet_conn == 0) ? 0 : -1;
    // terminate if there is no internet connectivity 
    if(return_val != 0){

        return return_val;
    }

    // file path for api access to handle when api should be called or not
    char filepath[] = "/home/strayker/.config/weather/last_access\0";

    FILE *fp = fopen(filepath, "a+");

    // Error handling checking valid file pointer
    if(fp == NULL){

        fprintf(stderr, "ERROR Reading Weather last_access File");
        return -1;
    }

    
    //Check if file is empty
    //If it is empty overwrite it with the current time and exit 
    struct stat st;
    if((stat(filepath, &st) == 0) && (st.st_size == 0)){

        WHT;
        printf("Info: last_access File was not found\n");
        printf("Info: new last_access File is being generated, API_CALL will be initiated\n");
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
    if(diff <= API_INTERVALL){

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

int retrieve_weather(prayers_t *prayer_times){


    FILE *fp = NULL;
    char proc_buff[50];
    char weather_info_filepath[] = "/home/strayker/.config/weather/info\0";

    // if check() == 0 -> make an API CALL otherwise just read the info file
    if(check_last_api_access_weather(prayer_times) == 0){
        
        // Retrieve the return value of the weather process
        FILE *proc_ptr = popen("get_weather", "r");
        fgets(proc_buff, sizeof(proc_buff), proc_ptr);
        pclose(proc_ptr); proc_ptr = NULL;
    
        WHT;
        printf("Info: Weather API Call initiated, minimum API period exceeded\n");
        RESET_COLOUR;
        
        // If API Call is valid create the file with rw permissions and 
        fp = fopen(weather_info_filepath, "w+");
        // Write the get_weather process output to info file 
        fprintf(fp, "%s", proc_buff); fclose(fp); fp = NULL;

        // reassign file pointer to same file but with read permission
        fp = fopen(weather_info_filepath, "r+");

    }else{

        // Create file pointer to info file in order to read it
        fp = fopen(weather_info_filepath, "a+");

        // Error handling checking valid file pointer
        if(fp == NULL){

            fprintf(stderr, "ERROR Reading Weather File");
            return -1;
        }

        struct stat st;
        if((stat(weather_info_filepath, &st) == 0) && (st.st_size == 0)){

            WHT;
            printf("\nInfo: info file was not found\n");
            printf("\nInfo: API call needs to be manually initiated - \nExecute: prayer_times API_CALL\n");
            RESET_COLOUR;

            // Overwrite the content
            fprintf(fp, "0, API Call necessary");

            fclose(fp); fp = NULL;
            fp = fopen(weather_info_filepath, "r+");
        }
    }

    //Initialize tmp and info values
    prayer_times->weather.tmp = 9999;
    for(int i = 0;i < (int)sizeof(prayer_times->weather.info); i++){

        prayer_times->weather.info[i] = '\0';
    }

    // Token for strok
    // tmp for buffer for strtok
    // seperator for delimiter for strtok
    char *token, buff[100], seperator[3] = ",\n";
    char letter;

    // Read first line of input stream to dump header
    fgets(buff, sizeof(buff), fp);

    // Read First token of string
    token = strtok(buff, seperator); 

    // Parse the tokens and save prayer times
    for(int i = 0; (token != NULL ) && (i < 30); i++){

        // First argument -> min TMP
        if(i == 0)sscanf(token, "%lf", &prayer_times->weather.tmp);
        // Second argument -> max TMP
        if(i == 1){

            for(int j = 0; ((letter = token[j]) != '\0') && (j < 30 ); j++){

                prayer_times->weather.info[j] = letter;
            }
        }
        token = strtok(NULL, seperator);
    }

    fclose(fp); fp = NULL;

    return 0;

}
