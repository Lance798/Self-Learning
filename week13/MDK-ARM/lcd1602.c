#include <stm32l0xx_hal.h>
#include <stdio.h>
#include <main.h>

#define LCD_ADDRESS 0x27<<1

extern I2C_HandleTypeDef hi2c1;

void lcd_init(void);
void lcd_send_cmd(uint8_t);
void lcd_send_char(char);
void lcd_print(char*, int);
void lcd_set_cursor(int, int);
void lcd_clear(void);

void lcd_init() {
	HAL_Delay(15);
	lcd_send_cmd(0x30);
	HAL_Delay(5); 
	lcd_send_cmd(0x30);
	HAL_Delay(1); 
	lcd_send_cmd(0x30);
	HAL_Delay(1);
	lcd_send_cmd(0x20);
	HAL_Delay(1);
	lcd_send_cmd(0x28);
	HAL_Delay(1);
	lcd_send_cmd(0x0F);
	HAL_Delay(1);
	lcd_send_cmd(0x01);
	HAL_Delay(1);
	lcd_send_cmd(0x06); 
	HAL_Delay(1);
}

void lcd_send_cmd(uint8_t cmd) {
  uint8_t cmds[4];
  cmds[0] = (cmd&0xF0) | 0x0C;
  cmds[1] = (cmd&0xF0) | 0x08;
  cmds[2] = (uint8_t)(cmd<<4) | 0x0C;
  cmds[3] = (uint8_t)(cmd<<4) | 0x08;
  if (HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, cmds, 4, 1000) != HAL_OK)
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void lcd_send_char(char c) {
  uint8_t cmds[4];
  cmds[0] = (c&0xF0) | 0x0D;
  cmds[1] = (c&0xF0) | 0x09;
  cmds[2] = (uint8_t)(c<<4) | 0x0D;
  cmds[3] = (uint8_t)(c<<4) | 0x09;
  if (HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, cmds, 4, 1000) != HAL_OK)
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void lcd_print(char* str, int len) {
  for(int i = 0; i<len; i++) 
    lcd_send_char(*(str+i));
}

void lcd_set_cursor(int column, int row) {
  lcd_send_cmd((row*0x40+column)+0x80);
  HAL_Delay(1);
}

void lcd_clear() {
  lcd_send_cmd(0x01);
  HAL_Delay(1);
}
