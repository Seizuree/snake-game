#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int m = 20;
int n = 40;

int i = 0, j = 0;

struct snakeGame {
  int kotak[25][45];
  int snakeX,snakeY;
  int temp;
  int snakeHead,snakeTail;
};

int game = 0;

int makanan = 0, posMakananX = 0, posMakananY = 0;

int move = 0;
int snakeDirection = 0;

int score = 0;
int highScore = 0;

int speed = 99;

int mainChoices, lives = 3;

FILE *fptr;

struct snakeGame sg;

void printMenu();
void snakeInitialize()
{
    fptr = fopen("score.txt","r");
    fscanf(fptr,"%d",&highScore);
    fclose(fptr);

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            sg.kotak[i][j] = 0;
        }
    }

    sg.snakeX = m/2;
    sg.snakeY = n/2;
    sg.snakeHead = 5;
    sg.snakeTail = 1;
    sg.temp = sg.snakeY;
    snakeDirection = 'd';

    for (i = 0; i < sg.snakeHead; i++)
    {
        sg.temp++;
        sg.kotak[sg.snakeX][sg.temp-sg.snakeHead] = i+1;
    }
}

void printSnake()
{
    for (i = 0; i <= n+1; i++)
    {
        if (i == 0) {
            printf("%c",201);
        }
        else if (i == n+1) {
            printf("%c",187);
        }
        else {
            printf("%c",205);
        }
    }
    puts("");
    for (i = 0; i < m; i++)
    {
        printf("%c",186);
        for (j = 0; j < n; j++)
        {
            if (sg.kotak[i][j] == 0) {
                printf(" ");
            }
            if (sg.kotak[i][j] > 0 && sg.kotak[i][j] != sg.snakeHead) {
                printf("%c",176);
            }
            if (sg.kotak[i][j] == sg.snakeHead) {
                printf("%c",178);
            }
            if (sg.kotak[i][j] == -1) {
                printf("%c",15);
            }
            if (j == n - 1) {
                printf("%c\n",186);
            }
        }
    }
    for (i = 0; i <= n+1; i++)
    {
        if (i == 0) {
            printf("%c",200);
        }
        else if (i == n+1) {
            printf("%c",188);
        }
        else {
            printf("%c",205);
        }
    }
    printf("\n\nRemaining lives: %d",lives);
    printf("\nCurrent Score: %d",score);
    printf("\nHigh Score: %d",highScore);
}

void resetPos()
{
  HANDLE hOut;
  COORD Pos;
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  Pos.X = 0;
  Pos.Y = 0;
  SetConsoleCursorPosition(hOut,Pos);
}

int getKeys()
{
  if (_kbhit()) {
    return _getch();
  }
  else {
    return -1;
  }
}

void randomNum()
{
  srand(time(0));
  posMakananX = 1 + rand() % 18;
  posMakananY = 1 + rand() % 38;

  if(makanan == 0 && sg.kotak[posMakananX][posMakananY] == 0) {
     sg.kotak[posMakananX][posMakananY] = -1;
     makanan = 1;
     if (speed > 10 && score != 0) {
       speed -= 2;
     }
  }
}

void tailUpdate()
{
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      if (sg.kotak[i][j] == sg.snakeTail) {
        sg.kotak[i][j] = 0;
      }
    }
  }
  sg.snakeTail++;
}

void gameOver() {
  printf("\a");
  Sleep(1500);
  system("cls");

  if (score > highScore) {
    printf("New high score is %d.\n",score);
    system("pause");
    fptr = fopen("score.txt","w");
    fprintf(fptr, "%d\n",score);
    fclose(fptr);
  }

  system("cls");
  printf("Game over.\n");
  printf("Score: %d\n",score);

  getchar();

  printf("Thank you for running this game.\n");
  printf("Have a nice day!\n");
  getchar();
  exit(0);

  system("cls");
}

