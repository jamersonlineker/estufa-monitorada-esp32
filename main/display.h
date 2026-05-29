#ifndef DISPLAY_H
#define DISPLAY_H

// ================= DISPLAY =================
// Definição dos pinos
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22

#include "ssd1306.h"

// display global
extern SSD1306_t dev;

// inicialização
void init_display();

// telas
void draw_home();
void draw_menu();
void draw_edit();
void draw_config();
void clear_display();

#endif