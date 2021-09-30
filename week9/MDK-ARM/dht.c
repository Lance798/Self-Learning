#include "stm32l0xx_hal.h"
#include "main.h"

#define DHT_PIN_IN dht_set_pinMode(GPIO_MODE_INPUT)
#define DHT_PIN_OUT dht_set_pinMode(GPIO_MODE_OUTPUT_PP)

int8_t DataBuf[5];
extern void delay_us(uint16_t);

void dht_set_pinMode(uint32_t mode) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT_Pin;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  if (mode == GPIO_MODE_OUTPUT_PP)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);
}

void dht_start() {
  HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET);
  HAL_Delay(18);
  HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET);
  delay_us(30);
}

HAL_StatusTypeDef dht_ack() {
  if (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin))
    return HAL_ERROR;
  while(HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_RESET);
  while(HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET);
  return HAL_OK;
}

int8_t dht_read_byte() { 
  int8_t i, data = 0;
  for(i=0;i<8;i++) {
    data <<= 1;
    while(!HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin));
    delay_us(40);
    if(HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET) {
      data |=1;
      while(HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin));
    }
  }
  return data;
}

uint8_t dht_read() {
  DHT_PIN_OUT;
  dht_start();
  DHT_PIN_IN;
  dht_ack();
  for(int8_t i = 0; i<5; i++)
    DataBuf[i] = dht_read_byte();

  return DataBuf[0] + DataBuf[1] + DataBuf[2] + DataBuf[3] == DataBuf[4];
}

float dht_readTemperature() {
  return ((DataBuf[2] << 8 ) + DataBuf[3])/10.0;
}


float dht_readHumidity() {
  return ((DataBuf[0]<<8) + DataBuf[1])/10.0;
}
