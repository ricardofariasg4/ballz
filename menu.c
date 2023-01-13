#include "menu.h"

static void teste (bool teste, char *descricao)
{
    if (teste) return;
    
    perror(descricao);
    exit(1);
}

int verificar_play_pause (MOUSE *mouse, HUD *hud, int menu)
{
    if (menu==PAUSE)
    {
        if (mouse->px_atual>=85 && mouse->px_atual<=205)
        {
            if (mouse->py_atual>=230 && mouse->py_atual<=350) {
                return REINICIAR;
            }

            if (mouse->py_atual>=360 && mouse->py_atual<=480) {
                return SAIR;
            }
        }
    }

    //Condição abaixo verifica se o click foi efetuado na região do botão PLAY/PAUSE
    if ((mouse->px_atual>=10 && mouse->px_atual<=27) && (mouse->py_atual>=10 && mouse->py_atual<=30))
    {
        if (hud->estado_botao==ESTADO_BOTAO_PAUSE)
        {
            hud->estado_botao=ESTADO_BOTAO_PLAY;
            return PAUSE;
        }

        hud->estado_botao=ESTADO_BOTAO_PAUSE;
        return JOGANDO;
    }

    return hud->estado_botao==ESTADO_BOTAO_PAUSE ? JOGANDO : PAUSE;
}

int verificar_opcao (MOUSE *mouse)
{
    if (mouse->px_atual>=50 && mouse->px_atual<=240)
    {
        //Botão de jogar
        if (mouse->py_atual>=260 && mouse->py_atual<=320)
            return JOGANDO;

        //Botão de sair
        if ((mouse->px_atual>=50 && mouse->px_atual<=240) && (mouse->py_atual>=360 && mouse->py_atual<=420))
            return SAIR;
    }
    
    return MENU;
}

int verificar_fim_jogo (MOUSE *mouse)
{
    if ((mouse->px_atual>=30 && mouse->px_atual<=260))
    {
        //Botão de reiniciar
        if (mouse->py_atual>=220 && mouse->py_atual<=280)
            return REINICIAR;

        //Botão de sair
        if (mouse->py_atual>=300 && mouse->py_atual<=360)
            return SAIR;
    }

    return FIM_DE_JOGO;
}

static int compara_int (const void *a, const void *b)
{
    if (*(int*)a == *(int*)b)
        return 0;

    if (*(int*)a < *(int*)b)
        return 1;

    return -1;
}

int *registrar_score (HUD *hud)
{
    int i, buffer_int=0, contador=0;
    int *scores;
    
    hud->pontuacao_anterior = fopen(PONTUACAO_LOCAL, "a");
    teste(hud->pontuacao_anterior, "abertura score");
    fprintf(hud->pontuacao_anterior, "%d\n", hud->pontos);

    freopen(PONTUACAO_LOCAL, "r", hud->pontuacao_anterior);
    teste(hud->pontuacao_anterior, "reabertura score");

    while (!feof(hud->pontuacao_anterior))
    {
        fscanf(hud->pontuacao_anterior, "%d", &buffer_int);
        contador++;
    }

    contador-=1;

    scores = (int *) malloc(contador * sizeof(int));
    teste(scores, "scores malloc");

    rewind(hud->pontuacao_anterior);

    for (i=0; i<contador; i++)
    {
        fscanf(hud->pontuacao_anterior, "%d", &buffer_int);
        scores[i] = buffer_int;
    }

    hud->num_scores = contador;

    fclose(hud->pontuacao_anterior);

    //Ordenar o vetor de scores e retornar
    qsort(scores, contador, sizeof(int), compara_int);

    return scores;
}

void iniciar_audios (SOM *sons)
{
    sons->coin = al_load_sample(COIN_AUDIO);
    teste(sons->coin, "coin audio");

    sons->colisao = al_load_sample(COLISAO_AUDIO);
    teste(sons->coin, "colisao audio");

    sons->fim_de_jogo = al_load_sample(FIM_AUDIO);
    teste(sons->coin, "fim audio");

    sons->intro = al_load_sample(INTRO_AUDIO);
    teste(sons->coin, "intro audio");

    sons->brasil_audio = al_load_sample(BRASIL_AUDIO);
    teste(sons->brasil_audio, "brasil audio");
}

void destruir_audios (SOM *sons)
{
    al_destroy_sample(sons->coin);
    al_destroy_sample(sons->colisao);
    al_destroy_sample(sons->fim_de_jogo);
    al_destroy_sample(sons->intro);
}