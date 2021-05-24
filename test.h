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
  WINDOW *scoreBoard;
  WINDOW *missionBoard;
  int maxwidth, maxheight, c;
  int gWidth, gHeight;
  int sWidth, sHeigth;
  int mWidth, mHeight;
  int cur_length, cnt_grow, cnt_poison, cnt_use_gate;
  int mis_length,mis_cnt_grow,mis_cnt_poison,mis_cnt_use_gate;
  char check_length,check_grow,check_poison,check_use_gate;
  char direction;
  char mapData[21][60];
  std::vector<CharPos> wall;
  int iter;
  int iter_gate;
  std::vector<CharPos> snake;
  CharPos growth_item;
  CharPos poison_item;
  CharPos gate_one;
  CharPos gate_two;
  void InitWindow();
  void InitGameBoard();
  void InitScoreBoard();
  void InitMissionBoard();
  void InitChar();
  void DrawMap(int n);
  void DrawGameBoard();
  void DrawMissionBoard();
  void DrawSnake();
  void MoveSnake();
  void RenewMap();
  void RenewBoard();
  void KeyEvent(int key);
  void MakeItem();
  void MakeGate();
  void Check(bool &flag);
  void TeleportRule(int n);
  bool MoveRule(int key);
  bool Crash();
  bool NextStage();
public:
  SnakeGame();
  ~SnakeGame();
  void StartGame();
};
