#pragma once

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <vector>

typedef struct SnakePos
{
  int xpos;
  int ypos;
  SnakePos(int x, int y);
  SnakePos();
}SnakePos;

class SnakeGame
{
private:
  WINDOW *gameBoard;
  //WINDOW *scoreBoard;
  //WINDOW *missionBoard;
  int maxwidth, maxheight, c, gWidth, gHeight;
  char direction;
  char mapData[21][60];
  std::vector<SnakePos> snake;
  void InitWindow();
  void InitSnake();
  void DrawMap(int n);
  void DrawSnake();
  void MoveSnake();
  bool MoveRule(int key);
  bool Crash(int key);
public:
  SnakeGame();
  ~SnakeGame();
  void StartGame();
};
