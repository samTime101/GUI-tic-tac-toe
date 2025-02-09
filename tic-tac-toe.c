#include "../../include/raylib.h"
// #include "../../include/raymath.h"
#include <stdio.h>
// #include <float.h>
#include <limits.h>
#define COLS 3
#define ROWS 3
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_WIDTH (SCREEN_WIDTH / COLS)
#define CELL_HEIGHT (SCREEN_HEIGHT / ROWS)
#define FPS 60

int check_for_win(int (*board)[3], int mark);
int check_for_draw(int (*board)[3]);

int check_for_win(int (*board)[3], int mark) {

  int winning_conditions[8][3][2] = {
      {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}},
      {{2, 0}, {2, 1}, {2, 2}}, {{0, 0}, {1, 0}, {2, 0}},
      {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}},
      {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}};
  for (int i = 0; i < 8; i++) {
    int condition_met = 1;
    for (int j = 0; j < 3; j++) {
      int row = winning_conditions[i][j][0];
      int col = winning_conditions[i][j][1];
      if (board[row][col] != mark) {
        condition_met = 0;
        break;
      }
    }
    if (condition_met) {
      return 1;
    }
  }

  return 0;
}

int check_for_draw(int (*board)[3]) {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == 0) {
        return 0;
      }
    }
  }
  return 1;
}
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

int minimax(int (*board)[3], int depth, int is_maximizing) {

  if (check_for_win(board, 1))
    return 10;
  if (check_for_win(board, -1))
    return -10;
  if (check_for_draw(board))
    return 0;

  if (is_maximizing) {
    int best_score = INT_MIN;
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (board[i][j] == 0) {
          board[i][j] = 1;
          int score = minimax(board, depth + 1, 0);
          board[i][j] = 0;
          best_score = max(score, best_score);
        }
      }
    }
    return best_score;
  }
  int best_score = INT_MAX;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (board[i][j] == 0) {
        board[i][j] = -1;
        int score = minimax(board, depth + 1, 1);
        board[i][j] = 0;
        best_score = min(score, best_score);
      }
    }
  }
  return best_score;
}

int *computer_move(int (*board)[3]) {
  int best_score = INT_MIN;
  static int best_move[2];
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (board[i][j] == 0) {
        board[i][j] = 1;
        int score = minimax(board, 0, 0);
        board[i][j] = 0;
        if (score > best_score) {
          best_score = score;
          best_move[0] = i;
          best_move[1] = j;
        }
      }
    }
  }
  return best_move;
}
// REFEREBCE :
// https://www.raylib.com/examples/core/loader.html?name=core_basic_screen_manager
typedef enum GameScreen { TITLE = 0, GAMEPLAY, ENDING } GameScreen;
int main() {
  int mat[ROWS][COLS];
  int player_turn = 1;
  int computer_turn = 0;


  GameScreen currentScreen = TITLE;
  SetTargetFPS(FPS);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AFTER 2 FREAKING YEARS!!");
  while (!WindowShouldClose()) {
    switch (currentScreen) {
    case TITLE: {
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = GAMEPLAY;
      }
    } break;
    case GAMEPLAY: {
      if (check_for_win(mat, 1) || check_for_win(mat, -1) ||
          check_for_draw(mat)) {
        // DrawText("Computer Wins", 200, 200, 10, BLACK);
        currentScreen = ENDING;
      }
      /*
      if (check_for_win(mat, -1)) {
        printf("Human wins!\n");
        // break;
      }

      if (check_for_draw(mat)) {
        printf("Draw!\n");
        // break;
      }
        */
    } break;
    case ENDING:{
      if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = TITLE;
      }
      }
    default:
      break;
    }
    //  DrawText(hello, SCREEN_HEIGHT/2, SCREEN_WIDTH/2 ,20, GREEN);
    ClearBackground(WHITE);
    BeginDrawing();
    switch (currentScreen) {
    case TITLE: {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      mat[i][j] = 0;
    }
  }
      DrawText("GAME OF LIFE IN C", 100, 30, 40, DARKGREEN);
      DrawText("Author : SAMIP REGMI", 100, 80, 30, RED);
      DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 10, 220, 20,
               DARKGREEN);
    } break;
    case GAMEPLAY: {

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player_turn) {
        Vector2 mPos = GetMousePosition();
        int col = (int)mPos.x / CELL_WIDTH;
        int row = (int)mPos.y / CELL_HEIGHT;
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {

          if (mat[row][col] == 0) {
            mat[row][col] = -1;
            player_turn = 0;
            computer_turn = 1;
          }
        }
      }
      if (computer_turn) {
        int *target = computer_move(mat);
        mat[target[0]][target[1]] = 1;
        player_turn = 1;
        computer_turn = 0;
      }

      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
          if (mat[i][j] == -1) {
            DrawRectangle(j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH,
                          CELL_HEIGHT, BLUE);
          } else if (mat[i][j] == 1) {
            DrawRectangle(j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH,
                          CELL_HEIGHT, RED);
          } else {
            DrawRectangle(j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH,
                          CELL_HEIGHT, WHITE);
          }
          DrawRectangleLines(j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH,
                             CELL_HEIGHT, GRAY);
        }
      }

    } break;
    case ENDING: {

      if (check_for_win(mat, 1)) {
        DrawText("RESULT: COMPUTER WON", 40, 140, 40, RED);
        // break;
      }
      if (check_for_win(mat, -1)) {
           DrawText("RESULT: HUMAN WON", 70, 140, 40, BLUE);

        //printf("Human wins!\n");
        // break;
      }

      if (check_for_draw(mat)) {
        //printf("Draw!\n");
      DrawText("RESULT: DRAW", 100, 140, 40, BLACK);
        // break;
      }
      DrawText("GAME OF LIFE IN C", 100, 30, 30, DARKGREEN);
      DrawText("Author : SAMIP REGMI", 100, 80, 30, BLACK);
      DrawText("PRESS ENTER or TAP to JUMP to TITLE SCREEN", 5, 120, 20,
               DARKGREEN);
    }break;
    default:
      break;
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
