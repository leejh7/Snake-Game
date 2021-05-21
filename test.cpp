#include "test.h"

SnakePos::SnakePos(int y, int x)
{
  xpos = x;
  ypos = y;
}

SnakePos::SnakePos()
{
  xpos = 0;
  ypos = 0;
}

SnakeGame::SnakeGame()
{
  gWidth = 60;
  gHeight = 21;
  srand(time(NULL));
  direction = 'l';
  InitSnake();
  InitWindow();
  gameBoard = newwin(gHeight,gWidth,0,0);
}

SnakeGame::~SnakeGame()
{
  nodelay(stdscr,FALSE);
  getch();
  endwin();
}

void SnakeGame::InitWindow()
{
  initscr();
  cbreak();
  start_color();
  keypad(stdscr,TRUE);
  noecho();
  curs_set(1);
  nodelay(stdscr,TRUE);
  getmaxyx(stdscr,maxheight, maxwidth);

  char mesg[] = "Press <Enter> to play game";
  mvprintw(maxheight/2,(maxwidth-strlen(mesg))/2,"%s",mesg);
  mvchgat(maxheight/2,(maxwidth-strlen(mesg))/2,-1,A_BLINK,0,NULL);
  while(c=getch())
  {
    if(c=='\n')
    {
      clear();
      break;
    }
  }
  getch();
  move(0,0);
  refresh();
}

void SnakeGame::InitSnake()
{
  if(snake.size() != 0)
    snake.clear();
  for(int i=0; i<3; i++)
    snake.push_back(SnakePos(gHeight/2,gWidth/2-2+i));
}

void SnakeGame::DrawMap(int n)
{
  int row=0, col=0;
  std::string mapfile = "/home/leejh7/C2021/test/1/map" + std::to_string(n) + ".txt";
  std::ifstream map(mapfile,std::ios::in);

  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  init_pair(4,COLOR_CYAN,COLOR_MAGENTA);
  while((c=map.get())!=EOF) {
    if(c=='1'){
      mapData[row][col++] = c;
      wattron(gameBoard,COLOR_PAIR(1));
      wprintw(gameBoard,"%c",'X');
      wattroff(gameBoard,COLOR_PAIR(1));
    }
    else if(c=='0'){
      mapData[row][col++] = c;
      wattron(gameBoard,COLOR_PAIR(1));
      wprintw(gameBoard,"%c",'@');
      wattroff(gameBoard,COLOR_PAIR(1));
    }
    else if(c=='2') {
      mapData[row][col++] = c;
      wprintw(gameBoard,"%c",' ');
    }
    else if(c=='3') {
      mapData[row][col++] = c;
      wattron(gameBoard,COLOR_PAIR(4));
      wprintw(gameBoard,"%c",'P');
      wattroff(gameBoard,COLOR_PAIR(4));
    }
    else {
      row++; col=0;
      wprintw(gameBoard,"%c",c);
    }
  }

  map.close();
  wrefresh(gameBoard);
}

void SnakeGame::StartGame()
{
  DrawMap(1);
  DrawSnake();
  for(int i=0; i<100; i++){
    MoveSnake();
    DrawSnake();
    if(Crash()) break;
  }
}

void SnakeGame::DrawSnake()
{
  init_pair(2,COLOR_RED,COLOR_YELLOW);
  init_pair(3,COLOR_RED,COLOR_MAGENTA);
  for(int i=snake.size()-1; i>=0; i--)
  {
    if(i==0){
      wattron(gameBoard,COLOR_PAIR(2));
      wmove(gameBoard,snake[i].ypos,snake[i].xpos);
      waddch(gameBoard,'X');
    }
    else {
      wattron(gameBoard,COLOR_PAIR(3));
      wmove(gameBoard,snake[i].ypos,snake[i].xpos);
      waddch(gameBoard,'O');
    }
  }
  wattroff(gameBoard,COLOR_PAIR(2));
  wmove(gameBoard,snake[0].ypos,snake[0].xpos);
  wrefresh(gameBoard);
}

void SnakeGame::MoveSnake()
{
  nodelay(gameBoard,TRUE);
  keypad(gameBoard,TRUE);
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = end - start;
  bool flag = false;
  int key = 0;
  while(diff.count()<0.5)
  {
    if(key = wgetch(gameBoard))
    {
        flag = true;
        if(!MoveRule(key)) break;
        KeyEvent(key);
        break;
    }
    end = std::chrono::system_clock::now();
    diff = end - start;
  }

  while(flag && diff.count()<0.5)
  {
    end = std::chrono::system_clock::now();
    diff = end - start;
  }

  switch(direction)
  {
    case 'u':
      snake.insert(snake.begin(),SnakePos(snake[0].ypos-1,snake[0].xpos));
      break;
    case 'd':
      snake.insert(snake.begin(),SnakePos(snake[0].ypos+1,snake[0].xpos));
      break;
    case 'l':
      snake.insert(snake.begin(),SnakePos(snake[0].ypos,snake[0].xpos-1));
      break;
    case 'r':
      snake.insert(snake.begin(),SnakePos(snake[0].ypos,snake[0].xpos+1));
      break;
    default:
      break;
  }
  RenewMap();
  snake.pop_back();
}

void SnakeGame::KeyEvent(int key)
{
  switch(key)
  {
    case KEY_UP:
      direction='u';
      break;
    case KEY_DOWN:
      direction='d';
      break;
    case KEY_LEFT:
      direction='l';
      break;
    case KEY_RIGHT:
      direction='r';
      break;
    default:
      break;
  }
}

void SnakeGame::RenewMap()
{
  wmove(gameBoard,snake.back().ypos,snake.back().xpos);
  int x,y;
  getyx(gameBoard,y,x);
  if(mapData[y][x]=='2') waddch(gameBoard,' ');
  else if(mapData[y][x]=='3') waddch(gameBoard,'P');
}

bool SnakeGame::MoveRule(int key)
{
  if(direction=='l'&&key==KEY_RIGHT) return false;
  else if(direction=='r'&&key==KEY_LEFT) return false;
  else if(direction=='u'&&key==KEY_DOWN) return false;
  else if(direction=='d'&&key==KEY_UP) return false;
  else return true;
}

bool SnakeGame::Crash()
{
  int x,y;
  getyx(gameBoard,y,x);
  if(mapData[y][x]=='0'||mapData[y][x]=='1') return true;
  else return false;
}
