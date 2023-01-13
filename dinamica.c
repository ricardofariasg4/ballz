#include "dinamica.h"

static void teste (bool teste, char *descricao)
{
    if (teste) return;
    
    perror(descricao);
    exit(1);
}

static int intervalo_aleatorio (int min, int max)
{
    return min + (rand() % (max - min));
}

void inicializar_mouse (MOUSE *mouse)
{
    mouse->disponivel = true;
    mouse->px_atual = 0;
    mouse->py_atual = 0;
    mouse->px_disparo = 0;
    mouse->py_disparo = 0;
}

void inicializar_hud (HUD *hud)
{
    int aux=0;
    hud->pontos = 0;
    hud->pontos_por_partida=0;
    hud->moedas = 0;
    hud->estado_botao = ESTADO_BOTAO_PAUSE;
    hud->maior_pontuacao = 0;
    
    hud->pontuacao_anterior = fopen(PONTUACAO_LOCAL, "r");
    teste(hud->pontuacao_anterior, "dinamica.c -> inicializar_hud");
    
    while (!feof(hud->pontuacao_anterior))
    {
        fscanf(hud->pontuacao_anterior, "%d", &hud->maior_pontuacao);    

        if (aux>hud->maior_pontuacao)
            hud->maior_pontuacao=aux;

        aux = hud->maior_pontuacao;
    }

    fclose(hud->pontuacao_anterior);
}

void inicializar_bola (LISTA *lista_bola)
{
    BOLA *bola = (BOLA *) malloc(sizeof(BOLA));
    teste(bola, "dinamica.c -> inicializar_bola");
    
    bola->r = 5.0;
    bola->px_origem = 0;
    bola->py_origem = 0;
    bola->py = 535;
    bola->px = 145;
    bola->vx = 0;
    bola->vy = 0;
    bola->multiplicador = 0;
    bola->cor = COR_BRANCO;
    bola->em_trajetoria = true;
    bola->pontos = 0;
    bola->moedas = 0;
    bola->colisao = true;
    bola->isenta_colisao = 0;

    lista_bola->qtd_bolas++;
    insere_bola(bola, lista_bola);
}

static void aplicar_cor_quadrado (QUADRADO *quadrado)
{
    if ( (quadrado->valor > 0) && (quadrado->valor < 5) )
    {
        quadrado->cor = COR_AMARELO;
        return;
    }

    if (quadrado->valor < 14)
    {
        quadrado->cor = COR_VERDE;
        return;
    }

    if (quadrado->valor < 28)
    {
        quadrado->cor = COR_ROSA;
        return;
    }

    if (quadrado->valor < 41)
    {
        quadrado->cor = COR_LARANJA;
        return;
    }

    if (quadrado->valor < 62)
    {
        quadrado->cor = COR_VERMELHO;
        return;
    }
    
    quadrado->cor = COR_VERMELHO;
}

void inicializar_quadrado (QUADRADO *quadrado, int bola_kills)
{
    int resultado = (rand()%100)+1;
    int incremento = bola_kills/QUADRADOS_POR_FILA;
    
    quadrado->visibilidade = false;
    quadrado->bola_extra = false;
    quadrado->moeda_extra = false;
    quadrado->valor = 1;
    quadrado->px = ESPACO;
    quadrado->py = HUD_H+ESPACO;

    //Aleatoriedade do quadrado
    if (resultado >= 30 && resultado <= 70) //40% de chance de o quadrado aparecer na tela
    {
        quadrado->visibilidade = true;

        if (resultado == intervalo_aleatorio(resultado-7, resultado+7)) //4% de chance de ser uma moeda extra
            quadrado->moeda_extra = true;
        else if (resultado == intervalo_aleatorio(resultado-20, resultado+20)) //2% de chance de ser uma bola extra
            quadrado->bola_extra = true;
        else
        {
            quadrado->valor = intervalo_aleatorio((1+incremento), (4+incremento));
            aplicar_cor_quadrado(quadrado);
            sprintf(quadrado->valor_texto, "%d", quadrado->valor);
        }
    }
}

