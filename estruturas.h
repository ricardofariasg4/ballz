#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

//Definições gerais
#define FPS 60.0
#define BUFFER_W 290
#define BUFFER_H 620
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

//Medidas, quantidades, tamanhos e espaçamentos
#define HUD_W BUFFER_W
#define HUD_H 80
#define QUADRADO_SIZE 30
#define ESPACO 10
#define QUADRADOS_POR_FILA 7
#define MAX_DIGITOS 3
#define FONTE_QUADRADO_TAM 30
#define FONTE_HUD_TAM 12
#define FONTE_FLAGS 0

//Cores mais usuais
#define COR_BRANCO al_map_rgb(255, 255, 255)
#define COR_PRETO al_map_rgb(0, 0, 0)
#define COR_AMARELO al_map_rgb(255, 255, 0)
#define COR_VERDE al_map_rgb(0, 128, 0)
#define COR_ROSA al_map_rgb(255, 20, 147)
#define COR_LARANJA al_map_rgb(255, 140, 0) 
#define COR_VERMELHO al_map_rgb(255, 0, 0)
#define COR_HUD al_map_rgb(38, 38, 38)
#define COR_HUD_BOTAO al_map_rgb(70, 70, 70)
#define COR_FUNDO al_map_rgb(31, 31, 31)
#define COR_AZUL al_map_rgb(28, 82, 148)
#define COR_VERDE_CLARO al_map_rgb(54, 128, 56)
#define COR_VERMELHO_CLARO al_map_rgb(156, 48, 48)
#define COR_AZUL_ESCURO al_map_rgb(0, 8, 44)

//Caminho de recursos
//#define FONTE_LOCAL "./recursos/fontes/KoralleRMU-SemiBold.ttf"
#define PONTUACAO_LOCAL "./recursos/pontuacao/melhor_pontuacao.txt"
#define MOEDA_SPRITESHEET "./recursos/spritesheets/coin.png"
#define BANDEIRA_SPRITESHEET "./recursos/spritesheets/bandeiras.png"
#define BOLA_SPRITESHEET "./recursos/spritesheets/ball.png"
#define AJUDA_IMAGEM "./recursos/imagens/ajuda.jpg"
#define COIN_AUDIO "./recursos/audios/coin.wav"
#define COLISAO_AUDIO "./recursos/audios/colisao.wav"
#define FIM_AUDIO "./recursos/audios/fim_de_jogo.wav"
#define INTRO_AUDIO "./recursos/audios/intro.wav"
#define LEVEL_UP_AUDIO "./recursos/audios/level_up.wav"
#define BRASIL_AUDIO "./recursos/audios/brasil_globo.wav"

//Estados
#define ESTADO_BOTAO_PAUSE 0
#define ESTADO_BOTAO_PLAY 1
#define MENU 0
#define JOGANDO 1
#define AJUDA 2
#define PAUSE 3
#define FIM_DE_JOGO 4
#define SAIR 5
#define REINICIAR 6

typedef struct _BOLA {
    float px_origem;
    float py_origem;
    float px;
    float py;
    float vx;
    float vy;
    float r;
    float cofa; //cofa = coeficiente angular, seria usado na nova função de movimento da bola
    float cofl; //cofl = coeficiente linear
    int multiplicador;
    int moedas;
    int pontos;
    bool colisao;
    int isenta_colisao;
    bool em_trajetoria;
    ALLEGRO_COLOR cor;
} BOLA;

typedef struct _QUADRADO {
    float px;
    float py;
    float font_px;
    float font_py;
    int valor;
    bool visibilidade;
    bool bola_extra;
    bool moeda_extra;
    char valor_texto[MAX_DIGITOS];
    ALLEGRO_COLOR cor;
} QUADRADO;

typedef struct _HUD {
    int pontos;
    int pontos_por_partida;
    int moedas;
    int maior_pontuacao;
    int estado_botao;
    int num_scores;
    FILE *pontuacao_anterior;
    char valor_texto[MAX_DIGITOS];
} HUD;

/*
    Dessa estrutura, apenas a fonte_nativa está sendo usada por problemas de incompatibilidade.
    Chama-se recurso, pois no início eu tinha a intenção de deixar nessa estrutura, todos os recursos
    externos ao programa, mas tornaria a estrutura complexa e confusa.
*/
typedef struct _RECURSO {
    ALLEGRO_FONT *fonte_maior;
    ALLEGRO_FONT *fonte_media;
    ALLEGRO_FONT *fonte_pequena;
    ALLEGRO_FONT *fonte_gigante;
    ALLEGRO_FONT *fonte_nativa;
} RECURSO;

typedef struct _MOUSE {
    int px_atual;
    int py_atual;
    int px_disparo;
    int py_disparo;
    bool disponivel;
} MOUSE;

typedef struct _SOM {
    ALLEGRO_SAMPLE *coin;
    ALLEGRO_SAMPLE *colisao;
    ALLEGRO_SAMPLE *fim_de_jogo;
    ALLEGRO_SAMPLE *intro;
    ALLEGRO_SAMPLE *brasil_audio;
} SOM;

typedef struct _SPRITES {
    ALLEGRO_BITMAP *moedas_sprites;
    ALLEGRO_BITMAP *bolas_sprites;
    ALLEGRO_BITMAP *bandeira_brasil;
    ALLEGRO_BITMAP *moedas[6];
    ALLEGRO_BITMAP *tela_ajuda;
    ALLEGRO_BITMAP *bola_futebol[8];
    int moeda_atual;
    int bola_atual;
} SPRITES;

#endif