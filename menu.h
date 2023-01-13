#ifndef MENU_H
#define MENU_H

#include "estruturas.h"
#include "lista.h"

int verificar_play_pause (MOUSE *mouse, HUD *hud, int menu);
int verificar_opcao (MOUSE *mouse);
int verificar_fim_jogo (MOUSE *mouse);
int *registrar_score (HUD *hud);
void iniciar_audios (SOM *sons);
void destruir_audios (SOM *sons);

#endif