void inserir_fileira (LISTA *lista_quadrado, LISTA *lista_bola)
{   
    QUADRADO *q = NULL; 
    q = (QUADRADO *) malloc(sizeof(QUADRADO));
    teste(q, "dinamica.c -> inserir_fileira");

    inicializar_quadrado(q, lista_bola->bola_kills);
    
    if (q->visibilidade)
        insere_quadrado(q, lista_quadrado);

    for (int i=1; i<QUADRADOS_POR_FILA; i++)
    {
        q = (QUADRADO *) malloc(sizeof(QUADRADO)); 
        teste(q, "dinamica.c -> inserir_fileira"); 
        
        inicializar_quadrado(q, lista_bola->bola_kills);
        q->px += i * (QUADRADO_SIZE + ESPACO);

        if (q->visibilidade)
            insere_quadrado(q, lista_quadrado);
    }

    lista_quadrado->qtd_quadrados+=7;
}

/*void inserir_fileira (LISTA *lista_quadrado, LISTA *lista_bola)
{   
    Infelizmente com essa função não estou conseguindo dar free nos quadrados depois
    
    QUADRADO *quadrados = (QUADRADO *) malloc(QUADRADOS_POR_FILA * sizeof(QUADRADO));
    teste(quadrados, "ballz.c->inicializar_lista");

    inicializar_quadrado(&quadrados[0], lista_bola->bola_kills);
    insere_quadrado(&quadrados[0], lista_quadrado);

    for (int i=1; i<QUADRADOS_POR_FILA; i++)
    {
        inicializar_quadrado(&quadrados[i], lista_bola->bola_kills);
        quadrados[i].px += i * (QUADRADO_SIZE + ESPACO);

        insere_quadrado(&quadrados[i], lista_quadrado);
    }

    lista_quadrado->qtd_quadrados+=7;
}*/

float distancia (float x1, float y1, float x2, float y2)
{
    return sqrt( (pow(x1-x2,2)) + (pow(y1-y2,2)) );
}

bool colisao (BOLA *bola, QUADRADO *quadrado)
{
    float x, y, raio, bx1, by1, bx2, by2;
    bool efeito_liberado = (!quadrado->bola_extra && !quadrado->moeda_extra);
    bool colidiu=false;

    x = bola->px;
    y = bola->py;
    raio = bola->r;
    bx1 = quadrado->px;
    by1 = quadrado->py;
    bx2 = quadrado->px+QUADRADO_SIZE;
    by2 = quadrado->py+QUADRADO_SIZE;

    if (!bola->colisao && bola->isenta_colisao<20)
    {
        bola->isenta_colisao++;
        return false;
    }

    if (bola->isenta_colisao>=20)
    {
        bola->colisao=true;
        bola->isenta_colisao=0;
        return false;
    }

    //Caso ela fure o quadrado
    if (x >= bx1 && x <= bx2)
    {
        if (y >= by1 && y <= by2)
        {
            bola->colisao=false;
            return false;
        }
    }

    //Círculo na região 1 e 2
    if (x >= bx1 && x <= bx2)
    {
        if ( ((y+raio)>by1) && ((y-raio)<by2) )
        {
            colidiu=true;

            if (efeito_liberado)
                bola->vy*=(-1);

            return colidiu;
        }
    }

    //Círculo na reigão 3 e 4
    if (y >= by1 && y <= by2)
    {
        if ( ((x+raio)>=bx1) && ((x-raio)<=bx2) )
        {
            colidiu=true;

            if (efeito_liberado)
                bola->vx*=(-1);

            return colidiu;
        }
    }

    //Círculo na região 5
    if (distancia(x,y,bx1,by1) <= raio)
    {
        colidiu=true;
        
        if (efeito_liberado)
        {
            if (bola->vx>0 && bola->vy>0)
            {
                bola->vy*=(-1);
                return colidiu;
            }

            if (bola->vx>0 && bola->vy<0)
            {
                bola->vx*=(-1);
                return colidiu;
            }

            return false;
        }
    }

    //Círculo na região 8
    if (distancia(x,y,bx2,by2) <= raio)
    {
        colidiu=true;
        
        if (efeito_liberado)
        {
            if (bola->vx<0 && bola->vy<0)
            {
                bola->vx*=(-1);
                return colidiu;
            }   
            
            if (bola->vx<0 && bola->vy>0)
            {
                bola->vx*=(-1);
                return colidiu;
            }

            return false;
        }
    }

    //Círculo na região 6 
    if (distancia(x,y,bx2,by1) <= raio)
    {
        colidiu=true;
        
        if (efeito_liberado)
        {
            if (bola->vx<0 && bola->vy<0)
            {
                bola->vx*=(-1);
                return colidiu;
            }

            if (bola->vx<0 && bola->vy>0)
            {
                bola->vx*=(-1);
                return colidiu;
            }

            return false;
        }
    }

    //Círculo na região 7
    if (distancia(x,y,bx1,by2) <= raio)
    {
        colidiu=true;
        
        if (efeito_liberado)
        {
            if (bola->vx>0 && bola->vy>0)
            {
                bola->vy*=(-1);
                return colidiu;
            }

            if (bola->vx>0 && bola->vy<0)
            {
                bola->vx*=(-1);
                return colidiu;
            }

            return false;
        }
    }

    return false;
}

