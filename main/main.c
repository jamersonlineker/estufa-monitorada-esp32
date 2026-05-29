#include <stdio.h>
#include <string.h>

// Arquivos do projeto

#include "control.h"
#include "display.h"
#include "menu.h"
#include "sensors.h"
#include "storage.h"

// Drivers UART do ESP-IDF
#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "esp_err.h"

// Sistema de arquivos virtual
#include "esp_vfs_dev.h"

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Driver GPIO
#include "driver/gpio.h"

// ===================== PINOS =====================

#define BUTTON_UP 18
#define BUTTON_DOWN 19
#define BUTTON_SELECT 23




// Buffer auxiliar para strings
char line[32];


// Buffer para comunicação serial
char serial_buffer[64];

// Contador usado para determinar quando salvar logs

int log_counter = 0;


// Flags auxiliares de limpeza de display
bool flag_limpar = false;
bool flag_limpar_draw = false;

// Habilita/desabilita gravação de logs
bool log_enable = true;

// ===================================================
// Inicialização dos botões
// ===================================================
void init_buttons() {
  gpio_config_t io_conf = {
	  // Define os pinos utilizados, habilita e desabilita Pull-up Pull-down
      .pin_bit_mask = (1ULL << BUTTON_UP) | (1ULL << BUTTON_DOWN) |
                      (1ULL << BUTTON_SELECT),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
  };
  // Aplica configuração
  gpio_config(&io_conf);
}

// ===================================================
// Função principal
// ===================================================
void app_main(void) {

  // Inicializa driver UART
  uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

  // Vincula UART ao VFS
  uart_vfs_dev_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

   // Inicialização dos módulos do sistema
  init_display();   // Display OLED
  init_ldr();       // Sensor LDR
  init_buttons();   // Botões
  init_leds();      // LEDs
  init_spiffs();    // Sistema de arquivos SPIFFS

   // Loop principal
  while (1) {

    // Atualiza leitura dos botões/controle
    update_control();

    // Lê luminosidade do LDR
    luminosidade = read_ldr();

    // Atualiza navegação do menu
    update_menu();

    // Lê sensor DHT
    esp_err_t result = read_dht();

    // Verifica troca de tela
    if (last_screen != screen) {

      // Limpa display ao mudar de tela
      clear_display();

      // Atualiza última tela
      last_screen = screen;
    }

    // ===================================================
    // Caso leitura dos sensores esteja OK
    // ===================================================
    if (result == ESP_OK && luminosidade)

    {
      // Tela principal
      if (screen == SCREEN_HOME) {
        draw_home();
      }

      // Tela de menu
      else if (screen == SCREEN_MENU) {
        draw_menu();
      }

      // Tela de edição
      else if (screen == SCREEN_EDIT) {
        draw_edit();

      }

      // Tela de configurações
      else if (screen == SCREEN_CONFIG) {
        draw_config();
      }

      // Marca que o display foi atualizado
      flag_limpar = true;

    }

    // ===================================================
    // Tratamento de erros
    // ===================================================
    else {

      // Limpa display apenas uma vez
      if (flag_limpar == true) {
        clear_display();
      }

      // Erro apenas no DHT
      if (result != ESP_OK && luminosidade) {

        ssd1306_display_text(&dev, 0, "Erro Leitura", 13, false);
        ssd1306_display_text(&dev, 2, "Verificar DHT", 13, false);
      }

      // Erro apenas no LDR
      if (result == ESP_OK && !luminosidade) {

        ssd1306_display_text(&dev, 0, "Erro Leitura", 13, false);
        ssd1306_display_text(&dev, 2, "Verificar LDR", 13, false);
      }

      // Erro nos dois sensores
      if (result != ESP_OK && !luminosidade) {

        ssd1306_display_text(&dev, 0, "Erro Leitura", 13, false);
        ssd1306_display_text(&dev, 2, "Verificar LDR", 13, false);
        ssd1306_display_text(&dev, 4, "Verificar DHT", 13, false);
      }

      // Evita limpar repetidamente
      flag_limpar = false;
    }

    // ===================================================
    // Controle de logs
    // ===================================================

    // Incrementa contador
    log_counter++;

    // A cada 50 ciclos
    if (log_counter >= 50) {

      // Reinicia contador
      log_counter = 0;

      // Se log estiver habilitado
      if (log_enable) {

        // Salva log no SPIFFS
        save_log();
      }
    }

    // ===================================================
    // Comunicação serial
    // ===================================================

    // Buffer de comando
    static char cmd_buffer[64];

    // Posição atual do buffer
    static int cmd_pos = 0;

    // Variável para receber byte UART
    uint8_t c;

    // Lê 1 byte da UART
    int len = uart_read_bytes(UART_NUM_0,
                              &c,
                              1,
                              pdMS_TO_TICKS(10));

    // Se recebeu dado
    if (len > 0) {

      // Se Enter foi pressionado
      if (c == '\r' || c == '\n') {

        // Finaliza string
        cmd_buffer[cmd_pos] = '\0';

        // Exibe comando recebido
        printf("\nComando: [%s]\n", cmd_buffer);

        // ===================================================
        // Comando: readlog
        // ===================================================
        if (strcmp(cmd_buffer, "readlog") == 0) {

          printf("Executando readlog\n");

          // Lê arquivo de log
          read_log_file();

        }

        // ===================================================
        // Comando: clearlog
        // ===================================================
        else if (strcmp(cmd_buffer, "clearlog") == 0) {

          // Limpa arquivo de log
          clear_log_file();
        }

        // Reinicia posição do buffer
        cmd_pos = 0;

      }

      // Caso caractere comum
      else {

        // Evita overflow
        if (cmd_pos < sizeof(cmd_buffer) - 1) {

          // Adiciona caractere ao buffer
          cmd_buffer[cmd_pos++] = c;

          
          printf("%c", c);
        }
      }
    }

    // Delay de 100 ms
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}