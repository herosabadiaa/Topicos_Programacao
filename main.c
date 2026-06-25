#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


typedef struct Cartas{
    Image imagem;
    Texture2D textura;
    Vector2 posicao;
}Cartas;

typedef struct Posicoes{
    Rectangle posicao;
    Cartas carregada;
}Posicoes;


int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "testes");
    SetTargetFPS(60);  
    
    Cartas carta;
    carta.imagem = LoadImage("images/azul.png");
    carta.textura = LoadTextureFromImage(carta.imagem);  
    UnloadImage(carta.imagem); 

    Vector2 mousePosition = {-100, -100};
    Rectangle areaRec = {-100, -100};
    Rectangle deck = {screenWidth-100, screenHeight-100, screenWidth/6, screenHeight/4};
    bool pull = false;
    Posicoes areas[5];
    for(int i = 0; i<5; i++){
        areas[i].posicao.x = i*screenWidth/5;
        areas[i].posicao.y = 3*screenHeight/4;
        areas[i].posicao.width = 80;
        areas[i].posicao.height =120;
    }

    while(!WindowShouldClose()){
        mousePosition = GetMousePosition();
        areaRec = (Rectangle){mousePosition.x-10, mousePosition.y-10, 20,20};
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if((CheckCollisionRecs(deck, areaRec))){
                pull = true;
            }
            for(int i = 0; i<5; i++){
                if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                    pull = false;
                }
            }
        }
        if (pull == true){
            carta.posicao = (Vector2){mousePosition.x-carta.textura.width/2, mousePosition.y - carta.textura.height/2};
        }
           
        
        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangle(deck.x, deck.y, deck.width, deck.height, BLUE);
            for(int i=0;i<5;i++){
                DrawRectangle(areas[i].posicao.x, areas[i].posicao.y, areas[i].posicao.width, areas[i].posicao.height, RED);
            }
            if (pull == true){
                DrawTexture(carta.textura, carta.posicao.x, carta.posicao.y, WHITE);
            }
            DrawRectangle(areaRec.x, areaRec.y, areaRec.width, areaRec.height, BLACK);
        EndDrawing();
    }

    UnloadTexture(carta.textura);       
    CloseWindow();
    return 0;
}
