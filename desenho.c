#include "desenho.h"

static void teste (bool teste, char *descricao)
{
    if (teste) return;
    
    perror(descricao);
    exit(1);
}

void iniciar_sprites (SPRITES *sprites)
{
    sprites->bolas_sprites = al_load_bitmap(BOLA_SPRITESHEET);
    teste(sprites->bolas_sprites, "bola spritesheet");

    sprites->moedas_sprites = al_load_bitmap(MOEDA_SPRITESHEET);
    teste(sprites->moedas_sprites, "moeda spritesheet");
    
    sprites->tela_ajuda = al_load_bitmap(AJUDA_IMAGEM);
    teste(sprites->tela_ajuda, "tela ajuda");

    sprites->bandeira_brasil = al_create_sub_bitmap(al_load_bitmap(BANDEIRA_SPRITESHEET), 4, 116, 30, 20);
    teste(sprites->bandeira_brasil, "bandeira do brasil");

    /* ----------------------------------------------------------------------------------- */
    
    sprites->bola_futebol[0] = al_create_sub_bitmap(sprites->bolas_sprites, 4, 44, 14, 14);
    teste(sprites->bola_futebol[0], "bola 1");

    sprites->bola_futebol[1] = al_create_sub_bitmap(sprites->bolas_sprites, 24, 44, 14, 14);
    teste(sprites->bola_futebol[1], "bola 2");

    sprites->bola_futebol[2] = al_create_sub_bitmap(sprites->bolas_sprites, 44, 44, 14, 14);
    teste(sprites->bola_futebol[2], "bola 3");

    sprites->bola_futebol[3] = al_create_sub_bitmap(sprites->bolas_sprites, 64, 44, 14, 14);
    teste(sprites->bola_futebol[3], "bola 4");

    sprites->bola_futebol[4] = al_create_sub_bitmap(sprites->bolas_sprites, 84, 80, 14, 14);
    teste(sprites->bola_futebol[4], "bola 5");

    sprites->bola_futebol[5] = al_create_sub_bitmap(sprites->bolas_sprites, 104, 80, 14, 14);
    teste(sprites->bola_futebol[5], "bola 6");

    sprites->bola_futebol[6] = al_create_sub_bitmap(sprites->bolas_sprites, 124, 80, 14, 14);
    teste(sprites->bola_futebol[6], "bola 7");

    sprites->bola_futebol[7] = al_create_sub_bitmap(sprites->bolas_sprites, 144, 80, 14, 14);
    teste(sprites->bola_futebol[7], "bola 8");

    /* ----------------------------------------------------------------------------------- */
    
    sprites->moedas[0] = al_create_sub_bitmap(sprites->moedas_sprites, 0, 0, 170, 170);
    teste(sprites->moedas[0], "moeda 1");
    
    sprites->moedas[1] = al_create_sub_bitmap(sprites->moedas_sprites, 210, 0, 130, 170);
    teste(sprites->moedas[1], "moeda 2");

    sprites->moedas[2] = al_create_sub_bitmap(sprites->moedas_sprites, 430, 0, 80, 170);
    teste(sprites->moedas[2], "moeda 3");

    sprites->moedas[3] = al_create_sub_bitmap(sprites->moedas_sprites, 650, 0, 40, 170);
    teste(sprites->moedas[3], "moeda 4");

    sprites->moedas[4] = al_create_sub_bitmap(sprites->moedas_sprites, 840, 0, 80, 170);
    teste(sprites->moedas[4], "moeda 5");

    sprites->moedas[5] = al_create_sub_bitmap(sprites->moedas_sprites, 1020, 0, 130, 170);
    teste(sprites->moedas[5], "moeda 6");
}

