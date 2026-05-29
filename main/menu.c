#include <stdbool.h> // Biblioteca para uso do tipo booleano (true/false)

#include "menu.h" // Biblioteca com definições do menu e telas

#include "driver/gpio.h"       // Biblioteca para leitura dos GPIOs
#include "freertos/FreeRTOS.h" // Biblioteca principal do FreeRTOS
#include "freertos/task.h"     // Biblioteca para tarefas e delays



// ================= VARIÁVEIS =================

// Tela atual do sistema
screen_status_t screen = SCREEN_HOME;

// Armazena a última tela exibida
screen_status_t last_screen = -1;

// Índice atual do menu selecionado
int menu_index = 0;

// Variável que indica se o usuário está editando um valor
bool editing_value = false;

// Variáveis globais vindas de outros arquivos
extern int temp_max;
extern int umid_min;
extern int luz_min;

// Variáveis globais de controle do sistema
extern bool system_enable;
extern bool log_enable;

// ======================================================
// FUNÇÃO: button_pressed
// OBJETIVO:
// Verificar se um botão foi pressionado
// Implementa debounce simples com delay
// ======================================================

bool button_pressed(int pin) {

  // Verifica se o botão está pressionado
  if (gpio_get_level(pin) == 1) {

    // Pequeno delay para debounce
    vTaskDelay(pdMS_TO_TICKS(100));

    // Confirma se o botão continua pressionado
    if (gpio_get_level(pin) == 1) {
      return true;
    }
  }

  return false;
}

// ======================================================
// FUNÇÃO: update_menu
// OBJETIVO:
// Atualizar navegação e ações do menu
// ======================================================

void update_menu() {

  // ================= HOME =================
  // Tela inicial do sistema

  if (screen == SCREEN_HOME) {

    // Ao pressionar SELECT entra no menu principal
    if (button_pressed(BUTTON_SELECT)) {
      screen = SCREEN_MENU;
    }
  }

  // ================= MENU =================
  // Menu principal

  else if (screen == SCREEN_MENU) {

    // Navegação para cima
    if (button_pressed(BUTTON_UP)) {

      menu_index--;

      // Retorna para o último item caso passe do início
      if (menu_index < 0) {
        menu_index = 2;
      }
    }

    // Navegação para baixo
    if (button_pressed(BUTTON_DOWN)) {

      menu_index++;

      // Retorna para o primeiro item caso passe do final
      if (menu_index > 2) {
        menu_index = 0;
      }
    }

    // Seleção do item do menu
    if (button_pressed(BUTTON_SELECT)) {

      // Abre tela de configurações
      if (menu_index == 1) {

        screen = SCREEN_CONFIG;

      }

      // Volta para tela HOME
      else if (menu_index == 2)

      {
        screen = SCREEN_HOME;

      }

      // Abre tela de edição
      else {
        screen = SCREEN_EDIT;
      }
    }
  }

  // ================= EDIT =================
  // Tela de edição dos limites

  else if (screen == SCREEN_EDIT) {

    // ================= LISTA DOS SENSORES =================
    // Navegação entre os sensores

    if (editing_value == false) {

      // Navega para cima
      if (button_pressed(BUTTON_UP)) {

        menu_index--;

        if (menu_index < 0) {
          menu_index = 3;
        }
      }

      // Navega para baixo
      if (button_pressed(BUTTON_DOWN)) {

        menu_index++;

        if (menu_index > 3) {
          menu_index = 0;
        }
      }

      // Seleciona item
      if (button_pressed(BUTTON_SELECT)) {

        // SAIR
        if (menu_index == 3) {

          // Volta ao menu principal
          screen = SCREEN_MENU;

        } else {

          // Entra no modo de edição
          editing_value = true;
        }
      }
    }

    // ================= EDITANDO VALOR =================
    // Usuário alterando os valores

    else {

      // ===== TEMPERATURA =====
      if (menu_index == 0) {

        // Incrementa temperatura máxima
        if (button_pressed(BUTTON_UP))

          if (temp_max < 80)
            temp_max++;

        // Decrementa temperatura máxima
        if (button_pressed(BUTTON_DOWN))

          if (temp_max > 0)
            temp_max--;
      }

      // ===== UMIDADE =====
      if (menu_index == 1) {

        // Incrementa umidade mínima
        if (button_pressed(BUTTON_UP))

          if (umid_min < 100)
            umid_min++;

        // Decrementa umidade mínima
        if (button_pressed(BUTTON_DOWN))

          if (umid_min > 0)
            umid_min--;
      }

      // ===== LUZ =====
      if (menu_index == 2) {

        // Incrementa luminosidade mínima
        if (button_pressed(BUTTON_UP))

          if (luz_min < 4085)
            luz_min += 10;

        // Decrementa luminosidade mínima
        if (button_pressed(BUTTON_DOWN))

          if (luz_min > 0)
            luz_min -= 10;
      }

      // Sai do modo de edição
      if (button_pressed(BUTTON_SELECT)) {
        editing_value = false;
      }
    }
  }

  // ================= CONFIG =================
  // Tela de configurações do sistema

  else if (screen == SCREEN_CONFIG) {

    // Navegação para cima
    if (button_pressed(BUTTON_UP)) {

      menu_index--;

      if (menu_index < 0) {
        menu_index = 2;
      }
    }

    // Navegação para baixo
    if (button_pressed(BUTTON_DOWN)) {

      menu_index++;

      if (menu_index > 2) {
        menu_index = 0;
      }
    }

    // Seleciona opção
    if (button_pressed(BUTTON_SELECT)) {

      // Liga/desliga sistema
      if (menu_index == 0) {
        system_enable = !system_enable;
      }

      // Liga/desliga logs
      if (menu_index == 1) {
        log_enable = !log_enable;
      }

      // Volta ao menu principal
      if (menu_index == 2) {
        screen = SCREEN_MENU;
      }
    }
  }
}