void movement()
{
  move = getKeys();
  move = tolower(move);

  if (((move == 'd' || move == 'a' || move == 'w' || move == 's'))
    && (abs(snakeDirection - move) > 5)) {
    snakeDirection = move;
  }

  if (snakeDirection == 'd') {
    sg.snakeY++;
    if (sg.kotak[sg.snakeX][sg.snakeY] != 0 && sg.kotak[sg.snakeX][sg.snakeY] != -1) {
      lives--;
    }
    if (lives == 0) {
      gameOver();
    }
    if (sg.snakeY == n-1) {
      sg.snakeY = 0;
    }
    if (sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      makanan = 0;
      score += 2;
      sg.snakeTail--;
    }
    if (score % 10 == 0 && score != 0 && sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      lives++;
    }
    sg.snakeHead++;
    sg.kotak[sg.snakeX][sg.snakeY] = sg.snakeHead;
  }

  if (snakeDirection == 'a') {
    sg.snakeY--;
    if (sg.kotak[sg.snakeX][sg.snakeY] != 0 && sg.kotak[sg.snakeX][sg.snakeY] != -1) {
      lives--;
    }
    if (lives == 0) {
      gameOver();
    }
    if (sg.snakeY == 0) {
      sg.snakeY = n-1;
    }
    if (sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      makanan = 0;
      score += 2;
      sg.snakeTail--;
    }
    if (score % 10 == 0 && score != 0 && sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      lives++;
    }
    sg.snakeHead++;
    sg.kotak[sg.snakeX][sg.snakeY] = sg.snakeHead;
  }

  if (snakeDirection == 'w') {
    sg.snakeX--;
    if (sg.kotak[sg.snakeX][sg.snakeY] != 0 && sg.kotak[sg.snakeX][sg.snakeY] != -1) {
      lives--;
    }
    if (lives == 0) {
      gameOver();
    }
    if (sg.snakeX == -1) {
      sg.snakeX = m-1;
    }
    if (sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      makanan = 0;
      score += 2;
      sg.snakeTail--;
    }
    if (score % 10 == 0 && score != 0 && sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      lives++;
    }
    sg.snakeHead++;
    sg.kotak[sg.snakeX][sg.snakeY] = sg.snakeHead;
  }

  if (snakeDirection == 's') {
    sg.snakeX++;
    if (sg.kotak[sg.snakeX][sg.snakeY] != 0 && sg.kotak[sg.snakeX][sg.snakeY] != -1) {
      lives--;
    }
    if (lives == 0) {
      gameOver();
    }
    if (sg.snakeX == m) {
      sg.snakeX = 0;
    }
    if (sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      makanan = 0;
      score += 2;
      sg.snakeTail--;
    }
    if (score % 10 == 0 && score != 0 && sg.kotak[sg.snakeX][sg.snakeY] == -1) {
      lives++;
    }
    sg.snakeHead++;
    sg.kotak[sg.snakeX][sg.snakeY] = sg.snakeHead;
  }
}

void printMenu() {
  do {
    puts("=======================================================");
    puts("                    Snake Game v1.0");
    puts("   A snake game made by Brian. Use WASD for controls.");
    puts("     Copyright (C) 2022 Brian. All rights reserved.");
    puts("=======================================================");
    puts("1. Start Game");
    puts("2. View Highscore on this PC");
    puts("3. Exit Game");
    printf(">> ");
    scanf("%d",&mainChoices); getchar();
  } while(mainChoices < 1 || mainChoices > 3);
  system("cls");
  if (mainChoices == 1) {
    while (game == 0)
    {
      printSnake();
      resetPos();
      randomNum();
      movement();
      tailUpdate();
      Sleep(speed);
    }
  }

  else if (mainChoices == 2) {
    fptr = fopen("score.txt","r");
    fscanf(fptr,"%d",&highScore);
    fclose(fptr);
    printf("High score is %d\n\n",highScore);
  }

  else if (mainChoices == 3) {
    printf("Thank you for running this game.\n");
    printf("Have a nice day!\n");
    getchar();
    exit(0);
  }
}

int main()
{
  snakeInitialize();
  printMenu();
  return 0;
}