void destruir_sprites (SPRITES *sprites)
{
    al_destroy_bitmap(sprites->tela_ajuda);
    al_destroy_bitmap(sprites->bandeira_brasil);

    for (int i=0; i<8; i++)
        al_destroy_bitmap(sprites->bola_futebol[i]);
    
    for (int i=0; i<6; i++)
        al_destroy_bitmap(sprites->moedas[i]);

    al_destroy_bitmap(sprites->moedas_sprites);
    al_destroy_bitmap(sprites->bolas_sprites);
}

void disp_pre_desenho (ALLEGRO_BITMAP **buffer)
{
    al_set_target_bitmap(*buffer);
}

void disp_pos_desenho (ALLEGRO_DISPLAY **display, ALLEGRO_BITMAP **buffer)
{
    al_set_target_backbuffer(*display);
    al_draw_scaled_bitmap(*buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}

void destruir_display (ALLEGRO_DISPLAY *tela, ALLEGRO_BITMAP *buffer)
{
    al_destroy_display(tela);
    al_destroy_bitmap(buffer);
}

void desenhar_menu (RECURSO *recurso)
{   
    al_draw_text(recurso->fonte_nativa, COR_ROSA, 117, 100, 0, "B");
    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 127, 100, 0, "a");
    al_draw_text(recurso->fonte_nativa, COR_AZUL, 134, 100, 0, "l");
    al_draw_text(recurso->fonte_nativa, COR_VERDE, 141, 100, 0, "l");
    al_draw_text(recurso->fonte_nativa, COR_VERMELHO, 148, 100, 0, "z");

    al_draw_filled_rounded_rectangle(ESPACO+40, HUD_H+180, BUFFER_W-ESPACO-40, 320, 10, 10, COR_ROSA);
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 120, 285, 0, "JOGAR");

    al_draw_filled_rounded_rectangle(ESPACO+40, HUD_H+280, BUFFER_W-ESPACO-40, 420, 10, 10, COR_PRETO);
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 125, 385, 0, "SAIR");
}

void desenhar_efeito_menu (float x1, float y1)
{
    if ((x1>=50 && x1<=240) && (y1>=260 && y1<=320))
        al_draw_filled_rounded_rectangle(ESPACO+35, HUD_H+175, BUFFER_W-ESPACO-35, 327, 10, 10, COR_PRETO);

    if ((x1>=50 && x1<=240) && (y1>=360 && y1<=420)) 
        al_draw_filled_rounded_rectangle(ESPACO+35, HUD_H+275, BUFFER_W-ESPACO-35, 425, 10, 10, COR_BRANCO);
}

void desenhar_hud (HUD *hud, RECURSO *recurso, SPRITES *sprites)
{
    al_draw_filled_rectangle(0, 0, HUD_W, HUD_H, COR_HUD); //fundo do hud
    
    if (hud->estado_botao == ESTADO_BOTAO_PAUSE)
    {
        //al_draw_filled_rectangle(10, 5, 20, 35, COR_BRANCO); //parte do botão pause
        al_draw_filled_rounded_rectangle(10, 10, 17, 30, 2, 2, COR_HUD_BOTAO); //parte do botão pause
        al_draw_filled_rounded_rectangle(20, 10, 27, 30, 2, 2, COR_HUD_BOTAO);//outra parte do botão pause
    }
    else
        al_draw_filled_triangle(10, 5, 10, 30, 32, 17, COR_HUD_BOTAO); //botão play

    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 40, 10, 0, "BEST");
    
    sprintf(hud->valor_texto, "%d", hud->maior_pontuacao);
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 40, 22, 0, hud->valor_texto);

    sprintf(hud->valor_texto, "%d", hud->pontos);
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 140, 22, 0, hud->valor_texto);

    sprintf(hud->valor_texto, "%d", hud->moedas);
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, BUFFER_W-60, 22, 0, hud->valor_texto);
    
    if (sprites->moeda_atual>2)
        al_draw_scaled_bitmap(sprites->moedas[sprites->moeda_atual], 0, 0, 170, 170, BUFFER_W-27, 12, 20, 20, 0);
    else
        al_draw_scaled_bitmap(sprites->moedas[sprites->moeda_atual], 0, 0, 170, 170, BUFFER_W-30, 12, 20, 20, 0);

    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 10, 45, 0, "F1 OU H PARA AJUDA");
}

