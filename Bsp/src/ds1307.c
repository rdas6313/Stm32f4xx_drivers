#include "ds1307.h"
#include <string.h>
#include <stdint.h>


/* static Function prototypes */
static void init_I2C_GPIOs();
static void init_I2C_Peripheral();
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t reg_addr,uint8_t data);
static uint8_t binary_to_bcd(uint8_t binary_data);
static uint8_t bcd_to_binary(uint8_t bcd_data);


static I2C_Handle_t I2CHandle;


/*
* @brief This function initializes the GPIO pins for I2C communication
* @param None
* @return None
*/
static void init_I2C_GPIOs(){
    GPIO_Handle_t I2CGpioHandleSDA, I2CGpioHandleSCL;

    memset(&I2CGpioHandleSDA,0,sizeof(I2CGpioHandleSDA));
    memset(&I2CGpioHandleSCL,0,sizeof(I2CGpioHandleSCL));

    // I2C SDA pin configuration
    I2CGpioHandleSDA.pGPIOx = DS1307_GPIO_SDA_PORT;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinAltFunMode = DS1307_GPIO_AF;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_OD;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_GPIO_PIN_PUPD;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    I2CGpioHandleSDA.GPIO_PinConfig.GPIO_PinNumber = DS1307_GPIO_SDA_PIN_NO;
    // enable GPIO SDA clock
    GPIO_PeriClockControl(I2CGpioHandleSDA.pGPIOx, ENABLE);
    GPIO_Init(&I2CGpioHandleSDA);


    // I2C SCL pin configuration
    I2CGpioHandleSCL.pGPIOx = DS1307_GPIO_SCL_PORT;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinAltFunMode = DS1307_GPIO_AF;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_OD;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_GPIO_PIN_PUPD;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    I2CGpioHandleSCL.GPIO_PinConfig.GPIO_PinNumber = DS1307_GPIO_SCL_PIN_NO;
    // enable GPIO SCL clock
    GPIO_PeriClockControl(I2CGpioHandleSCL.pGPIOx, ENABLE);
    GPIO_Init(&I2CGpioHandleSCL);

}

/*
* @brief This function initializes the I2C peripheral for DS1307 communication
* @param None
* @return None
*/
static void init_I2C_Peripheral(){

    memset(&I2CHandle,0,sizeof(I2CHandle));

    I2CHandle.pI2Cx = I2C1;
    I2CHandle.i2cConfig.I2C_SCLSpeed = DS1307_I2C_SPEED;
    I2CHandle.i2cConfig.I2C_ACKControl = DS1307_I2C_ACK_CONTROL;
    I2C_Init((I2C_Handle_t*)&I2CHandle);

}

/*
* @brief This function writes a byte of data to a specific register of DS1307
* @param reg_addr: Register address to write to
* @param data: Data byte to write
* @return None
*/
static void ds1307_write(uint8_t reg_addr,uint8_t data){
    uint8_t buffer[] = {reg_addr, data};
    I2C_MasterSendData(&I2CHandle,buffer,2,DS1307_I2C_ADDRESS,I2C_DISABLE_SR);
}

/*
* @brief This function reads a byte of data from a specific register of DS1307
* @param reg_addr: Register address to read from
* @return Data byte read from the register
*/
static uint8_t ds1307_read(uint8_t reg_addr){
    uint8_t reg_value = 0;
    I2C_MasterSendData(&I2CHandle,&reg_addr,1,DS1307_I2C_ADDRESS,I2C_ENABLE_SR);
    I2C_MasterReceiveData(&I2CHandle,&reg_value,1,DS1307_I2C_ADDRESS,I2C_DISABLE_SR);
    return reg_value;
}

/*
* @brief This function initializes the DS1307 RTC module
* @param None
* @return 1 if oscillator is enabled, 0 otherwise
*/
uint8_t DS1307_Init(void){
    // 1. Initialize I2C GPIOs
    init_I2C_GPIOs();
    // 2. Initialize I2C Peripheral
    init_I2C_Peripheral();
    // 3. Enable I2C Peripheral
    I2C_PeripheralControl(I2CHandle.pI2Cx, ENABLE);
    // 4. Enable Ocilator by writing 0x00 to register 0x00
    ds1307_write(0x00,0x00);
    // 5. Check if oscillator is enabled, if Ch bit of reg_value is 0 means enabled
    uint8_t reg_value = ds1307_read(0x00);
    uint8_t is_enable = !(reg_value >> 7);

    return is_enable;

}

/*
* @brief This function converts binary data to BCD format
* @param binary_data: Binary data to be converted
* @return BCD formatted data
*/
static uint8_t binary_to_bcd(uint8_t binary_data){
    // Convert binary to BCD
    return 1;
}

