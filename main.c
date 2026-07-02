//----------------------------------------------------------------------------------
// Libraries
//----------------------------------------------------------------------------------
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define MAX_POSITION 5 
#define MAX_DECK 60

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Cartas {
    Texture2D textura;
    Vector2 posicao;
} Cartas;

typedef struct Posicoes {
    Rectangle posicao;
    Cartas carregada;
    bool vazia;
} Posicoes;

typedef struct Deck {
    Rectangle deck_area;
    Cartas deck[MAX_DECK];
} Deck;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    //------------------------------------------------------------------------------
    // Window Initialization
    //------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "CardGameTestFuctions");
    SetTargetFPS(60);  
    
    //------------------------------------------------------------------------------
    // Game Entities Initialization
    //------------------------------------------------------------------------------
    //Instancia o deck
    Deck deck;
    deck.deck_area = (Rectangle){screenWidth-50, screenHeight-50, 50, 50};

    //Instancia as cartas
    Image imagem = LoadImage("images/azul.png");
    ImageResize(&imagem, 40, 40); 
    for(int i=0;i<MAX_DECK;i++){
        deck.deck[i].textura = LoadTextureFromImage(imagem);  
    }
    UnloadImage(imagem); 

    //Instancia o estado de ação e controle do mouse
    Vector2 mousePosition = {-100, -100};
    Rectangle areaRec = {-100, -100};
    bool pull = false;
    int carta_num = 0;

    // Cria e define a posição das áreas do campo
    Posicoes areas[MAX_POSITION]; 
    for(int i = 0; i<MAX_POSITION; i++){
        areas[i].posicao.x = i*screenWidth/MAX_POSITION;
        areas[i].posicao.y = 3*screenHeight/4;
        areas[i].posicao.width = 80;
        areas[i].posicao.height =120;
        areas[i].vazia = true;
    }

    //------------------------------------------------------------------------------
    // Main Game Loop
    //------------------------------------------------------------------------------
    while(!WindowShouldClose()){
        // 1. Update / Input Handling
        //--------------------------------------------------------------------------
        mousePosition = GetMousePosition();
        areaRec = (Rectangle){mousePosition.x-0.25, mousePosition.y-0.25, 0.5,0.5};

        if (pull == true){
            deck.deck[carta_num].posicao = (Vector2){mousePosition.x-20, mousePosition.y-20};
        }

        // Clique com botão esquerdo
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

        // Clique com botão esquerdo
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            for(int i = 0; i<MAX_POSITION; i++){
                if((CheckCollisionRecs(areas[i].posicao, areaRec))){
                    areas[i].vazia = true;
                    areas[i].carregada = (Cartas){0};
                }
            }
        }

        // 2. Rendering
        //--------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(WHITE);

            // Interface (Texto do Deck)
            char ammoText[16];
            snprintf(ammoText, sizeof(ammoText), "DECK: %d/%d", MAX_DECK-carta_num, MAX_DECK);
            DrawText(ammoText, 6.7*screenWidth/8, 20, 20, BLACK);

            // Desenha a área do Deck
            DrawRectangle(deck.deck_area.x, deck.deck_area.y, deck.deck_area.width, deck.deck_area.height, GREEN);

            // Desenha as posições do campo e as cartas carregadas nelas
            for(int i=0;i<MAX_POSITION;i++){
                DrawRectangle(areas[i].posicao.x, areas[i].posicao.y, areas[i].posicao.width, areas[i].posicao.height, RED);
                if(areas[i].vazia == false){
                    DrawTexture(areas[i].carregada.textura, areas[i].posicao.x, areas[i].posicao.y, WHITE);
                }
            }

            // Desenha a carta sendo arrastada (se houver)
            if (pull == true){
                DrawTexture(deck.deck[carta_num].textura, deck.deck[carta_num].posicao.x, deck.deck[carta_num].posicao.y, WHITE);
            }

            // Desenha o hitbox do mouse
            DrawRectangle(areaRec.x, areaRec.y, areaRec.width, areaRec.height, BLACK);
        EndDrawing();
    }

    //------------------------------------------------------------------------------    
    // De-Initialization / Unload Resources
    //------------------------------------------------------------------------------
    for(int i=0;i<MAX_DECK;i++){
        UnloadTexture(deck.deck[i].textura);
    }     

    CloseWindow();
    return 0;
}
