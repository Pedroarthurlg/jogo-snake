#include "snake.h"
#include <stdlib.h>

Texture2D maca;

void CarregaImagens(){
    maca = LoadTexture("./assets/maca.png");
}

void IniciaBody(Jogo *j){
    Segmento *novo = malloc(sizeof(Segmento));
    novo->pos = (Rectangle){LARGURA/2 - STD_SIZE_X, ALTURA/2, STD_SIZE_X, STD_SIZE_Y};
    novo->prox = NULL;
    j->head = novo;
    j->cauda = novo;
}

void IniciaBordas(Jogo *j){
    j->bordas[0].pos = (Rectangle){0, 0, LARGURA, 10};
    j->bordas[1].pos = (Rectangle){LARGURA - 10, 0, 10, ALTURA};
    j->bordas[2].pos = (Rectangle){0, ALTURA - 10, LARGURA, 10};
    j->bordas[3].pos = (Rectangle){0, 0, 10, ALTURA};
}

void IniciaBarreiras(Jogo *j) {
    j->num_barreiras = 0;
    
    // Padrão de barreiras nas bordas (como um quadro decorativo)
    // Define quantos blocos de parede e quantos espaços vazios
    int espacamento = 3; // blocos vazios entre barreiras
    int tamanho_barreira = 2; // blocos de barreira
    
    // Borda superior
    for (int x = 10; x < LARGURA - 10; x += (tamanho_barreira + espacamento) * STD_SIZE_X) {
        if (j->num_barreiras < MAX_BARREIRAS) {
            j->barreiras[j->num_barreiras].pos = (Rectangle){
                x, 10, 
                STD_SIZE_X * tamanho_barreira, 
                STD_SIZE_Y * tamanho_barreira
            };
            j->barreiras[j->num_barreiras].color = RED;
            j->num_barreiras++;
        }
    }
    
    // Borda inferior
    for (int x = 10; x < LARGURA - 10; x += (tamanho_barreira + espacamento) * STD_SIZE_X) {
        if (j->num_barreiras < MAX_BARREIRAS) {
            j->barreiras[j->num_barreiras].pos = (Rectangle){
                x, 
                ALTURA - 10 - STD_SIZE_Y * tamanho_barreira, 
                STD_SIZE_X * tamanho_barreira, 
                STD_SIZE_Y * tamanho_barreira
            };
            j->barreiras[j->num_barreiras].color = RED;
            j->num_barreiras++;
        }
    }
    
    // Borda esquerda
    for (int y = 10 + STD_SIZE_Y * tamanho_barreira; y < ALTURA - 10 - STD_SIZE_Y * tamanho_barreira; y += (tamanho_barreira + espacamento) * STD_SIZE_Y) {
        if (j->num_barreiras < MAX_BARREIRAS) {
            j->barreiras[j->num_barreiras].pos = (Rectangle){
                10, y, 
                STD_SIZE_X * tamanho_barreira, 
                STD_SIZE_Y * tamanho_barreira
            };
            j->barreiras[j->num_barreiras].color = RED;
            j->num_barreiras++;
        }
    }
    
    // Borda direita
    for (int y = 10 + STD_SIZE_Y * tamanho_barreira; y < ALTURA - 10 - STD_SIZE_Y * tamanho_barreira; y += (tamanho_barreira + espacamento) * STD_SIZE_Y) {
        if (j->num_barreiras < MAX_BARREIRAS) {
            j->barreiras[j->num_barreiras].pos = (Rectangle){
                LARGURA - 10 - STD_SIZE_X * tamanho_barreira, 
                y, 
                STD_SIZE_X * tamanho_barreira, 
                STD_SIZE_Y * tamanho_barreira
            };
            j->barreiras[j->num_barreiras].color = RED;
            j->num_barreiras++;
        }
    }
}

void IniciaFood(Jogo *j) {
    int valido;
    do {
        valido = 1;
        j->food.pos = (Rectangle){
            (float)(rand() % ((LARGURA - 30) / STD_SIZE_X) * STD_SIZE_X + 10),
            (float)(rand() % ((ALTURA - 30) / STD_SIZE_Y) * STD_SIZE_Y + 10),
            STD_SIZE_X, 
            STD_SIZE_Y
        };
        
        // Verifica se não está na cobra
        Segmento *aux = j->head;
        while (aux != NULL) {
            if (CheckCollisionRecs(j->food.pos, aux->pos)) {
                valido = 0;
                break;
            }
            aux = aux->prox;
        }
        
        // Verifica se não está nas barreiras
        if (valido) {
            for (int i = 0; i < j->num_barreiras; i++) {
                if (CheckCollisionRecs(j->food.pos, j->barreiras[i].pos)) {
                    valido = 0;
                    break;
                }
            }
        }
    } while (!valido);
    
    j->food.color = FOOD_COLOR;
}

void LiberaImagens(){
    UnloadTexture(maca);
}

void IniciaJogo(Jogo *j){
    IniciaBordas(j);
    IniciaBody(j);
    j->num_barreiras = 0; // Inicializa antes de criar barreiras
    IniciaBarreiras(j);
    IniciaFood(j);
    j->tempo = GetTime();
    j->direcao = 0;
    j->crescer = 0;
    j->score = 0;
}

