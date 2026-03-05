

#include "sh1106.h"


/* * * * * * * * * * * * * * * * *  * * * * * 
* Private functions for SH1106 OLED display *
** * * * * * * * * * * * * * * *  * * * * * */

static void set_column(uint8_t column);
static void set_page(uint8_t page);
static void initialize_display();



/*
* @brief This function Initializes the SH1106 Controller. Must be called before any other SH1106 function
* @param: None
* @return None
*/
void SH1106_init(void){
    delay(1); // Delay for power stabilization and reset timing
    initialize_display(); // Initialize the SH1106 display with necessary commands
    SH1106_clear(); // Clear the display after initialization
    SH1106_display_on_off(SH1106_DISPLAY_ON); // Turn on the display after initialization
    delay(110); // Delay for display to stabilize after initialization
}

/*
* @brief This function writes the data into Data Ram of Display Controller. Display renders according to the data of Data Ram.
* @param data: takes 8x128 byte data. data format must follow SH1106 Controller data format.
* @return None
*/
void SH1106_show(uint8_t *data){
    
    #define TEMP_SIZE (SH1106_WIDTH + 3)
    for(int i = 0; i < SH1106_PAGE_MAX; i++){
        uint8_t temp[TEMP_SIZE] = {SH1106_DATA_CONTROL,0x00,0x00};
        memcpy(temp+1,(data + (SH1106_WIDTH * i)),SH1106_WIDTH);

        set_page(i);
        set_column(0);
        i2c_send_SH1106(SH1106_I2C_ADDRESS,temp,TEMP_SIZE);
    }

}

/*
* @brief This function clears the Display Data ram.
* @param data: None
* @return None
*/
void SH1106_clear(void){
    #define DATA_SIZE (SH1106_WIDTH + 3)
    uint8_t data[DATA_SIZE] = {SH1106_DATA_CONTROL,0x00,0x00};
    
    for(int i=0;i<SH1106_PAGE_MAX;i++){
        set_page(i);
        set_column(0);
        i2c_send_SH1106(SH1106_I2C_ADDRESS,data,DATA_SIZE);
    }


    
}

/*
* @brief This function turns on/off display.
* @param data: value could be either SH1106_DISPLAY_ON or SH1106_DISPLAY_OFF macro
* @return None
*/
void SH1106_display_on_off(uint8_t on){
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,(SH1106_DISPLAY_ON_OFF_COMMAND | on)}, 2);
}

/*
* @brief This function writes the initialize sequence to intialize the display settings. makes everything to its default value.
* @param data: None
* @return None
*/
static void initialize_display(){
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_LOWER_COL_DEFAULT},2); // Setting Lower 4 bits of the column address to 0
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_HIGHER_COL_DEFAULT},2); // Setting Higher 4 bits of the column address to 0
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_PUMP_VOLTAGE_DEFAULT},2); // Setting Pump Voltage to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_DISPLAY_START_LINE_DEFAULT},2); // Setting Display Start Line to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_CONTRAST_COMMAND, SH1106_CONTRAST_DEFAULT},3); // Setting Contrast to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_SEGMENT_REMAP_DEFAULT},2); // Setting Segment Remap to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_ENTIRE_DISPLAY_DEFAULT},2); // Setting Entire Display to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_NORMAL_INVERSE_DEFAULT},2); // Setting Normal/Inverse Display to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_MULTIPLEX_RATIO_COMMAND, SH1106_MULTIPLEX_RATIO_DEFAULT},3); // Setting Multiplex Ratio to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_DC_DC_COMMAND, SH1106_DC_DC_DEFAULT},3); // Setting DC-DC to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_DISPLAY_OFF_DEFAULT},2); // Setting Display Off to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_PAGE_START_ADDRESS_DEFAULT},2); // Setting Page Start Address to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_COM_SCAN_DIR_DEFAULT},2); // Setting COM Scan Direction to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_DISPLAY_OFFSET_COMMAND, SH1106_DISPLAY_OFFSET_DEFAULT},3); // Setting Display Offset to default value    
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_FREQUENCY_COMMAND, SH1106_FREQUENCY_DEFAULT},3); // Setting Frequency to default value   
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_PRE_DIS_CHARGE_COMMAND, SH1106_PRE_DIS_CHARGE_DEFAULT},3); // Setting Pre-Charge Period to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_COMMON_PADS_COMMAND, SH1106_COMMON_PADS_DEFAULT},3); // Setting Common Pads to default value
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,SH1106_VCOMH_DESELECT_LEVEL_COMMAND, SH1106_VCOMH_DESELECT_LEVEL_DEFAULT},3); // Setting VCOMH Deselect Level to default value
}


/*
* @brief This function set column no of column register of display controller. there are 0 to 127 column.
* @param data: Column no.
* @return None
*/
static void set_column(uint8_t column){
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,(SH1106_COLUMN_LOWER_ADDRESS_COMMAND | (column & 0x0F))}, 2); // Set lower 4 bits of column address
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,(SH1106_COLUMN_HIGHER_ADDRESS_COMMAND | ((column >> 4) & 0x0F))}, 2); // Set higher 4 bits of column address
}

/*
* @brief This function set the page no of page register of display controller. there are 0 to 7 pages. each page has 128 byte display data (0-127 column). per column holder each byte.
* @param data: page no
* @return None
*/
static void set_page(uint8_t page){
    i2c_send_SH1106(SH1106_I2C_ADDRESS, (uint8_t[]){SH1106_COMMAND_CONTROL,(SH1106_PAGE_ADDRESS_COMMAND | (page & 0x0F))}, 2); // Set page address
}

/*
* @brief This function is the user callback to i2c send. user must implement i2c send command inside this.
* @param address: i2c slave address
* @param data: pointer of i2c data 
* @param size: i2c data size 
* @return None
*/
__attribute__((weak)) void i2c_send_SH1106(uint8_t address,uint8_t *data, uint16_t size){
    /*
    * Implement this function in your project to send data to SH1106 display using I2C communication.
    */
}

/*
* @brief This function is the user callback of delay function. user must implement delay inside this.
* @param ms: delay in milli seconds. 
* @return None
*/
__attribute__((weak)) void delay(uint16_t ms){
    /*
    * Implement this function in your project to delay for a specified number of milliseconds. This is useful for timing operations in the SH1106 display.
    */
}