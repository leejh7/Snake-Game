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
  std::vector<CharPos> wall;
  int iter;
  std::vector<CharPos> snake;
  CharPos growth_item;
  CharPos poison_item;
  CharPos gate_one;
  CharPos gate_two;
  void InitWindow();
  void InitGameBoard();
  void InitChar();
  void DrawMap(int n);
  void DrawSnake();
  void MoveSnake();
  void RenewMap();
  void KeyEvent(int key);
  void MakeItem();
  void MakeGate();
  void Check(bool &flag);
  void TeleportRule(int n);
  bool MoveRule(int key);
  bool Crash();
public:
  SnakeGame();
  ~SnakeGame();
  void StartGame();
};