void DesenhaBody(Jogo *j){
    Segmento *aux = j->head;
    DrawRectangleRec(aux->pos, SNAKE_COLOR_HEAD);
    aux = aux->prox;
    
    while (aux != NULL){
        DrawRectangleRec(aux->pos, SNAKE_COLOR);
        aux = aux->prox;
    }
}

void DesenhaFood(Jogo *j){
    DrawTexture(maca, j->food.pos.x, j->food.pos.y, RAYWHITE);
}

void DesenhaBordas(Jogo *j){
    for (int i = 0; i < 4; i++){
        DrawRectangleRec(j->bordas[i].pos, LIGHTGRAY);
    }
}

void DesenhaBarreiras(Jogo *j){
    for (int i = 0; i < j->num_barreiras; i++){
        // Desenha a barreira principal
        DrawRectangleRec(j->barreiras[i].pos, j->barreiras[i].color);
        // Desenha detalhes em branco (padrão xadrez/listras)
        for (int bx = 0; bx < j->barreiras[i].pos.width; bx += STD_SIZE_X) {
            for (int by = 0; by < j->barreiras[i].pos.height; by += STD_SIZE_Y) {
                // Alterna entre vermelho e branco
                if ((bx / STD_SIZE_X + by / STD_SIZE_Y) % 2 == 0) {
                    DrawRectangle(
                        j->barreiras[i].pos.x + bx, 
                        j->barreiras[i].pos.y + by, 
                        STD_SIZE_X, STD_SIZE_Y, 
                        WHITE
                    );
                } else {
                    DrawRectangle(
                        j->barreiras[i].pos.x + bx, 
                        j->barreiras[i].pos.y + by, 
                        STD_SIZE_X, STD_SIZE_Y, 
                        RED
                    );
                }
            }
        }
    }
}

void DesenhaJogo(Jogo *j){
    DesenhaBordas(j);
    DesenhaBarreiras(j);
    DesenhaBody(j);
    DesenhaFood(j);
}

void AtualizaDirecao(Jogo *j){
    if(IsKeyDown(KEY_UP) && j->direcao != 2 && GetTime() - j->cooldown > COOLDOWN){
        j->direcao = 0; j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_RIGHT) && j->direcao != 3 && GetTime() - j->cooldown > COOLDOWN){
        j->direcao = 1; j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_DOWN) && j->direcao != 0 && GetTime() - j->cooldown > COOLDOWN){
        j->direcao = 2; j->cooldown = GetTime();
    }
    if(IsKeyDown(KEY_LEFT) && j->direcao != 1 && GetTime() - j->cooldown > COOLDOWN){
        j->direcao = 3; j->cooldown = GetTime();
    }
}

void MoveSnake(Jogo *j){
    Segmento *novo = malloc(sizeof(Segmento));
    novo->pos = j->head->pos;
    novo->prox = j->head;
    j->head = novo;

    // Movimento normal
    if (j->direcao == 0) j->head->pos.y -= STD_SIZE_Y;
    if (j->direcao == 1) j->head->pos.x += STD_SIZE_X;
    if (j->direcao == 2) j->head->pos.y += STD_SIZE_Y;
    if (j->direcao == 3) j->head->pos.x -= STD_SIZE_X;

    // Efeito wrap-around (aparecer do outro lado)
    // Borda superior/inferior
    if (j->head->pos.y < 10) {
        j->head->pos.y = ALTURA - STD_SIZE_Y - 10;
    } else if (j->head->pos.y >= ALTURA - 10) {
        j->head->pos.y = 10;
    }
    
    // Borda esquerda/direita
    if (j->head->pos.x < 10) {
        j->head->pos.x = LARGURA - STD_SIZE_X - 10;
    } else if (j->head->pos.x >= LARGURA - 10) {
        j->head->pos.x = 10;
    }

    // Remove o último segmento se não estiver crescendo
    if (!j->crescer){
        Segmento *aux = j->head;
        while (aux->prox != j->cauda)
            aux = aux->prox;
        free(j->cauda);
        j->cauda = aux;
        j->cauda->prox = NULL;
    } else {
        j->crescer = 0;
    }
}

void AtualizaRodada(Jogo *j){
    AtualizaDirecao(j);
    if (GetTime() - j->tempo > TEMPO){
        MoveSnake(j);
        j->tempo = GetTime();
        j->cooldown = COOLDOWN;
    }
}

int ColisaoFood(Jogo *j){
    return CheckCollisionRecs(j->head->pos, j->food.pos);
}

int ColisaoBarreiras(Jogo *j){
    for (int i = 0; i < j->num_barreiras; i++){
        if (CheckCollisionRecs(j->head->pos, j->barreiras[i].pos)){
            return 1;
        }
    }
    return 0;
}

int ColisaoCorpo(Jogo *j){
    Segmento *aux = j->head->prox;
    while (aux != NULL){
        if (CheckCollisionRecs(j->head->pos, aux->pos)){
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
}

void LiberaSnake(Jogo *j) {
    Segmento *aux = j->head;
    
    while (aux != NULL) {
        Segmento *temp = aux;
        aux = aux->prox;
        free(temp);
    }
    
    // Importante: reseta os ponteiros
    j->head = NULL;
    j->cauda = NULL;
}