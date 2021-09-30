#include "stm32l0xx_hal.h"
#include "main.h"
#include <stdio.h>

#define LCD_ADDRESS 0x27<<1

extern I2C_HandleTypeDef hi2c1;

void lcd_send_cmd(uint8_t cmd) {
  uint8_t cmds[4];
  cmds[0] = (cmd&0xF0) | 0x0C;
  cmds[1] = (cmd&0xF0) | 0x08;
  cmds[2] = (cmd<<4) | 0x0C;
  cmds[3] = (cmd<<4) | 0x08;
  if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, cmds, 4, 100) != HAL_OK)
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}

void lcd_send_data(uint8_t data) {
  uint8_t cmds[4];
  cmds[0] = (data&0xF0) | 0x0D;
  cmds[1] = (data&0xF0) | 0x09;
  cmds[2] = (data<<4) | 0x0D;
  cmds[3] = (data<<4) | 0x09;
  if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, cmds, 4, 100) != HAL_OK)
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}

void lcd_init() {
	HAL_Delay(50); 
	lcd_send_cmd(0x30);
	HAL_Delay(5); 
	lcd_send_cmd(0x30);
	HAL_Delay(1);
	lcd_send_cmd(0x30);
	HAL_Delay(10);
	lcd_send_cmd(0x20);
	HAL_Delay(10);
	lcd_send_cmd(0x28);
	HAL_Delay(1);
	lcd_send_cmd(0x08); 
	HAL_Delay(1);
	lcd_send_cmd(0x01);
	HAL_Delay(1);
	lcd_send_cmd(0x06);
	HAL_Delay(1);
	lcd_send_cmd(0x0C);
	HAL_Delay(1);
}

void lcd_print(char *str, uint8_t len) {
  for(uint8_t i = 0; i<len; i++)
    lcd_send_data(*(str+i));
}

void lcd_clear() {
  lcd_send_cmd(0x01);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  lcd_send_cmd(0x80|(40*row+col));
}
