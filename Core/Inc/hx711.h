#ifndef HX711_H
#define HX711_H

#include "stm32f3xx_hal.h"
#include "stdbool.h"

// Definisci i pin e il port utilizzati per il HX711
#define HX711_SCK_PIN       GPIO_PIN_8
#define HX711_SCK_PORT      GPIOA
#define HX711_DT_PIN        GPIO_PIN_1
#define HX711_DT_PORT       GPIOA

// Dichiarazione delle funzioni
void HX711_Init(void);
int32_t HX711_Read(void);
void HX711_SetGain(uint8_t gain);
bool HX711_IsReady(void);

#endif // HX711_H
