#include <stdbool.h> // Tipo booleano
#include <stdio.h>   // Biblioteca padrão de entrada e saída
#include <string.h>  // Manipulação de strings

// Biblioteca do display OLED
#include "display.h"

// Driver I2C do ESP-IDF
#include "driver/i2c.h"



// Estrutura usada pela biblioteca do display
SSD1306_t dev;

// ==========================================================
// VARIÁVEIS EXTERNAS
// ==========================================================

// Valores lidos dos sensores
extern float temperatura;
extern float umidade;
extern int luminosidade;

// Índice atual do menu
extern int menu_index;

// Controle para tela de editar valor
extern bool editing_value;

// Configurações ajustáveis do sistema
extern int temp_max;
extern int umid_min;
extern int luz_min;

// Estados gerais do sistema
extern bool system_enable;
extern bool log_enable;

// Flag usada para controlar limpeza do display
extern bool flag_limpar_draw;

// ==========================================================
// INICIALIZAÇÃO DO DISPLAY
// ==========================================================

void init_display() {

  // Inicializa barramento I2C
  i2c_master_init(&dev, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, -1);

  // Inicializa display SSD1306
  ssd1306_init(&dev, 128, 64);

  // Limpa display apenas uma vez
  ssd1306_clear_screen(&dev, false);
}

// ==========================================================
// TELA HOME
// ==========================================================

void draw_home() {

  // Buffer auxiliar
  char line[32];

  // Exibe temperatura
  sprintf(line, "Temp: %.1f C  ", temperatura);

  ssd1306_display_text(&dev, 0, line, strlen(line), false);

  // Exibe umidade
  sprintf(line, "Umid: %.1f %%  ", umidade);

  ssd1306_display_text(&dev, 2, line, strlen(line), false);

  // Exibe luminosidade
  sprintf(line, "Luz : %d   ", luminosidade);

  ssd1306_display_text(&dev, 4, line, strlen(line), false);

  // Instrução para abrir menu
  ssd1306_display_text(&dev, 7, "SELECT = MENU", strlen("SELECT = MENU"),
                       false);
}

// ==========================================================
// TELA MENU PRINCIPAL
// ==========================================================

void draw_menu() {

  char line[32];

  // ======================================================
  // ITEM 1 - EDIT SENSOR
  // ======================================================

  // Adiciona ">" caso esteja selecionado
  sprintf(line, "%s Edit sensor", (menu_index == 0) ? ">" : " ");

  ssd1306_display_text(&dev, 0, line, strlen(line), false);

  // ======================================================
  // ITEM 2 - CONFIG
  // ======================================================

  sprintf(line, "%s Config", (menu_index == 1) ? ">" : " ");

  ssd1306_display_text(&dev, 2, line, strlen(line), false);

  // ======================================================
  // ITEM 3 - SAIR
  // ======================================================

  sprintf(line, "%s Sair", (menu_index == 2) ? ">" : " ");

  ssd1306_display_text(&dev, 4, line, strlen(line), false);
}

// ==========================================================
// TELA DE EDIÇÃO DOS LIMITES
// ==========================================================

void draw_edit() {

  char line[32];

  // ======================================================
  // MODO LISTA
  // ======================================================

  // Usuário navegando entre opções
  if (editing_value == false) {

    // Limpa display apenas quando necessário
    if (flag_limpar_draw == false) {
      ssd1306_clear_screen(&dev, false);
    }

    // ===================================================
    // TEMP MAX
    // ===================================================

    sprintf(line, "%s Temp Max", (menu_index == 0) ? ">" : " ");

    ssd1306_display_text(&dev, 0, line, strlen(line), false);

    // ===================================================
    // UMID MIN
    // ===================================================

    sprintf(line, "%s Umid Min", (menu_index == 1) ? ">" : " ");

    ssd1306_display_text(&dev, 2, line, strlen(line), false);

    // ===================================================
    // LUZ MIN
    // ===================================================

    sprintf(line, "%s Luz Min", (menu_index == 2) ? ">" : " ");

    ssd1306_display_text(&dev, 4, line, strlen(line), false);

    // ===================================================
    // SAIR
    // ===================================================

    sprintf(line, "%s Sair", (menu_index == 3) ? ">" : " ");

    ssd1306_display_text(&dev, 6, line, strlen(line), false);

    // Marca que display já foi limpo
    flag_limpar_draw = true;
  }

  // ======================================================
  // MODO EDIÇÃO
  // ======================================================

  else {

    // Limpa display ao entrar na edição
    if (flag_limpar_draw == true) {
      ssd1306_clear_screen(&dev, false);
    }

    // ===================================================
    // EDITANDO TEMP MAX
    // ===================================================

    if (menu_index == 0) {

      ssd1306_display_text(&dev, 0, "TEMP MAX", strlen("TEMP MAX"), false);

      sprintf(line, "%d C", temp_max);

      ssd1306_display_text(&dev, 3, line, strlen(line), false);
    }

    // ===================================================
    // EDITANDO UMID MIN
    // ===================================================

    else if (menu_index == 1) {

      ssd1306_display_text(&dev, 0, "UMID MIN", strlen("UMID MIN"), false);

      sprintf(line, "%d %%", umid_min);

      ssd1306_display_text(&dev, 3, line, strlen(line), false);
    }

    // ===================================================
    // EDITANDO LUZ MIN
    // ===================================================

    else if (menu_index == 2) {

      ssd1306_display_text(&dev, 0, "LUZ MIN", strlen("LUZ MIN"), false);

      sprintf(line, "%d", luz_min);

      ssd1306_display_text(&dev, 3, line, strlen(line), false);
    }

    // Indica que tela de edição já foi desenhada
    flag_limpar_draw = false;
  }
}

// ==========================================================
// TELA CONFIGURAÇÕES
// ==========================================================

void draw_config() {

  char line[32];

  // ======================================================
  // STATUS DO SISTEMA
  // ======================================================

  sprintf(line, "%s Sistema: %s ", (menu_index == 0) ? ">" : " ",
          system_enable ? "ON" : "OFF");

  ssd1306_display_text(&dev, 0, line, strlen(line), false);

  // ======================================================
  // STATUS DO LOG
  // ======================================================

  sprintf(line, "%s Log: %s ", (menu_index == 1) ? ">" : " ",
          log_enable ? "ON" : "OFF");

  ssd1306_display_text(&dev, 2, line, strlen(line), false);

  // ======================================================
  // OPÇÃO SAIR
  // ======================================================

  sprintf(line, "%s Sair", (menu_index == 2) ? ">" : " ");

  ssd1306_display_text(&dev, 4, line, strlen(line), false);
}

// ==========================================================
// LIMPA DISPLAY
// ==========================================================

void clear_display() {

  // Limpa completamente a tela OLED
  ssd1306_clear_screen(&dev, false);
}