#include "hx711.h"

static uint8_t GAIN = 128;

void HX711_Init(void) {
   /*
	// Configura i pin SCK e DT
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configura il pin SCK come output
    GPIO_InitStruct.Pin = HX711_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_PORT, &GPIO_InitStruct);

    // Configura il pin DT come input
    GPIO_InitStruct.Pin = HX711_DT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HX711_DT_PORT, &GPIO_InitStruct);
*/
    // Inizializza il pin SCK a basso
    HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(HX711_DT_PORT, HX711_DT_PIN, GPIO_PIN_RESET);
}

bool HX711_IsReady(void) {
    // Controlla se il DT è basso, indicando che il HX711 è pronto
	if( GPIO_PIN_RESET == HAL_GPIO_ReadPin(HX711_DT_PORT, HX711_DT_PIN))
		return true;
	else
		return false;
}

void HX711_SetGain(uint8_t gain) {
    switch (gain) {
        case 128: // channel A, gain factor 128
            GAIN = 1;
            break;
        case 64:  // channel A, gain factor 64
            GAIN = 3;
            break;
        case 32:  // channel B, gain factor 32
            GAIN = 2;
            break;
        default:  // default to channel A, gain factor 128
            GAIN = 1;
            break;
    }
}

int32_t HX711_Read(void) {
    // Attendi che il HX711 sia pronto
	bool passo = HX711_IsReady();
    while (passo == false){};

    int32_t count = 0;
    uint32_t value = 0;

    // Lettura dei 24 bit di dati dal HX711
    for (int i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
        value = value << 1;
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
        if (HAL_GPIO_ReadPin(HX711_DT_PORT, HX711_DT_PIN) == GPIO_PIN_SET) {
            value++;
        }
    }

    // Imposta il gain per il prossimo ciclo
    for (int i = 0; i < GAIN; i++) {
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, GPIO_PIN_RESET);
    }

    // Converti il valore letto in un int32_t
    if (value & 0x800000) {
        value |= 0xFF000000; // Se il valore è negativo
    } else {
        value &= 0x00FFFFFF; // Se il valore è positivo
    }
    count = (int32_t)value;

    return count;
}
