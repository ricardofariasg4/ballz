#include "estruturas.h"
#include "lista.h"

void carregar_recursos (RECURSO *recurso);
void iniciar_sprites (SPRITES *sprites);
void destruir_sprites (SPRITES *sprites);
void disp_pre_desenho (ALLEGRO_BITMAP **buffer);
void disp_pos_desenho (ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer);
void destruir_display (ALLEGRO_DISPLAY *tela, ALLEGRO_BITMAP *buffer);
void desenhar_menu (RECURSO *recurso);
void desenhar_efeito_menu (float x1, float y1);
void desenhar_hud (HUD *hud, RECURSO *recurso, SPRITES *sprites);
void desenhar_bola (LISTA *lista, SPRITES *sprites, bool surpresa);
void desenhar_quadrados (LISTA *lista, RECURSO *recurso, ALLEGRO_BITMAP *moeda_atual, int moeda);
void desenhar_fim(RECURSO *recurso, HUD *hud, int *scores);
void desenhar_trajetoria_disparo (float x1, float y1, int x2, int y2);
void desenhar_efeito_pause (float x1, float y1, RECURSO *recurso);
void desenhar_menu_pause (RECURSO *recurso);
void desenhar_efeito_menu_ajuda (float x1, float y1);
void desenhar_menu_ajuda (RECURSO *recurso, ALLEGRO_BITMAP *tela_ajuda);