//Função que eu estava trabalhando para melhorar o lançamento da bola
void limite_da_bola_v2 (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;
    float y, a, b;
    
    a = (-1)*aux->bola->cofa;
    b = aux->bola->cofl;
    y = a*aux->bola->px+b;

    while (aux != NULL)
    {
        bool 
            condicao_ax = ((aux->bola->vx>0 && aux->bola->vy<0) && (aux->bola->px > (BUFFER_W-aux->bola->r))),
            condicao_bx = ((aux->bola->vx>0 && aux->bola->vy>0) && (aux->bola->px > (BUFFER_W-aux->bola->r))),
            condicao_cx = ((aux->bola->vx<0 && aux->bola->vy<0) && (aux->bola->px < aux->bola->r)),
            condicao_dx = ((aux->bola->vx<0 && aux->bola->vy>0) && (aux->bola->px < aux->bola->r)),
            
            condicao_ay = ((aux->bola->vx<0 && aux->bola->vy<0) && (aux->bola->py < HUD_H)),
            condicao_by = ((aux->bola->vx>0 && aux->bola->vy<0) && (aux->bola->py < HUD_H)),
            condicao_cy = ((aux->bola->vx>0 && aux->bola->vy>0) && aux->bola->py > BUFFER_H),
            condicao_dy = ((aux->bola->vx<0 && aux->bola->vy>0) && (aux->bola->py > BUFFER_H));

        if (condicao_ax || condicao_bx || condicao_cx || condicao_dx)
        {
            aux->bola->vx*=(-1);
            aux->bola->em_trajetoria=false;
        }
        
        else if (condicao_ay || condicao_by || condicao_cy || condicao_dy)
        {
            aux->bola->vy*=(-1);
            aux->bola->em_trajetoria=false;
        }
        
        if (abs(a) > 1)
        {
            aux->bola->py += aux->bola->vy;
            aux->bola->px = (aux->bola->py-b)/a;
        }
        else
        {
            aux->bola->px += aux->bola->vx;
            aux->bola->py = y;
        }

        aux = aux->proximo;
    }
}

//Função que atualmente movimenta a bola após a trajetória
void limite_da_bola (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;

    while (aux != NULL)
    {
        bool 
            condicao_ax = ((aux->bola->vx>0 && aux->bola->vy<0) && (aux->bola->px > (BUFFER_W-aux->bola->r))),
            condicao_bx = ((aux->bola->vx>0 && aux->bola->vy>0) && (aux->bola->px > (BUFFER_W-aux->bola->r))),
            condicao_cx = ((aux->bola->vx<0 && aux->bola->vy<0) && (aux->bola->px < aux->bola->r)),
            condicao_dx = ((aux->bola->vx<0 && aux->bola->vy>0) && (aux->bola->px < aux->bola->r)),
            
            condicao_ay = ((aux->bola->vx<0 && aux->bola->vy<0) && (aux->bola->py < HUD_H)),
            condicao_by = ((aux->bola->vx>0 && aux->bola->vy<0) && (aux->bola->py < HUD_H)),
            condicao_cy = ((aux->bola->vx>0 && aux->bola->vy>0) && aux->bola->py > BUFFER_H),
            condicao_dy = ((aux->bola->vx<0 && aux->bola->vy>0) && (aux->bola->py > BUFFER_H));

        if (condicao_ax || condicao_bx || condicao_cx || condicao_dx)
        {
            aux->bola->vx*=(-1);
            aux->bola->em_trajetoria=false;
        }

        else if (condicao_ay || condicao_by || condicao_cy || condicao_dy)
        {
            aux->bola->vy*=(-1);
            aux->bola->em_trajetoria=false;
        }
        
        aux = aux->proximo;
    }
}

