#include "estruturas.h"
#include "lista.h"
#include "desenho.h"
#include "dinamica.h"
#include "menu.h"

static void teste (bool teste, char *descricao);
static void rotinas_de_sistema ();

int main (void)
{
    //Variáveis essenciais allegro
    ALLEGRO_TIMER *relogio = NULL;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *buffer = NULL;
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    ALLEGRO_EVENT evento;
    
    //Estruturas principais do jogo
    SPRITES sprites;
    SOM sons;
    HUD hud;
    MOUSE mouse;
    RECURSO recurso; //Era para as fontes TTF, mas não funcionam nos computadores do lab
    LISTA *lista_quadrado=NULL;
    LISTA *lista_bola=NULL;

    //Variáveis de controle da main
    int menu=MENU, 
        limite_inferior=0, 
        frames=0,
        *scores=NULL;
    
    sprites.moeda_atual=0;
    sprites.bola_atual=0;

    //Estados de jogo
    bool fim=false, 
        desenho=true, 
        gravidade=true, 
        score_gravado=false, 
        surpresa=false, som_surpresa=false, desenho_surpresa=false,
        inicio_partida=true;
    
    char qtd_bolas[2];

    rotinas_de_sistema();

    recurso.fonte_nativa = al_create_builtin_font();
    teste(recurso.fonte_nativa, "impossível carregar a fonte");
    
    display = al_create_display(DISP_W, DISP_H);
    teste(display, "display");
    
    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    teste(buffer, "buffer");
    
    relogio = al_create_timer(1.0 / FPS);
    teste(relogio, "relogio");

    fila_de_eventos = al_create_event_queue();
    teste(fila_de_eventos, "fila_de_eventos");

    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_de_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_de_eventos, al_get_timer_event_source(relogio));
    al_register_event_source(fila_de_eventos, al_get_mouse_event_source()); 

    al_start_timer(relogio);

    //Inicializar objetos
    lista_quadrado = criar_lista();
    lista_bola = criar_lista();
    inicializar_mouse(&mouse);
    inicializar_hud(&hud);
    inicializar_bola(lista_bola); //Insere a primeira bola na lista
    inserir_fileira(lista_quadrado, lista_bola); //Insere os primeiros quadrados do jogo
    iniciar_audios(&sons);
    iniciar_sprites(&sprites);

    while (1)
    {
        if (frames%10==0)
        {
            sprites.moeda_atual++;
            sprites.bola_atual++;
        }

        if (sprites.moeda_atual>5)
            sprites.moeda_atual=0;
        
        if (sprites.bola_atual>7)
            sprites.bola_atual=0;

        if (frames>60)
            frames=0;

        al_wait_for_event(fila_de_eventos, &evento);

        switch (evento.type)
        {
            case ALLEGRO_EVENT_TIMER:
                switch (menu)
                {
                    case REINICIAR:
                        destruir_lista(lista_quadrado, 0);
                        destruir_lista(lista_bola, 1);
                        lista_quadrado = criar_lista();
                        lista_bola = criar_lista();
                        inicializar_bola(lista_bola); //Insere a primeira bola na lista
                        inserir_fileira(lista_quadrado, lista_bola);
                        
                        menu=JOGANDO;
                        hud.estado_botao=ESTADO_BOTAO_PAUSE;
                        hud.moedas=0;
                        hud.pontos=0;
                        limite_inferior=0;

                        surpresa=false;
                        som_surpresa=false;
                        desenho_surpresa=false;
                        mouse.disponivel=true;
                        gravidade=true;
                        inicio_partida=true;
                        score_gravado=false;	
                        
                    break;
                    
                    case SAIR: 
                        fim=true; 
                    break;

                    case FIM_DE_JOGO: 
                        if (!score_gravado)
                        {
                            scores = registrar_score(&hud);
                            score_gravado=true;
                        }
                    break;

                    case JOGANDO:
                        //Som de inicio da partida
                        if (inicio_partida)
                        {
                            al_play_sample(sons.intro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            inicio_partida=false;
                        }

                        if (som_surpresa)
                        {
                            al_play_sample(sons.brasil_audio, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            som_surpresa=false;
                        }

                        //Se mouse não disponível, então, estamos em jogo
                        if (!mouse.disponivel)
                        { 
                            if (mouse.py_disparo<HUD_H || mouse.py_disparo>535)
                            {
                                mouse.disponivel=true;
                                break;
                            }
                            
                            if (bolas_em_trajetoria(lista_bola))
                                trajetoria_da_bola(lista_bola, mouse.px_disparo, mouse.py_disparo);
                            else
                                mover_bolas(lista_bola);

                            verificar_colisao(lista_bola, lista_quadrado, &sons, &hud);

                            limite_da_bola(lista_bola);
                            //limite_da_bola_v2(lista_bola);
                            
                            atualizar_hud (&hud, lista_bola);
                            atualizar_quadrados(lista_quadrado, lista_bola);
                            caiu_no_chao(lista_bola);
                            
                            //Se as bolas estão no chão, então é fim de round
                            if (bolas_no_chao(lista_bola) && mouse.py_disparo>HUD_H)
                            {
                                surpresa = verificar_surpresa(&hud, desenho_surpresa);   
                                
                                if (surpresa) 
                                {
                                    som_surpresa=true;
                                    desenho_surpresa=true;
                                }
                                else 
                                    som_surpresa=false;

                                mouse.disponivel=true;
                                limite_inferior=0;
                                gravidade=true;
                                atualizar_bola(lista_bola);
                                inserir_fileira(lista_quadrado, lista_bola);
                                conta_bolas(lista_bola);
                            }
                        }
                        else
                        {
                            if (gravidade)
                            {
                                limite_inferior += gravidade_do_quadrado(lista_quadrado);
                                if (limite_inferior == 40) gravidade=false; 

                                if (fim_de_jogo(lista_quadrado))
                                {
                                    al_play_sample(sons.fim_de_jogo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    menu=FIM_DE_JOGO;
                                }
                                    
                            }
                        }
                    break;
                }
            break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (evento.keyboard.keycode == ALLEGRO_KEY_H || evento.keyboard.keycode == ALLEGRO_KEY_F1)
                {
                    if (menu==AJUDA)
                        menu=JOGANDO;
                    else
                        menu=AJUDA;
                }
            break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                switch (menu)
                {
                    case MENU: menu = verificar_opcao(&mouse); break;
                    case JOGANDO: menu = verificar_play_pause (&mouse, &hud, menu); break;
                    case PAUSE: menu = verificar_play_pause (&mouse, &hud, menu); break;
                    case FIM_DE_JOGO: menu = verificar_fim_jogo(&mouse); break;
                }
            break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                //Botão do mouse liberado
                if (mouse.disponivel && limite_inferior>39)
                {
                    mouse.px_disparo = evento.mouse.x;
                    mouse.py_disparo = evento.mouse.y;
                    redefinir_origem_bolas(lista_bola);
                    mouse.disponivel = false;
                }
            break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                mouse.px_atual = evento.mouse.x;
                mouse.py_atual = evento.mouse.y;
            break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fim=true;
            break;
        }

        desenho=true;

        if (fim) break;

        if (desenho && al_is_event_queue_empty(fila_de_eventos))
        {
            disp_pre_desenho(&buffer);
            al_clear_to_color(COR_FUNDO);

            switch (menu)
            {
                case MENU:
                    desenhar_efeito_menu(mouse.px_atual, mouse.py_atual);
                    desenhar_menu(&recurso);
                break;

                case PAUSE:
                    desenhar_hud(&hud, &recurso, &sprites);
                    desenhar_quadrados(lista_quadrado, &recurso, sprites.moedas[sprites.moeda_atual], sprites.moeda_atual);
                    al_draw_filled_rectangle(0, 560, BUFFER_W, BUFFER_H, COR_HUD);
                    desenhar_menu_pause(&recurso);
                    desenhar_efeito_pause (mouse.px_atual, mouse.py_atual, &recurso);
                break;
                
                case JOGANDO:
                    desenhar_hud(&hud, &recurso, &sprites);
                
                    desenhar_quadrados(lista_quadrado, &recurso, sprites.moedas[sprites.moeda_atual], sprites.moeda_atual);
            
                    if (mouse.disponivel)
                    {
                        desenhar_trajetoria_disparo(lista_bola->primeira_bola->bola->px, lista_bola->primeira_bola->bola->py, mouse.px_atual, mouse.py_atual);
                        
                        sprintf(qtd_bolas, "%d", lista_bola->qtd_bolas);
                        al_draw_text(recurso.fonte_nativa, COR_BRANCO, lista_bola->primeira_bola->bola->px+10, 525, 0, "x");
                        al_draw_text(recurso.fonte_nativa, COR_BRANCO, lista_bola->primeira_bola->bola->px+20, 525, 0, qtd_bolas);
                    }

                    if (desenho_surpresa)
                    {
                        al_draw_text(recurso.fonte_nativa, COR_VERDE, 180, 55, 0, "#VEMHEXA");
                        al_draw_bitmap(sprites.bandeira_brasil, 250, 50, 0);
                        desenhar_bola(lista_bola, &sprites, desenho_surpresa);
                    }
                    else
                        desenhar_bola(lista_bola, &sprites, desenho_surpresa);
                    
                    
                    al_draw_filled_rectangle(0, 560, BUFFER_W, BUFFER_H, COR_HUD);
                break;

                case AJUDA:
                    desenhar_menu_ajuda(&recurso, sprites.tela_ajuda);
                break;

                case FIM_DE_JOGO:
                    desenhar_fim(&recurso, &hud, scores);
                break;
            }
            
            disp_pos_desenho(&display, &buffer);
            desenho=false;
        }   

        frames++;
    }

    /* ABAIXO É REALIZADA A LIMPEZA DA MEMORIA */
    destruir_lista(lista_quadrado, 0); //Destrói: quadrados, nodos e a lista
    destruir_lista(lista_bola, 1); //Destrói: bolas, nodos e a lista
    destruir_sprites(&sprites);
    destruir_display(display, buffer);
    destruir_audios(&sons);

    return 0;
}

static void teste (bool teste, char *descricao)
{
    if (teste) return;
    
    perror(descricao);
    exit(1);
}

static void rotinas_de_sistema ()
{
    teste(al_init(), "nucleo allegro");
    teste(al_install_keyboard(), "complemento de teclado");
    teste(al_init_primitives_addon(), "tipos primitivos");    
    teste(al_init_image_addon(), "complemento de imagem");
    teste(al_install_mouse(), "install mouse");
    teste(al_install_audio(), "audio");
    teste(al_init_acodec_addon(), "audio codecs");
    teste(al_reserve_samples(16), "reserve samples");
    srand(time(NULL)); //Gerando a semente de aleatórios

    /*Antialiasing -> não notei diferença no uso
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);*/
}

