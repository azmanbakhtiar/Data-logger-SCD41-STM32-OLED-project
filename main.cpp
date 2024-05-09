 #include "mbed.h"
#include "OLEDDisplay.h"
#include <cstdio>
#include <stdio.h>
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "mbed.h"
#include "mbed.h"
#include "SDBlockDevice.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <errno.h>
#include "platform/mbed_retarget.h"
#include "FATFileSystem.h"
#include <string.h>

#define LOG_FILE_NAME "/fs/log.csv"
#define ROOT_DIR      "/fs/"

FileHandle* console_in  = mbed_file_handle(STDIN_FILENO);
FileHandle* console_out = mbed_file_handle(STDOUT_FILENO);

const int   buffer_size = 256;
char        buffer[buffer_size];
int         read_count;
time_t seconds;
SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO,MBED_CONF_SD_SPI_CLK,MBED_CONF_SD_SPI_CS);
FATFileSystem fs("fs");
bool SD_card_logging_flag = true;

FILE *fd; DIR *dir;  


bool log_data_to_SD_card(int temp, int humi, int carbon_dioxide_ppm);
int check_for_log_file(void);
void mountSD_CARD(void);
void generateBeep();
DigitalOut buzzerPin(D7); 
extern I2C i2c_connection;
OLEDDisplay oled;
int main()




{
    int new_temperature;
    int error_code;
    set_time(1713874450);  
    buzzerPin = 1;
    oled.clear();
    char buffer[10];
    int16_t error = 0;
    uint16_t co2;
    int32_t temperature;
    int32_t humidity;
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    wait_us(10000000);

     mountSD_CARD();

    //Check for the log file
    error_code = check_for_log_file();
    if (error_code == -1) {
        printf("Error: Cannot log data to the SD card.\n");
        SD_card_logging_flag = false; // Set flag to false if log file creation failed
    }

    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        printf("Error executing scd4x_get_serial_number(): %i\n", error);
    } else {
        printf("serial: 0x%04x%04x%04x\n", serial_0, serial_1, serial_2);
    }
    error = scd4x_start_periodic_measurement();
    if (error) {
        printf("Error executing scd4x_start_periodic_measurement(): %i\n",
               error);
    }
    printf("Waiting for first measurement... (5 sec)\n");



    while (true) {

        // Read Measurement
        sensirion_i2c_hal_sleep_usec(1000);
        bool data_ready_flag = false;
        error = scd4x_get_data_ready_flag(&data_ready_flag);
        if (error) {
            printf("Error executing scd4x_get_data_ready_flag(): %i\n", error);
            continue;
        }
        if (!data_ready_flag) {
            continue;
        }

        error = scd4x_read_measurement(&co2, &temperature, &humidity);
        if (error) {
            printf("Error executing scd4x_read_measurement(): %i\n", error);
        } else {
            printf("CO2: %u\n", co2);
            printf("Temperature: %d m°C\n", temperature);
            printf("Humidity: %d mRH\n", humidity);

            oled.cursor(0, 0);
            oled.printf("---JASPER  PROJECT--- ");
            oled.cursor(1, 0);
            oled.printf("Co2 LEVEL   | %d\n", co2);
            if (co2 < 1000) {
                generateBeep();
            }

            oled.cursor(2, 0);
            new_temperature = temperature / 1000;
            oled.printf("TEMPERATURE | %d\n", new_temperature);
            if (new_temperature > 25 || new_temperature < 20) {
                generateBeep();
            }

            oled.cursor(2, 17);
            oled.printf(" C ");
            oled.cursor(3, 0);
            humidity = humidity / 1000;
            oled.printf("HUMIDITY    | %d\n", humidity);
            if (humidity > 60 || humidity < 30) {
                generateBeep();
            }

            oled.cursor(3, 17);
            oled.printf(" RH ");
            wait_us(2000000);


        }
         int i = rand()/10000;
        if (!log_data_to_SD_card(new_temperature,humidity,co2))
        {
            printf("Error while Logging..\n");
        }

        while (console_in->readable())
        {
            read_count = console_in->read(buffer, buffer_size);
            printf("Time Syc Recevied: %s", buffer);
            long timeSerialinput;
            if (sscanf(buffer,"%ld",&timeSerialinput) == 1)
            {
                set_time(timeSerialinput);
                printf("Parsed value: %ld\n", timeSerialinput);
            }
            else {
                printf("Error: Failed to parse the value\n");
            }
            
        }
        ThisThread::sleep_for(1000ms);


        ThisThread::sleep_for(9000ms);
    }

    return 0;
}

void generateBeep() {
    buzzerPin = 1;
    ThisThread::sleep_for(500ms);
    buzzerPin = 0;
    ThisThread::sleep_for(1000ms);
    buzzerPin = 1;
}

void mountSD_CARD(void)
{
    int error = 0;
    printf("Mounting SD card and FAT File system\n");
    error = fs.mount(&sd);
    if (error)
    {
        printf("No filesystem found, formatting... ");
        error = fs.reformat(&sd);
        if (error){
            printf("Formatted Successfully, now mounting again...\n");
        }
        else 
        {
            printf("Error while formatting the SD... Pleaes format it as FAT32 from PC and insert again...\n");
        }
    }
    else
    {
        printf("Mounted succefully\n");
    }
}

int check_for_log_file(void) {
    FILE *fd = fopen(LOG_FILE_NAME, "r"); // Open file in read mode
    if (fd == NULL) {
        // File does not exist, create it
        fd = fopen(LOG_FILE_NAME, "w+");
        if (fd != NULL) {
            printf("Log file created successfully: %s\n", LOG_FILE_NAME);
            fprintf(fd, "Timestamp,Temperature,Humidity,Carbon Dioxide_PPM\n");
            fclose(fd);
            return 0; // File created successfully
        } else {
            printf("Failed to create log file: %s\n", LOG_FILE_NAME);
            return -1; // Error creating file
        }
    } else {
        fclose(fd); // File exists, close it
        printf("Log file already exists: %s\n", LOG_FILE_NAME);
        return 1; // File already exists
    }
}

bool log_data_to_SD_card(int temp, int humi, int carbon_dioxide_ppm) {
    // Get current time
    time_t current_time = time(NULL);
    if (current_time == -1) {
        printf("Error: Failed to get current time.\n");
        return false;
    }

    // Format time as string
    char buffer[40];
    
    strftime(buffer,40, "%a %d/%m/%Y-%H:%M:%S", localtime(&current_time));
    printf("Entry logged at: %s\n", buffer);
    // Open file in append mode
    FILE *fd = fopen(LOG_FILE_NAME, "a");
    if (fd == NULL) {
        printf("Failed to open log file for appending: %s\n", LOG_FILE_NAME);
        return false;
    }

    // Append data to the file
    fprintf(fd, "%s,%d,%d,%d\n", buffer, temp, humi, carbon_dioxide_ppm);

    // Close the file
    fclose(fd);

    return true;
}