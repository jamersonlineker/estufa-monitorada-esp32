#include <stdio.h> // Biblioteca padrão de entrada e saída

#include "sensors.h"              // Biblioteca com definições dos sensores
#include "esp_adc/adc_oneshot.h" // Biblioteca ADC modo one-shot do ESP-IDF

#include "driver/gpio.h" // Biblioteca para controle dos GPIOs
#include "dht.h"         // Biblioteca do sensor DHT11/DHT22

// ================= PINOS =================

// Define o pino do sensor LDR via menuconfig
#define LDR CONFIG_LDR_GPIO

// Define o pino do sensor DHT via menuconfig
#define DHT_PIN CONFIG_DHT_GPIO

// ================= VARIÁVEIS =================

// Variável global para armazenar temperatura
float temperatura = 0;

// Variável global para armazenar umidade
float umidade = 0;

// Variável global para armazenar luminosidade
int luminosidade = 0;

// Handle da unidade ADC utilizada
adc_oneshot_unit_handle_t adc1_handle;

// ======================================================
// FUNÇÃO: init_ldr
// OBJETIVO:
// Inicializar o sensor LDR utilizando ADC
// ======================================================

void init_ldr() {

  // Configuração do pino do LDR
  gpio_config_t io_conf = {

      // Define o pino utilizado pelo LDR
      .pin_bit_mask = (1ULL << LDR),

      // Configura o pino como entrada
      .mode = GPIO_MODE_INPUT,
  };

  // Aplica configuração do GPIO
  gpio_config(&io_conf);

  // ================= CONFIGURAÇÃO ADC =================

  // Estrutura de inicialização do ADC
  adc_oneshot_unit_init_cfg_t init_config = {

      // Utiliza ADC1 do ESP32
      .unit_id = ADC_UNIT_1,
  };

  // Cria nova unidade ADC
  adc_oneshot_new_unit(&init_config, &adc1_handle);

  // ================= CONFIGURAÇÃO DO CANAL =================

  adc_oneshot_chan_cfg_t config = {

      // Usa resolução padrão do ADC
      .bitwidth = ADC_BITWIDTH_DEFAULT,

      // Atenuação de 12 dB
      // Permite leitura de tensões maiores
      .atten = ADC_ATTEN_DB_12,
  };

  // Configura canal ADC do LDR
  adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config);
}

// ======================================================
// FUNÇÃO: read_ldr
// OBJETIVO:
// Ler valor analógico do sensor LDR
// ======================================================

int read_ldr() {

  // Variável local para armazenar leitura
  int valor = 0;

  // Realiza leitura do ADC
  adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &valor);

  // Retorna valor lido
  return valor;
}

// ======================================================
// FUNÇÃO: read_dht
// OBJETIVO:
// Ler temperatura e umidade do sensor DHT11
// ======================================================

esp_err_t read_dht() {

    // Realiza leitura do sensor DHT11
    // Os valores são armazenados diretamente
    // nas variáveis globais temperatura e umidade

    return dht_read_float_data(

        // Tipo do sensor
        DHT_TYPE_DHT11,

        // Pino conectado ao DHT
        DHT_PIN,

        // Ponteiro da variável umidade
        &umidade,

        // Ponteiro da variável temperatura
        &temperatura
    );
}