/*
*   LCD Driver source file
*/

#include "lcd.h"

void (*Delay_ms)(uint16_t delay) = NULL;

/*
* @brief This function writes 4 bits of data to the LCD data pins and generates an enable pulse to latch the data into the LCD controller.
* @param data: The 4-bit data to be sent to the LCD (lower nibble of the byte)
* @return None
*/
static void write_4bits(uint8_t data,uint16_t delay){
    // 1. set data[0] to D4 PIN
    GPIO_WriteToOutputPin(LCD_DATA4_PORT, LCD_DATA4_PIN_NO, ((data >> 0) & 0x01));
    // 2. set data[1] to D5 PIN
    GPIO_WriteToOutputPin(LCD_DATA5_PORT, LCD_DATA5_PIN_NO, ((data >> 1) & 0x01));
    // 3. set data[2] to D6 PIN
    GPIO_WriteToOutputPin(LCD_DATA6_PORT, LCD_DATA6_PIN_NO, ((data >> 2) & 0x01));
    // 4. set data[3] to D7 PIN
    GPIO_WriteToOutputPin(LCD_DATA7_PORT, LCD_DATA7_PIN_NO, ((data >> 3) & 0x01));

    Delay_ms(delay); // Short delay to allow data lines to stabilize
    // 5. Generate Enable pulse
    GPIO_WriteToOutputPin(LCD_EN_PORT, LCD_EN_PIN_NO, GPIO_PIN_SET);
    Delay_ms(delay);
    GPIO_WriteToOutputPin(LCD_EN_PORT, LCD_EN_PIN_NO, GPIO_PIN_RESET);
    Delay_ms(delay);

}


/*
* @brief This function initializes the LCD display by sending the required initialization commands to the LCD controller.
* @param fDelay: A function pointer to a delay function that takes a uint16_t delay in milliseconds. This function will be used to create necessary delays during the LCD initialization process.
* @return None
* @Note The LCD_Init function must be called after initializing the GPIO pins connected to the LCD. The provided delay function should create accurate delays to ensure proper timing for the LCD initialization sequence.
*/
void LCD_Init(void (*fDelay)(uint16_t delay)){

    if(fDelay == NULL)
        return; // Invalid delay function pointer, do nothing

    Delay_ms = fDelay; // Assign the provided delay function to the global function pointer

    Delay_ms(2000);

    // 1. Set RS pin low for command mode
    GPIO_WriteToOutputPin(LCD_RS_PORT, LCD_RS_PIN_NO, GPIO_PIN_RESET);

    // 2. Set RW pin low for write mode
    GPIO_WriteToOutputPin(LCD_RW_PORT, LCD_RW_PIN_NO, GPIO_PIN_RESET);

    // 3. Send sequence of function set commands to initialize LCD in 4-bit mode
    Delay_ms(1000);
    write_4bits(LCD_INIT_CMD1,500);
    //Delay_ms(3000);
    write_4bits(LCD_INIT_CMD2,500);
    //Delay_ms(3000);
    write_4bits(LCD_INIT_CMD3,500);
    //Delay_ms(3000);
    write_4bits(LCD_INIT_CMD4,500);
    //Delay_ms(3000);

    // 4. Configuring LCD Function Set
    LCD_SendCommand(LCD_FUNCTION_SET_4BIT_2LINE);

    // 5. Display OFF, Cursor OFF
    LCD_SendCommand(LCD_DISPLAY_OFF_CURSOR_OFF);
  

    // 6. Clear display
    LCD_SendCommand(LCD_DISPLAY_CLEAR);
  

    // 7. Entry mode set
    LCD_SendCommand(LCD_ENTRY_MODE_SET);


    // 8. Display ON, Cursor ON
    LCD_SendCommand(LCD_DISPLAY_ON_CURSOR_ON);

}

/*
* @brief This function sends a command to the LCD controller by setting the appropriate control signals and data lines.
* @param cmd: The command byte to be sent to the LCD controller
* @return None
*/
void LCD_SendCommand(uint8_t cmd){
    // 1. Set RS pin low for command mode
    GPIO_WriteToOutputPin(LCD_RS_PORT, LCD_RS_PIN_NO, GPIO_PIN_RESET);

    // 2. Set RW pin low for write mode
    GPIO_WriteToOutputPin(LCD_RW_PORT, LCD_RW_PIN_NO, GPIO_PIN_RESET);

    // 3. Send higher nibble of command
    write_4bits(cmd >> 4,5);

    // 4. Send lower nibble of command
    write_4bits(cmd & 0x0F,5);
}


/*
* @brief This function sets the DDRAM address in the LCD controller, which determines the position where the next data will be written on the LCD display.
* @param address: The DDRAM address to set (0x00 to 0x7F for 16x2 LCD)
* @return None 
*/
void LCD_Ddram_SetAddress(uint8_t address){
    // 1. Set RS pin low for command mode
    GPIO_WriteToOutputPin(LCD_RS_PORT, LCD_RS_PIN_NO, GPIO_PIN_RESET);

    // 2. Set RW pin low for write mode
    GPIO_WriteToOutputPin(LCD_RW_PORT, LCD_RW_PIN_NO, GPIO_PIN_RESET);

    // 3. Set 1 to 7th bits of command byte to set DDRAM address
    address = address | (1 << 7);

    // 4. Send higher nibble of command
    write_4bits(address >> 4,5);

    // 5. Send lower nibble of command
    write_4bits(address & 0x0F,5);

}

/*
* @brief This function writes a data byte to the LCD at the current DDRAM address, which will be displayed on the LCD screen.
* @param data: The data byte to be written to the LCD (ASCII value of the character to be displayed)
* @return None
*/
void LCD_Ddram_SendData(uint8_t data){
    // 1. Set RS pin high for data mode
    GPIO_WriteToOutputPin(LCD_RS_PORT, LCD_RS_PIN_NO, GPIO_PIN_SET);

    // 2. Set RW pin low for write mode
    GPIO_WriteToOutputPin(LCD_RW_PORT, LCD_RW_PIN_NO, GPIO_PIN_RESET);

    // 3. Send higher nibble of data
    write_4bits(data >> 4,5);

    // 4. Send lower nibble of data
    write_4bits(data & 0x0F,5);

}

/*
* @brief This function sends a null-terminated string to the LCD by writing each character to
* the LCD using the LCD_Ddram_SendData function. The string will be displayed on the LCD screen starting from the current DDRAM address.
* @param str: Pointer to the null-terminated string to be displayed on the LCD
* @return None
*/
void LCD_SendString(char *str){
    uint8_t i = 0;
    while(*str){
        LCD_Ddram_SendData(*str++);
        i++;
    }
}