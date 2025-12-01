#include "snake.h"
#include <stdlib.h>

Texture2D maca;

void CarregaImagens(){
maca= LoadTexture("./assets/maca.png");
}


void IniciaBody(Jogo *j){
    Segmento *novo = malloc(sizeof(Segmento));
    novo->pos = (Rectangle){LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y - 10, STD_SIZE_X, STD_SIZE_Y};
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
    } while (!valido);
    
    j->food.color = FOOD_COLOR;
}

void LiberaImagens(){
    UnloadTexture(maca);
}

void IniciaJogo(Jogo *j){
    IniciaBordas(j);
    IniciaBody(j);
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

void DesenhaJogo(Jogo *j){
    DesenhaBordas(j);
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

    if (j->direcao == 0) j->head->pos.y -= STD_SIZE_Y;
    if (j->direcao == 1) j->head->pos.x += STD_SIZE_X;
    if (j->direcao == 2) j->head->pos.y += STD_SIZE_Y;
    if (j->direcao == 3) j->head->pos.x -= STD_SIZE_X;

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

int ColisaoBordas(Jogo *j){
    for (int i = 0; i < 4; i++){
        if (CheckCollisionRecs(j->head->pos, j->bordas[i].pos)){
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