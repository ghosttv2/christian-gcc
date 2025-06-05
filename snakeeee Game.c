// snake.c - Version personnalisée avec bonhomme mince qui prend du muscle
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define CELL_SIZE 20
#define GRID_WIDTH 40
#define GRID_HEIGHT 30

typedef struct {
    int x, y;
} Vector2Int;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

// MAX_LENGTH du bonhomme (serpent modifié)
#define MAX_LENGTH 100

int main() {
    const int screenWidth = CELL_SIZE * GRID_WIDTH;
    const int screenHeight = CELL_SIZE * GRID_HEIGHT;

    InitWindow(screenWidth, screenHeight, "Snake Bodybuilder Edition");
    InitAudioDevice();
    SetTargetFPS(10);

    // Chargement des ressources
    Texture2D spriteMince = LoadTexture("mince.png");
    Texture2D spriteMuscle = LoadTexture("muscle.png");
    Texture2D spritePill = LoadTexture("pill.png");
    Sound eatSound = LoadSound("eat.wav");

    // Initialisation du joueur
    Vector2Int bonhomme[MAX_LENGTH];
    int taille = 3;
    bonhomme[0] = (Vector2Int){ GRID_WIDTH / 2, GRID_HEIGHT / 2 };
    Direction dir = RIGHT;

    // Fruit
    Vector2Int fruit = { GetRandomValue(0, GRID_WIDTH - 1), GetRandomValue(0, GRID_HEIGHT - 1) };
    bool bonus = false;

    int score = 0;
    int speed = 30;
    int frameCounter = 0;
    int decayTimer = 0;

    while (!WindowShouldClose()) {
        // --- INPUT ---
        if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
        if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
        if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
        if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;

        // --- MOUVEMENT ---
        if (frameCounter++ >= (60 / speed)) {
            frameCounter = 0;

            // décaler les cases
            for (int i = taille; i > 0; i--) bonhomme[i] = bonhomme[i - 1];

            // nouvelle position
            switch (dir) {
                case UP: bonhomme[0].y--; break;
                case DOWN: bonhomme[0].y++; break;
                case LEFT: bonhomme[0].x--; break;
                case RIGHT: bonhomme[0].x++; break;
            }

            // collision murs
            if (bonhomme[0].x < 0 || bonhomme[0].x >= GRID_WIDTH ||
                bonhomme[0].y < 0 || bonhomme[0].y >= GRID_HEIGHT) break;

            // collision soi-même
            for (int i = 1; i < taille; i++) {
                if (bonhomme[0].x == bonhomme[i].x && bonhomme[0].y == bonhomme[i].y) break;
            }

            // manger
            if (bonhomme[0].x == fruit.x && bonhomme[0].y == fruit.y) {
                PlaySound(eatSound);
                int gain = bonus ? 3 : 1;
                taille += gain;
                score += gain;
                fruit.x = GetRandomValue(0, GRID_WIDTH - 1);
                fruit.y = GetRandomValue(0, GRID_HEIGHT - 1);
                bonus = (GetRandomValue(0, 9) == 0);
                speed++;
            } else {
                // pas mangé, on réduit
                if (taille > 1 && ++decayTimer >= 100) {
                    taille--;
                    decayTimer = 0;
                }
            }
        }

        // --- DESSIN ---
        BeginDrawing();
        ClearBackground((Color){30, 30, 30, 255});

        // Fond animé simple
        for (int i = 0; i < 50; i++) {
            DrawCircle((i * 83 + frameCounter * 2) % screenWidth, (i * 53) % screenHeight, 2, DARKGRAY);
        }

        // Fruit / pillule
        DrawTexture(spritePill, fruit.x * CELL_SIZE, fruit.y * CELL_SIZE, bonus ? RED : GOLD);

        // Affichage du bonhomme
        Texture2D current = (taille < 5) ? spriteMince : spriteMuscle;
        for (int i = 0; i < taille; i++) {
            DrawRectangle(bonhomme[i].x * CELL_SIZE, bonhomme[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIME);

        }

        DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
        DrawRectangle(fruit.x * CELL_SIZE, fruit.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, bonus ? RED : GOLD);

        EndDrawing();
    }

    // Libérer
    UnloadTexture(spriteMince);
    UnloadTexture(spriteMuscle);
    UnloadTexture(spritePill);
    UnloadSound(eatSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
