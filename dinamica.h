#include "estruturas.h"
#include "lista.h"

void inicializar_mouse (MOUSE *mouse);
void inicializar_hud (HUD *hud);
void inicializar_bola (LISTA *lista_bola);
void inicializar_quadrado (QUADRADO *quadrado, int bola_kills);
void inserir_fileira (LISTA *lista_quadrado, LISTA *lista_bola);
float distancia (float x1, float y1, float x2, float y2);
bool colisao (BOLA *bola, QUADRADO *quadrado);
void limite_da_bola_v2 (LISTA *lista_bola);
void limite_da_bola (LISTA *lista_bola);
int gravidade_do_quadrado (LISTA *lista_quadrado);
void atualizar_hud (HUD *hud, LISTA *lista_bola);
bool bolas_em_trajetoria (LISTA *lista_bola);
void trajetoria_da_bola (LISTA *lista_bola, int x2, int y2);
void mover_bolas (LISTA *lista_bola);
void verificar_colisao (LISTA *lista_bola, LISTA *lista_quadrado, SOM *sons, HUD *hud);
void atualizar_quadrados (LISTA *lista_quadrado, LISTA *lista_bola);
void caiu_no_chao (LISTA *lista_bola);
bool bolas_no_chao (LISTA *lista_bola);
void atualizar_bola (LISTA *lista_bola);
void conta_bolas(LISTA *lista_bola);
bool fim_de_jogo (LISTA *lista_quadrado);
bool fim_de_round (LISTA *lista_bola);
bool verificar_surpresa (HUD *hud, bool desenho_surpresa);

