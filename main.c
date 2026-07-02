#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define MAX_POSITION 5 //Define o maximo de areas disponiveis em campo
#define MAX_DECK 60

typedef struct Cartas{ //Cria a estrutura de uma carta
    Texture2D textura;
    Vector2 posicao;
}Cartas;

typedef struct Posicoes{//Cria a estrutura de uma area do campo
    Rectangle posicao;
    Cartas carregada;
    bool vazia;
}Posicoes;

typedef struct Deck{//Criar a estrutura do deck
    Rectangle deck_area;
    Cartas deck[MAX_DECK];
}Deck;


int main(void){
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "testes");
    SetTargetFPS(60);  
    
    //Instancia o deck
    Deck deck;
    deck.deck_area = (Rectangle){screenWidth-50, screenHeight-50, 50, 50};

    //Define as cartas no deck
    Image imagem = LoadImage("images/azul.png");
    ImageResize(&imagem, 40, 40); 
    
    for(int i=0;i<MAX_DECK;i++){
        deck.deck[i].textura = LoadTextureFromImage(imagem);  
    }
    UnloadImage(imagem); 

    Vector2 mousePosition = {-100, -100};
    Rectangle areaRec = {-100, -100};

    
    bool pull = false; //Bool usado para verificar se o usuario esta com a carta na mão
    Posicoes areas[MAX_POSITION]; //Cria a posição das areas do campo
    for(int i = 0; i<MAX_POSITION; i++){
        areas[i].posicao.x = i*screenWidth/MAX_POSITION;
        areas[i].posicao.y = 3*screenHeight/4;
        areas[i].posicao.width = 80;
        areas[i].posicao.height =120;
        areas[i].vazia = true;
    }
    int carta_num = 0;


    while(!WindowShouldClose()){
        mousePosition = GetMousePosition();//Pega a posicão do mouse
        areaRec = (Rectangle){mousePosition.x-0.25, mousePosition.y-0.25, 0.5,0.5};//Define um quadrado em volta do mouse
        if (pull == true){
            deck.deck[carta_num].posicao = (Vector2){mousePosition.x-20, mousePosition.y-20};
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(carta_num < MAX_DECK){
                if((CheckCollisionRecs(deck.deck_area, areaRec))){
                    pull = true;
                }
            }
            if (pull == true){
                for(int i = 0; i<MAX_POSITION; i++){
                    if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                        if(areas[i].vazia == true){
                            pull = false;
                            areas[i].carregada = deck.deck[carta_num];
                            areas[i].vazia = false;
                            carta_num += 1;
                        }
                    }
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            for(int i = 0; i<MAX_POSITION; i++){
                if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                    areas[i].vazia = true;
                    areas[i].carregada = (Cartas){0};
                }
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);

            char ammoText[16];
            snprintf(ammoText, sizeof(ammoText), "DECK: %d/%d", MAX_DECK-carta_num, MAX_DECK);
            DrawText(ammoText, 6.7*screenWidth/8, 20, 20, BLACK);

            DrawRectangle(deck.deck_area.x, deck.deck_area.y, deck.deck_area.width, deck.deck_area.height, GREEN);

            for(int i=0;i<MAX_POSITION;i++){
                DrawRectangle(areas[i].posicao.x, areas[i].posicao.y, areas[i].posicao.width, areas[i].posicao.height, RED);
                if(areas[i].vazia == false){
                    DrawTexture(areas[i].carregada.textura, areas[i].posicao.x, areas[i].posicao.y, WHITE);
                }
            }

            if (pull == true){
                DrawTexture(deck.deck[carta_num].textura, deck.deck[carta_num].posicao.x, deck.deck[carta_num].posicao.y, WHITE);
            }

            DrawRectangle(areaRec.x, areaRec.y, areaRec.width, areaRec.height, BLACK);
        EndDrawing();
    }

    for(int i=0;i<MAX_DECK;i++){
    UnloadTexture(deck.deck[i].textura);
    }     

    CloseWindow();
    return 0;
}
