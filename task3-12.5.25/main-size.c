#include <stdio.h>
#include <stdbool.h>

#pragma GCC optimize("Ofast")

#define board_size          7
#define R                   1
#define Y                  -1
#define empty               0
#define numOfRounds         3
#define get_row()           game_state[game_round].row
#define get_column()        game_state[game_round].column
#define get_gameover()      game_state[game_round].game_over
#define set_gameover(x)     game_state[game_round].game_over = x
#define get_turn()          game_state[game_round].turn
#define set_turn(x)         game_state[game_round].turn = x
#define get_winner()        game_state[game_round].winner
#define set_winner(x)       game_state[game_round].winner = x

void playMove();
void initialize_game(int round);

void set_row(int value);
void set_column(int value);
int get_gameboard(int row, int col);
void set_gameboard(int row, int col, int value);

#pragma pack(2)
struct gamestate {
  int row;    
  int column;  
  bool game_over;  
  bool turn;     
  int winner;     
  int game_board[board_size][board_size]; 
};

struct gamestate game_state[numOfRounds];
int countR = 0;
int countY = 0;
int col;
int game_round = 0;

int main() {

    initialize_game(game_round);
    printf("Round %d\n", game_round+1);

    while (!get_gameover()) {
        printf("Enter column to place piece:\n");
        scanf("%d", &col);

        if (col < 0 || col >= board_size) {
            printf("Invalid column!\n");
            continue;
        } else if (get_gameboard(0, col) != empty) {
            printf("Invalid column!\n");
            continue;
        } else {
            printf("column chosen: %d\n", col);
        }
    
        for (int row = 0; row <= 6; row++) {
            if (get_gameboard(row, col) == empty) {
                set_column(col);
                set_row(row); 
            }
        }
    
        playMove();

        for (int i = 0; i < board_size; i++) { 
            for (int j = 0; j < board_size; j++) { 
                if (get_gameboard(i, j) == empty) {
                    printf("_");
                } if (get_gameboard(i, j) == 1) {
                    printf("R");
                } if (get_gameboard(i, j) == -1) {
                    printf("Y");
                }
            } printf("\n");
        }

    if (get_winner() == 1) {
        printf("R won this round!\n");
        countR ++;
        if (game_round < numOfRounds && countR < 2) {
            game_round++;
            set_turn(1);
            printf("Round %d\n", game_round+1);
            initialize_game(game_round);
        } else {
          printf("R won the match!\n");
          break;
        }
    }
    else if (get_winner() == -1) {
        printf("Y won this round!\n");
        countY++;
        if (game_round < numOfRounds && countY < 2) {
            game_round++;
            set_turn(1);
            printf("Round %d\n", game_round+1);
            initialize_game(game_round);
        } else {
          printf("Y won the match!\n");
          break;
        }
    }
  }
}
  
void playMove() {
  if (get_turn() == true) {
    set_gameboard(get_row(), get_column(), 1);
  } else {
    set_gameboard(get_row(), get_column(), -1);
  }
  set_turn(!get_turn());
  for (int i = 0; i < board_size; i++) { 
    for (int j = 0; j < board_size - 3; j++) { 
      if ((get_gameboard(i, j) == R || (get_gameboard(i, j) == Y)) && (get_gameboard(i, j) == get_gameboard(i, j+1)) && (get_gameboard(i, j) == get_gameboard(i, j+2)) && (get_gameboard(i, j) == get_gameboard(i, j+3))) {
        if (get_gameboard(i, j) == R) {
          set_winner(R);
        } else if(get_gameboard(i, j) == Y) {
          set_winner(Y);
        }
      }
    }
  }
  for (int j = 0; j < board_size; j++) { 
    for (int i = 0; i < board_size - 3; i++) { 
      if ((get_gameboard(i, j) == R || (get_gameboard(i, j) == Y)) && (get_gameboard(i, j) == get_gameboard(i+1, j)) && (get_gameboard(i, j) == get_gameboard(i+2, j)) && (get_gameboard(i, j) == get_gameboard(i+3, j))) {
        if (get_gameboard(i, j) == R) {
          set_winner(R);
        } else if(get_gameboard(i, j) == Y) {
          set_winner(Y);
        }
      }
    }
  }
  for (int i = board_size-1; i > 2; i--) { 
    for (int j = 0; j < board_size - 3; j++) { 
      if ((get_gameboard(i, j) == R || (get_gameboard(i, j) == Y)) && (get_gameboard(i, j) == get_gameboard(i-1, j+1)) && (get_gameboard(i, j) == get_gameboard(i-2, j+2)) && (get_gameboard(i, j) == get_gameboard(i-3, j+3))) {
        if (get_gameboard(i, j) == R) {
          set_winner(R);
        } else if(get_gameboard(i, j) == Y) {
          set_winner(Y);
        }
      }
    }
  }
  for (int i = 0; i < board_size - 3; i++) { 
    for (int j = 0; j < board_size - 3; j++) { 
      if ((get_gameboard(i, j) == R || (get_gameboard(i, j) == Y)) && (get_gameboard(i, j) == get_gameboard(i+1, j+1)) && (get_gameboard(i, j) == get_gameboard(i+2, j+2)) && (get_gameboard(i, j) == get_gameboard(i+3, j+3))) {
        if (get_gameboard(i, j) == R) {
          set_winner(R);
        } else if(get_gameboard(i, j) == Y) {
          set_winner(Y);
        }
      }
    }
  }
}

void initialize_game(int round) {
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      game_state[round].game_board[i][j] = empty; 
    }
  }
  game_state[round].column = 0; 
  game_state[round].game_over = false; 
  game_state[round].turn = 1; 
  game_state[round].winner = 0; 
}

void set_row(int value) {
   if (value < board_size && value >= 0) {
      game_state[game_round].row = value; 
   } else {
      return; 
   }
}

void set_column(int value) {
   if (value < board_size && value >= 0) { 
      game_state[game_round].column = value; 
   } else {
      return; 
   }
}

int get_gameboard(int row, int col) {
   if (row >= 0 && col >= 0 && row < board_size && col < board_size) {
      return game_state[game_round].game_board[row][col]; 
   } else {
      return game_state[game_round].game_board[row][col] = empty;
   }
}

void set_gameboard(int row, int col, int value) {
   if (row >= 0 && col >= 0 && row < board_size && col < board_size) { 
      game_state[game_round].game_board[row][col] = value;
   } else {
      return;
   }
}
