#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define MAX_POSITION 5


typedef struct Cartas{
    Image imagem;
    Texture2D textura;
    Vector2 posicao;
}Cartas;

typedef struct Posicoes{
    Rectangle posicao;
    Cartas carregada;
    bool vazia;
}Posicoes;

typedef struct Deck{
    Rectangle deck_area;
    Cartas deck[60];
}Deck;


int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "testes");
    SetTargetFPS(60);  
    
    Cartas carta;
    carta.imagem = LoadImage("images/azul.png");
    ImageResize(&carta.imagem, 40, 40); 
    carta.textura = LoadTextureFromImage(carta.imagem);  
    UnloadImage(carta.imagem); 

    Vector2 mousePosition = {-100, -100};
    Rectangle areaRec = {-100, -100};

    Deck deck;
    deck.deck_area = (Rectangle){screenWidth-50, screenHeight-50, 50, 50};

    
    bool pull = false;
    Posicoes areas[5];
    for(int i = 0; i<MAX_POSITION; i++){
        areas[i].posicao.x = i*screenWidth/5;
        areas[i].posicao.y = 3*screenHeight/4;
        areas[i].posicao.width = 80;
        areas[i].posicao.height =120;
        areas[i].vazia = true;
    }

    while(!WindowShouldClose()){
        mousePosition = GetMousePosition();
        areaRec = (Rectangle){mousePosition.x-0.25, mousePosition.y-0.25, 0.5,0.5};
        if (pull == true){
            carta.posicao = (Vector2){mousePosition.x-20, mousePosition.y-20};
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if((CheckCollisionRecs(deck.deck_area, areaRec))){
                pull = true;
            }
            if (pull == true){
                for(int i = 0; i<5; i++){
                    if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                        if(areas[i].vazia == true){
                            pull = false;
                            areas[i].carregada = carta;
                            areas[i].vazia = false;
                        }
                    }
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            for(int i = 0; i<5; i++){
                if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                    areas[i].vazia = true;
                    areas[i].carregada = (Cartas){0};
                }
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangle(deck.deck_area.x, deck.deck_area.y, deck.deck_area.width, deck.deck_area.height, GREEN);
            for(int i=0;i<MAX_POSITION;i++){
                DrawRectangle(areas[i].posicao.x, areas[i].posicao.y, areas[i].posicao.width, areas[i].posicao.height, RED);
                if(areas[i].vazia == false){
                    DrawTexture(areas[i].carregada.textura, areas[i].posicao.x, areas[i].posicao.y, WHITE);
                }
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
