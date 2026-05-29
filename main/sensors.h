#ifndef SENSORS_H
#define SENSORS_H

#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

// ===== variáveis globais =====

extern float temperatura;
extern float umidade;
extern int luminosidade;

// ===== ADC =====

extern adc_oneshot_unit_handle_t adc1_handle;

// ===== funções =====

void init_ldr();

int read_ldr();

esp_err_t read_dht();

#endif