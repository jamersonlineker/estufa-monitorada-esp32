#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

// ================= PINOS =================

#define BUTTON_UP 18
#define BUTTON_DOWN 19
#define BUTTON_SELECT 23

// ================= MENU =================

typedef enum {

    SCREEN_HOME,
    SCREEN_MENU,
    SCREEN_EDIT,
    SCREEN_CONFIG

} screen_status_t;

// ===== variáveis externas=====

extern screen_status_t screen;
extern screen_status_t last_screen;

extern int menu_index;

extern bool editing_value;

// ===== funções =====

void update_menu();

#endif