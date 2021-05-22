#pragma once

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <vector>

typedef struct CharPos
{
  int xpos;
  int ypos;
  CharPos(int x, int y);
  CharPos();
}CharPos;

class SnakeGame
{
private:
  WINDOW *gameBoard;
  //WINDOW *scoreBoard;
  //WINDOW *missionBoard;
  int maxwidth, maxheight, c, gWidth, gHeight;
  char direction;
  char mapData[21][60];
  std::vector<CharPos> snake;
  CharPos growth_item;
  CharPos poison_item;
  void InitWindow();
  void InitChar();
  void DrawMap(int n);
  void DrawSnake();
  void MoveSnake();
  void RenewMap();
  void KeyEvent(int key);
  void MakeItem();
  void Check(bool &flag);
  bool MoveRule(int key);
  bool Crash();
public:
  SnakeGame();
  ~SnakeGame();
  void StartGame();
};
