#ifndef DS1307_H
#define DS1307_H

#include "../../Drivers/Inc/stm32f407xx.h"
#include "../../Drivers/Inc/stm32f4xx_gpio_driver.h"
#include "../../Drivers/Inc/stm32f4xx_i2c_driver.h"


/* I2C Connection configuration */
#define DS1307_I2C                          I2C1
#define DS1307_GPIO_SDA_PIN_NO              7
#define DS1307_GPIO_SCL_PIN_NO              6
#define DS1307_GPIO_SDA_PORT                GPIOB
#define DS1307_GPIO_SCL_PORT                GPIOB
#define DS1307_GPIO_AF                      GPIO_AF_4
#define DS1307_I2C_SPEED                    I2C_SCL_SPEED_SM
#define DS1307_I2C_ACK_CONTROL              I2C_ACK_ENABLE
#define DS1307_GPIO_PIN_PUPD                GPIO_PUPDR_NO


/* DS1307 register addresses */
#define DS1307_ADDRESS_SEC       0x00
#define DS1307_ADDRESS_MIN       0x01
#define DS1307_ADDRESS_HOUR      0x02
#define DS1307_ADDRESS_DAY       0x03
#define DS1307_ADDRESS_DATE      0x04
#define DS1307_ADDRESS_MONTH     0x05
#define DS1307_ADDRESS_YEAR      0x06
#define DS1307_ADDRESS_CONTROL   0x07

/* DS1307 Address (I2C SLAVE)*/
#define DS1307_I2C_ADDRESS       0x68

/* DS1307 Settings */
#define DS1307_TIME_FORMAT_24H      0
#define DS1307_TIME_FORMAT_12H_AM   1
#define DS1307_TIME_FORMAT_12H_PM   2


typedef struct {
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint16_t year;
} RTC_Date_t;

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t time_format;
} RTC_Time_t;


/* BSP Function Prototypes */
uint8_t DS1307_Init(void);
void DS1307_SetTime(RTC_Time_t *rtc_time);
void DS1307_GetTime(RTC_Time_t *rtc_time);
void DS1307_SetDate(RTC_Date_t *rtc_date);
void DS1307_GetDate(RTC_Date_t *rtc_date);


#endif