void desenhar_bola (LISTA *lista, SPRITES *sprites, bool surpresa)
{
    NODO *aux = lista->primeira_bola;

    if (surpresa)
    {
        while (aux != NULL)
        {
            al_draw_bitmap(sprites->bola_futebol[sprites->bola_atual], aux->bola->px-10, aux->bola->py-5, 0);
            
            aux = aux->proximo;
        }
    }   
    else
    {
        while (aux != NULL)
        {
            al_draw_filled_circle(aux->bola->px, aux->bola->py, aux->bola->r, aux->bola->cor);    
            
            aux = aux->proximo;
        }
    }
}

void desenhar_quadrados (LISTA *lista, RECURSO *recurso, ALLEGRO_BITMAP *moeda_atual, int moeda)
{
    NODO *aux = lista->primeiro_quadrado;
    int pos_x, pos_y; //para o caso de bola extra

    while (aux != NULL)
    {
        if (aux->quadrado->visibilidade)
        {
            if (aux->quadrado->bola_extra)
            {
                pos_x = aux->quadrado->px+(QUADRADO_SIZE/2);
                pos_y = aux->quadrado->py+(QUADRADO_SIZE/2);

                al_draw_filled_circle(pos_x, pos_y, 10, COR_BRANCO);
                al_draw_filled_circle(pos_x, pos_y, 7, COR_FUNDO);
            }
            
            if (aux->quadrado->moeda_extra)
            {
                pos_x = aux->quadrado->px+(QUADRADO_SIZE/2)-7;
                pos_y = aux->quadrado->py+(QUADRADO_SIZE/2)-7;

                if (moeda>2)
                    al_draw_scaled_bitmap(moeda_atual, 0, 0, 170, 170, pos_x+3, pos_y, 20, 20, 0);
                else
                    al_draw_scaled_bitmap(moeda_atual, 0, 0, 170, 170, pos_x, pos_y, 20, 20, 0);
            }

            else
            {
                aux->quadrado->font_px = aux->quadrado->px+4;
                aux->quadrado->font_py = aux->quadrado->py+12;

                al_draw_filled_rectangle
                (
                    aux->quadrado->px,
                    aux->quadrado->py,
                    aux->quadrado->px+QUADRADO_SIZE,
                    aux->quadrado->py+QUADRADO_SIZE,
                    aux->quadrado->cor
                );
                
                al_draw_text(recurso->fonte_nativa, COR_PRETO, aux->quadrado->font_px, aux->quadrado->font_py, 0, aux->quadrado->valor_texto);
            }
        }
        
        aux = aux->proximo;
    }
}

void desenhar_fim (RECURSO *recurso, HUD *hud, int *scores)
{
    char buffer_char[3];
    int i, j;

    for (i=0; i<hud->num_scores; i++)
    {
        if (scores[i]==hud->pontos)
            break;
    }

    i+=1;

    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 30, 40, 0, "VOCÊ CONSEGUIU");
    sprintf(buffer_char, "%d", hud->pontos);
    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 150, 40, 0, buffer_char);
    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 180, 40, 0, "PONTOS!");

    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 30, 60, 0, "VOCÊ FICOU EM");
    sprintf(buffer_char, "%d", i);
    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 150, 60, 0, buffer_char);
    al_draw_text(recurso->fonte_nativa, COR_AMARELO, 175, 60, 0, "LUGAR!");

    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 30, 85, 0, "AS 5 MELHORES PONTUAÇÕES FORAM:");

    //Exibir os 5 melhores
    for (j=0, i=0; i<hud->num_scores; i++, j+=18)
    {
        sprintf(buffer_char, "%d", (i+1));
        al_draw_text(recurso->fonte_nativa, COR_BRANCO, 30, j+100, 0, buffer_char);
        al_draw_text(recurso->fonte_nativa, COR_BRANCO, 40, j+100, 0, "LUGAR");
        sprintf(buffer_char, "%d", scores[i]);
        al_draw_text(recurso->fonte_nativa, COR_BRANCO, 90, j+100, 0, buffer_char);
        if (i==4) break;
    }
    
    al_draw_filled_rounded_rectangle(30, 220, 260, 280, 10, 10, COR_AZUL);
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 110, 245, 0, "REINICIAR");

    al_draw_filled_rounded_rectangle(30, 300, 260, 360, 10, 10, COR_VERMELHO);
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 125, 325, 0, "SAIR");

    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 20, BUFFER_H-50, 0, "Ballz!");
}

