#include "stm32l0xx_hal.h"
#include "main.h"

#define SSD1306_ADDRESS 0x78
#define ssd1306_send_cmd(command) ssd1306_send(0x00, command)

#define OLED_WIDTH 128
#define OLED_HEIGHT 64


extern I2C_HandleTypeDef hi2c1;
uint8_t ScreenBuf[OLED_WIDTH * OLED_HEIGHT / 8];

HAL_StatusTypeDef ssd1306_send(uint8_t reg, uint8_t cmd) { 
  uint8_t temp[2] = {reg, cmd};
  return HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDRESS, temp, 2, 10);
}

void ssd1306_send_cmds(uint8_t reg, uint8_t *cmds, uint8_t len) {
  uint8_t temp[len+1];
  temp[0] = reg;
  for(uint8_t i = 0; i<len; i++)
    temp[i+1] = *(cmds+i);
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDRESS, temp, len+1, 1000);
}

void oled_init() {
  HAL_Delay(100);
  uint8_t init_cmds[] = {
    0xAE, 0x20, 0x10, 0xB0, 0xC8, 0x00, 0x10,
    0x40, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
    0xA4, 0xD3, 0x00, 0xD5, 0xF0, 0xD9, 0x22,
    0xDA, 0x12, 0xDB, 0x20, 0x8D, 0x14, 0xAF};
  for(uint8_t i = 0; i<sizeof(init_cmds)/sizeof(uint8_t); i++)
    if (ssd1306_send_cmd(init_cmds[i])!= HAL_OK)
      while(1);
  for(uint16_t i = 0; i < OLED_WIDTH * OLED_HEIGHT / 8; i++)
    ScreenBuf[i] = 0;
}

void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t status) {
  if (status)
    ScreenBuf[x + y/8*OLED_WIDTH] |= 1<<(y%8);
  else
    ScreenBuf[x + y/8*OLED_WIDTH] &= ~(1<<(y%8));
}

void oled_update() {
  for(uint8_t page = 0; page<8; page++) {
    ssd1306_send_cmd(0xB0+page);
    ssd1306_send_cmd(0x00);
    ssd1306_send_cmd(0x10);
    ssd1306_send_cmds(0x40, &ScreenBuf[page*OLED_WIDTH], 128);
  }
}

void oled_clear() {
  for(uint16_t i = 0; i < OLED_WIDTH*OLED_HEIGHT/8; i++)
    ScreenBuf[i] = 0;
}

void oled_draw_bitmap(uint8_t x, uint8_t y, uint8_t *map, uint8_t w, uint8_t h) {
  uint16_t a = 0;
  for(uint8_t i = y; i<h+y; i++)
    for(uint8_t j = x; j<w+x; j++) {
      ScreenBuf[j+i*OLED_WIDTH] = map[a];
      a++;
    }
}
