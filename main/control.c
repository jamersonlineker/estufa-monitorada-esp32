#include "control.h"     // Biblioteca com definições de controle do sistema
#include "sensors.h"     // Biblioteca com variáveis e funções dos sensores

#include "driver/gpio.h" // Biblioteca do ESP-IDF para controle dos GPIOs

// ================= CONFIGURAÇÕES =================

// Valor máximo permitido para temperatura
// Recebe valor padrão definido no menuconfig
int temp_max = CONFIG_TEMP_MAX_DEFAULT;

// Valor mínimo permitido para umidade
int umid_min = CONFIG_UMID_MIN_DEFAULT;

// Valor mínimo permitido para luminosidade
int luz_min = CONFIG_LUZ_MIN_DEFAULT;

// ================= CONTROLE =================

// Variável responsável por habilitar/desabilitar o sistema
bool system_enable = true;

// ======================================================
// FUNÇÃO: init_leds
// OBJETIVO:
// Configurar os LEDs RGB como saída digital
// ======================================================

void init_leds() {

  // Estrutura de configuração dos GPIOs
  gpio_config_t leds_io_conf = {

      // Define os pinos utilizados pelos LEDs
      .pin_bit_mask = (1ULL << LED_R) | (1ULL << LED_G) | (1ULL << LED_B),

      // Configura os pinos como saída
      .mode = GPIO_MODE_OUTPUT,

      // Desabilita resistor de pull-up
      .pull_up_en = GPIO_PULLUP_DISABLE,

      // Desabilita resistor de pull-down
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
  };

  // Aplica a configuração nos GPIOs
  gpio_config(&leds_io_conf);
}

// ======================================================
// FUNÇÃO: update_control
// OBJETIVO:
// Atualizar o estado dos LEDs conforme os sensores
// ======================================================

void update_control() {

  // Verifica se o sistema está habilitado
  if (system_enable) {

    // ================= TEMPERATURA =================
    // Se a temperatura for maior ou igual ao limite,
    // acende o LED vermelho
    if (temperatura >= temp_max) {

      gpio_set_level(LED_R, 1);

    } else {

      // Caso contrário, apaga o LED vermelho
      gpio_set_level(LED_R, 0);
    }

    // ================= UMIDADE =================
    // Se a umidade estiver abaixo do limite mínimo,
    // acende o LED verde
    if (umidade <= umid_min) {

      gpio_set_level(LED_G, 1);

    } else {

      // Caso contrário, apaga o LED verde
      gpio_set_level(LED_G, 0);
    }

    // ================= LUMINOSIDADE =================
    // Se a luminosidade estiver abaixo do limite mínimo,
    // acende o LED azul
    if (luminosidade <= luz_min) {

      gpio_set_level(LED_B, 1);

    } else {

      // Caso contrário, apaga o LED azul
      gpio_set_level(LED_B, 0);
    }

  } else {

    // Caso o sistema esteja desabilitado,
    // todos os LEDs são apagados

    gpio_set_level(LED_B, 0);
    gpio_set_level(LED_R, 0);
    gpio_set_level(LED_G, 0);
  }
}