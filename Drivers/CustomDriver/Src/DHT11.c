#include "DHT11.h"
#include "stm32f4xx_hal.h"

// DHT11 driver for STM32F4xx
// This driver is designed to work with the HAL library.
// It provides functions to initialize the DHT11 sensor, read temperature and humidity data, and convert temperature to Fahrenheit.
// Datasheet: https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf

// Written By: Jaycy Ivan A. Bañaga
// Date: 2025-05-10

void HAL_DHT11_Init(DHT11_InitTypeDef *DHT11, GPIO_TypeDef *GPIOx, uint8_t pin, TIM_HandleTypeDef *htim) {
    DHT11->_Port = GPIOx;
    DHT11->_Pin = pin;
    DHT11->_htim = htim;
  
    HAL_TIM_Base_Start(DHT11->_htim);
}

void DHT_SetPinMode(DHT11_InitTypeDef *DHT11, uint8_t mode) {
    GPIO_InitTypeDef GPIO_CONFIG = {
        .Pin = DHT11->_Pin,
        .Mode = mode ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };

    HAL_GPIO_Init(DHT11->_Port, &GPIO_CONFIG);
}

int8_t HAL_DHT11_ReadTemperature(DHT11_InitTypeDef *DHT11) {
    return DHT11->temperature;
}

int8_t HAL_DHT11_ReadHumidity(DHT11_InitTypeDef *DHT11) {
    return DHT11->humidity;
}

int8_t HAL_DHT11_ReadTemperatureF(DHT11_InitTypeDef *DHT11) {
    return (int8_t)(DHT11->temperature * 9.0f / 5.0f + 32.0f);
}

uint16_t HAL_DHT11_ReadData(DHT11_InitTypeDef *DHT11) {
    uint8_t data[5] = {0,0,0,0,0};
    uint8_t checksum;

    DHT_SetPinMode(DHT11, 0);
    HAL_GPIO_WritePin(DHT11->_Port, DHT11->_Pin, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11->_Port, DHT11->_Pin, GPIO_PIN_SET);
    delay_us(40, DHT11->_htim);
    DHT_SetPinMode(DHT11, 1);

    if(HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_RESET) {
        while(HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_RESET);
        while(HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_SET);

        for (uint8_t i = 0; i < 5; i++) {
            for (uint8_t j = 0; j < 8; j++) {
                while(HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_RESET);
                delay_us(50, DHT11->_htim);

                if (HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_SET) {
                    data[i] |= (0 << (7 - j));
                    delay_us(28, DHT11->_htim);
                    if (HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_SET) {
                        data[i] |= (1 << (7 - j));
                    }
                    while(HAL_GPIO_ReadPin(DHT11->_Port, DHT11->_Pin) == GPIO_PIN_SET);
                }   
            }
        }
        checksum = data[0] + data[1] + data[2] + data[3];
        if (checksum == data[4]) {
            DHT11->humidity = data[0];
            DHT11->temperature = data[2];
            return DHT11_OK;
        }
    }
    return DHT11_ERROR;
}

void delay_us(uint8_t delay, TIM_HandleTypeDef *htim) {
    __HAL_TIM_SET_COUNTER(htim, 0);
    while (__HAL_TIM_GET_COUNTER(htim) < delay);
}
