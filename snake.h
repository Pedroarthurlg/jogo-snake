#ifndef SNAKE_H
#define SNAKE_H

#include "raylib.h"

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TEMPO 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR_HEAD GREEN 
#define SNAKE_COLOR DARKGREEN
#define FOOD_COLOR BLACK


typedef struct Segmento {
    Rectangle pos;
    struct Segmento *prox;
} Segmento;

typedef struct Bordas{
    Rectangle pos;
} Bordas;

typedef struct Food{
    Rectangle pos;
    Color color;
    Texture2D textura;
} Food;

typedef struct Jogo{
    Segmento *head;
    Segmento *cauda;
    Food food;
    Bordas bordas[4];
    double tempo;
    double cooldown;
    int direcao;
    int crescer;
    int score;
} Jogo;

void IniciaBody(Jogo *j);
void AumentaBody(Jogo * j);
void IniciaBordas(Jogo *j);
void IniciaFood(Jogo *j);
void IniciaJogo(Jogo *j);
void DesenhaBody(Jogo *j);
void DesenhaFood(Jogo *j);
void DesenhaBordas(Jogo *j);
void DesenhaJogo(Jogo *j);
void AtualizaDirecao(Jogo *j);
void MoveSnake(Jogo *j);
void AtualizaRodada(Jogo *j);
int ColisaoFood(Jogo *j);
int ColisaoBordas(Jogo *j);
int ColisaoCorpo(Jogo *j);
void CarregaImagens();
void LiberaImagens();
void LiberaSnake(Jogo *j);

#endif