int gravidade_do_quadrado (LISTA *lista_quadrado)
{
    NODO *aux = lista_quadrado->primeiro_quadrado;

    while (aux != NULL)
    {
        aux->quadrado->py+=2;
        aux = aux->proximo;
    }

    return 2;
}

void atualizar_hud (HUD *hud, LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;
    int pontos = 0, moedas = 0;

    while (aux != NULL)
    {
        pontos += aux->bola->pontos;
        moedas += aux->bola->moedas;

        aux = aux->proximo;
    }

    hud->pontos = pontos;
    hud->moedas = moedas;
}

bool bolas_em_trajetoria (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;

    while (aux != NULL)
    {
        if (!aux->bola->em_trajetoria)
            return false;

        aux = aux->proximo;
    }

    return true;
}

void trajetoria_da_bola (LISTA *lista_bola, int x2, int y2)
{
    float a, b, y, x1, y1;
    int metade_da_tela=145, difdist=0;
    NODO *aux = lista_bola->primeira_bola;
    
    //Para garantir que a bola não será lançada ao pausar o jogo
    if (y2<HUD_H)
        return;
   
    while (aux != NULL)
    {
        aux->bola->vx = 4.5;
        aux->bola->vy = -4.5;   

        x1=aux->bola->px_origem;
        y1=aux->bola->py_origem;
        
        if (x2-x1 == 0)
            a = 0;
        else
            a = (y2-y1) / (x2-x1);

        b = y1-(a*x1);
        y = (a*aux->bola->px)+b;

        if (x2 < metade_da_tela && aux->bola->vx > 0)
            aux->bola->vx*=(-1);

        if (a == 0)
            aux->bola->py += aux->bola->vy;
        else
        {
            if (abs(a) > 1)
            {
                aux->bola->py += aux->bola->vy;
                aux->bola->px = (aux->bola->py-b)/a;
            }
            else
            {
                aux->bola->px += aux->bola->vx;
                aux->bola->py = y;
            }
        }

        if (aux->proximo)
            difdist = distancia(aux->bola->px, aux->bola->py, aux->proximo->bola->px, aux->proximo->bola->py);

        if (difdist>30 && aux->proximo)
            aux = aux->proximo;
        else
            return;
    }
}

void mover_bolas (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;

    while (aux != NULL)
    {
        aux->bola->px+=aux->bola->vx;
        aux->bola->py+=aux->bola->vy;

        aux = aux->proximo;
    }
}