//P1(x1,y1) -> bola | P2(x2,y2) -> posição do mouse
void desenhar_trajetoria_disparo (float x1, float y1, int x2, int y2)
{
    float a = (y2-y1)/(x2-x1);
    float b = y1-(a*x1);
    float i, y;
    
    if (x2>x1)
    {
        for (i=x1; i<=x2; i+=13)
        {   
            y = (a*i)+b;
            al_draw_filled_circle(i, y, 4, COR_BRANCO);
        }
    }
    else if (x2<x1)
    {
        for (i=x1; i>=x2; i-=13)
        {   
            y = (a*i)+b;
            al_draw_filled_circle(i, y, 4, COR_BRANCO);
        }
    }
    else 
        for (i=y1; i>=y2; i-=40)
            al_draw_filled_circle(x1, i, 4, COR_BRANCO);
}

void desenhar_efeito_pause (float x1, float y1, RECURSO *recurso)
{
    if (x1>=85 && x1<=205)
    {
        if (y1>=230 && y1<=350) {
            al_draw_filled_circle(145, 290, 50, COR_VERDE);
            al_draw_text(recurso->fonte_nativa, COR_PRETO, 110, 286, 0, "REINICIAR");
            return;
        }

        if (y1>=360 && y1<=480) {
            al_draw_filled_circle(145, 420, 50, COR_VERMELHO);
            al_draw_text(recurso->fonte_nativa, COR_PRETO, 125, 406, 0, "SAIR");
            al_draw_text(recurso->fonte_nativa, COR_PRETO, 135, 420, 0, "DO");
            al_draw_text(recurso->fonte_nativa, COR_PRETO, 125, 435, 0, "JOGO");
            return;
        }
    }
}

void desenhar_menu_pause (RECURSO *recurso)
{
    al_draw_filled_rounded_rectangle(65, 205, 230, 505, 10, 10, COR_HUD);
    
    al_draw_filled_circle(145, 290, 60, COR_PRETO);
    al_draw_filled_circle(145, 290, 50, COR_VERDE_CLARO);
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 110, 286, 0, "REINICIAR");

    al_draw_filled_circle(145, 420, 60, COR_PRETO);
    al_draw_filled_circle(145, 420, 50, COR_VERMELHO_CLARO);
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 125, 406, 0, "SAIR");
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 135, 420, 0, "DO");
    al_draw_text(recurso->fonte_nativa, COR_PRETO, 125, 435, 0, "JOGO");
}

void desenhar_menu_ajuda (RECURSO *recurso, ALLEGRO_BITMAP *tela_ajuda)
{
    al_draw_bitmap(tela_ajuda, 40, 40, 0);
    al_draw_filled_rounded_rectangle(40, 480, 250, 550, 10, 10, COR_AZUL_ESCURO);
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 95, 500, 0, "PRESSIONE");
    al_draw_text(recurso->fonte_nativa, COR_BRANCO, 60, 520, 0, "H OU F1 PARA VOLTAR");
}