#include "snake.h"
#include <stdlib.h>
#include <time.h>

int main(){
    Jogo jogo;
    int gameOver = 1;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    CarregaImagens();
    SetTargetFPS(60);
    srand(time(NULL));
    
    IniciaJogo(&jogo);
    
    while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    if (gameOver){
        DesenhaJogo(&jogo);


        DrawText(TextFormat("Score: %d", jogo.score), 20, 20, 30, WHITE);

        AtualizaRodada(&jogo);

        if (ColisaoFood(&jogo)){
            jogo.crescer = 1;
            IniciaFood(&jogo);
            jogo.score++;
        }
        if(ColisaoBordas(&jogo)){
            gameOver=0;
        }
        if(ColisaoCorpo(&jogo)){
            gameOver=0;
        }
    } else {
        
        DrawText("GameOver!", 220, 200, 40, WHITE);
        DrawText(TextFormat("Score final: %d", jogo.score), 220, 300, 30, WHITE);
        DrawText("Aperte ENTER para reiniciar", 110, 450, 30, WHITE);

        if (IsKeyPressed(KEY_ENTER)){
            LiberaSnake(&jogo);
            IniciaJogo(&jogo);
            gameOver = 1;
        }
    }
   
    EndDrawing();
}
    LiberaImagens();
    CloseWindow();
    return 0;
}