/*
* @brief This function converts BCD formatted data to binary
* @param bcd_data: BCD formatted data to be converted
* @return Binary data
*/
static uint8_t bcd_to_binary(uint8_t bcd_data){
    // Convert BCD to binary
    return 1;
}


/*
* @brief This function sets the time on the DS1307 RTC module
* @param rtc_time: Pointer to RTC_Time_t structure containing time information
* @return None
*/
void DS1307_SetTime(RTC_Time_t *rtc_time){
    // configure hours
    uint8_t hour = binary_to_bcd(rtc_time->hours);
    if(rtc_time->time_format == DS1307_TIME_FORMAT_24H){
        hour &= ~(1 << 6); // clear 6th bit for 24H format
    }else{
        hour |= (1 << 6); // set 6th bit for 12H format
        hour = (rtc_time->time_format == DS1307_TIME_FORMAT_12H_PM) ? (hour | (1 << 5)) : (hour & ~(1<<5)); // set or clear 5th bit for AM/PM
    }
    
    ds1307_write(DS1307_ADDRESS_HOUR,hour);
    // configure minutes
    ds1307_write(DS1307_ADDRESS_MIN,binary_to_bcd(rtc_time->minutes));

    // configure seconds
    uint8_t seconds = binary_to_bcd(rtc_time->seconds);
    seconds &= ~(1 << 7); // ensure CH bit is cleared to enable oscillator
    ds1307_write(DS1307_ADDRESS_SEC,seconds);
}

/*
* @brief This function gets the current time from the DS1307 RTC module
* @param rtc_time: Pointer to RTC_Time_t structure to store retrieved time information
* @return None
*/
void DS1307_GetTime(RTC_Time_t *rtc_time){
    // read seconds
    uint8_t second_reg_value = ds1307_read(DS1307_ADDRESS_SEC);
    rtc_time->seconds = bcd_to_binary(second_reg_value & 0x7F); // mask CH bit
    // read minutes
    uint8_t minute_reg_value = ds1307_read(DS1307_ADDRESS_MIN);
    rtc_time->minutes = bcd_to_binary(minute_reg_value & 0x7F);
    // read hours
    uint8_t hour_reg_value = ds1307_read(DS1307_ADDRESS_HOUR);
    if((hour_reg_value >> 6) & 0x1){
        // 12H format
        rtc_time->time_format = ((hour_reg_value >> 5) & 0x1) ? DS1307_TIME_FORMAT_12H_PM : DS1307_TIME_FORMAT_12H_AM;
        rtc_time->hours = bcd_to_binary(hour_reg_value & 0x1F);
    }else{
        // 24H format
        rtc_time->time_format = DS1307_TIME_FORMAT_24H;
        rtc_time->hours = bcd_to_binary(hour_reg_value & 0x3F);
    }
}

/*
* @brief This function sets the date on the DS1307 RTC module
* @param rtc_date: Pointer to RTC_Date_t structure containing date information
* @return None
*/
void DS1307_SetDate(RTC_Date_t *rtc_date){
    // configure day
    ds1307_write(DS1307_ADDRESS_DAY,binary_to_bcd(rtc_date->day) & 0x07);
    // configure date
    ds1307_write(DS1307_ADDRESS_DATE,binary_to_bcd(rtc_date->date) & 0x3F);
    // configure month
    ds1307_write(DS1307_ADDRESS_MONTH,binary_to_bcd(rtc_date->month) & 0x1F);
    // configure year
    ds1307_write(DS1307_ADDRESS_YEAR,binary_to_bcd(rtc_date->year));
}

/*
* @brief This function gets the current date from the DS1307 RTC module
* @param rtc_date: Pointer to RTC_Date_t structure to store retrieved date information
* @return None
*/
void DS1307_GetDate(RTC_Date_t *rtc_date){
    // read day
    uint8_t day_reg_value = ds1307_read(DS1307_ADDRESS_DAY);
    rtc_date->day = bcd_to_binary(day_reg_value & 0x07);
    // read date
    uint8_t date_reg_value = ds1307_read(DS1307_ADDRESS_DATE);
    rtc_date->date = bcd_to_binary(date_reg_value & 0x3F);
    // read month
    uint8_t month_reg_value = ds1307_read(DS1307_ADDRESS_MONTH);
    rtc_date->month = bcd_to_binary(month_reg_value & 0x1F);
    // read year
    uint8_t year_reg_value = ds1307_read(DS1307_ADDRESS_YEAR);
    rtc_date->year = bcd_to_binary(year_reg_value);
}



