#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stm32f4xx_hal_gpio.h>

#define DHT11_OK 0
#define DHT11_ERROR 1

typedef struct {
    GPIO_TypeDef *_Port;
    uint8_t _Pin;
    TIM_HandleTypeDef *_htim;
    float temperature;
    float humidity;

} DHT11_InitTypeDef;

void HAL_DHT11_Init(DHT11_InitTypeDef *DHT11, GPIO_TypeDef *GPIOx, uint8_t pin, TIM_HandleTypeDef *htim);
void DHT_SetPinMode(DHT11_InitTypeDef *DHT11, uint8_t mode);
void delay_us(uint8_t delay, TIM_HandleTypeDef *htim);
int8_t HAL_DHT11_ReadTemperature(DHT11_InitTypeDef *DHT11);
int8_t HAL_DHT11_ReadHumidity(DHT11_InitTypeDef *DHT11);
int8_t HAL_DHT11_ReadTemperatureF(DHT11_InitTypeDef *DHT11);
uint16_t HAL_DHT11_ReadData(DHT11_InitTypeDef *DHT11);


#endif