void verificar_colisao (LISTA *lista_bola, LISTA *lista_quadrado, SOM *sons, HUD *hud)
{
    NODO *aux_q = lista_quadrado->primeiro_quadrado;
    NODO *aux_b = lista_bola->primeira_bola;

    while (aux_b != NULL)
    {
        while (aux_q != NULL)
        {
            //if (colisao(bola->px-bola->r, bola->py-bola->r, bola->px+bola->r, bola->py+bola->r, aux_q->quadrado->px, aux_q->quadrado->py, aux_q->quadrado->px+QUADRADO_SIZE, aux_q->quadrado->py+QUADRADO_SIZE))
            if (aux_q->quadrado->visibilidade && colisao(aux_b->bola, aux_q->quadrado))
            {
                if (aux_q->quadrado->bola_extra)
                {
                    aux_b->bola->multiplicador++;
                    aux_q->quadrado->valor=0;
                    return;
                }

                if (aux_q->quadrado->moeda_extra)
                {
                    al_play_sample(sons->coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    aux_b->bola->moedas++;
                    aux_q->quadrado->valor=0;
                    return;
                }

                al_play_sample(sons->colisao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                aux_b->bola->em_trajetoria=false;
                aux_b->bola->pontos++;
                hud->pontos_por_partida++;
                aux_q->quadrado->valor--;
                sprintf(aux_q->quadrado->valor_texto, "%d", aux_q->quadrado->valor);
            }

            aux_q = aux_q->proximo;
        }

        aux_q = lista_quadrado->primeiro_quadrado;
        aux_b = aux_b->proximo;
    }
}

void atualizar_quadrados (LISTA *lista_quadrado, LISTA *lista_bola)
{
    NODO *aux = lista_quadrado->primeiro_quadrado;

    while (aux != NULL)
    {
        if (aux->quadrado->visibilidade && aux->quadrado->valor<=0)
        {
            aux->quadrado->visibilidade=false;
            
            if (!aux->quadrado->bola_extra)
                lista_bola->bola_kills++;
        }

        aux = aux->proximo;
    }
}

void caiu_no_chao (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;

    while (aux != NULL)
    {
        if (aux->bola->py>535)
        {
            aux->bola->vx=0;
            aux->bola->vy=0;
        }

        aux = aux->proximo;
    }
}

bool bolas_no_chao (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;

    while (aux != NULL)
    {
        if (aux->bola->vx==0 && aux->bola->vy==0)
            aux = aux->proximo;
        else
            return false;
    }

    return true;
}

void atualizar_bola (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;
    int novas_bolas = 0;
    bool primeira=true;
    float nova_posicao=0;

    while (aux != NULL)
    {
        novas_bolas += aux->bola->multiplicador;
        aux->bola->em_trajetoria = true;
        
        if (primeira) {
            nova_posicao = aux->bola->px;
            primeira=false;   
        }

        if (nova_posicao<10)
            nova_posicao+=5;
        
        if (nova_posicao>BUFFER_W-10)
            nova_posicao-=5;
        
        aux->bola->px = nova_posicao;
        aux->bola->py = aux->bola->py_origem;
        aux->bola->multiplicador = 0;
        
        aux = aux->proximo;
    }

    for (int i=0; i<novas_bolas; i++)
    {
        BOLA *bola = (BOLA *) malloc(sizeof(BOLA));
        teste(bola, "dinamica.c -> atualizar_bola");
        
        bola->r = 5.0;
        bola->px_origem = 0;
        bola->py_origem = 0;
        bola->px = nova_posicao;
        bola->py = 535;
        bola->vx = 0;
        bola->vy = 0;
        bola->multiplicador = 0;
        bola->moedas = 0;
        bola->em_trajetoria = true;
        bola->cor = COR_BRANCO;
        bola->pontos = 0;
        
        lista_bola->qtd_bolas++;
        insere_bola(bola, lista_bola);
    }
}

void conta_bolas (LISTA *lista_bola)
{
    NODO *aux = lista_bola->primeira_bola;
    int c = 0;
    
    while (aux != NULL)
    {
        ++c;
        aux = aux->proximo;
    }
}

bool fim_de_jogo (LISTA *lista_quadrado)
{
    NODO *aux = lista_quadrado->primeiro_quadrado;

    while (aux != NULL)
    {
        if (aux->quadrado->visibilidade)
            if (aux->quadrado->py+QUADRADO_SIZE>515)
                return true;
        
        aux = aux->proximo;
    }

    return false;
}

/*
    QUI 24/11/2022 -> Soma dos numeros da data de estreia do brasil na copa = 2+4+1+1+2+0+2+2 = 14
    Soma das datas + 6 do hexa = 20, número usado para comparar
*/
bool verificar_surpresa (HUD *hud, bool desenho_surpresa)
{
    if (!desenho_surpresa)
    {
        if (hud->moedas==6 && hud->pontos_por_partida>19)
        {
            hud->pontos_por_partida=0;
            return true;
        }
    }

    hud->pontos_por_partida=0;
    return false;
}
