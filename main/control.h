#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>


// ================= PINOS =================

#define LED_R 16
#define LED_G 17
#define LED_B 4
// ===== CONFIGURAÇÕES =====

extern int temp_max;
extern int umid_min;
extern int luz_min;

// ===== CONTROLE =====

extern bool system_enable;

// ===== FUNÇÕES =====

void init_leds();

void update_control();

#endif