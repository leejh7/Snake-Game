#include "test.h"

CharPos::CharPos(int y, int x)
{
  xpos = x;
  ypos = y;
}

CharPos::CharPos()
{
  xpos = 0;
  ypos = 0;
}

SnakeGame::SnakeGame()
{
  InitWindow();
  InitGameBoard();
  InitChar();
}

SnakeGame::~SnakeGame()
{
  nodelay(stdscr,FALSE);
  getch();
  endwin();
}

void SnakeGame::InitWindow()
{
  srand(time(NULL));
  initscr();
  cbreak();
  start_color();
  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);
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

void SnakeGame::InitGameBoard()
{
  gWidth = 60;
  gHeight = 21;
  gameBoard = newwin(gHeight,gWidth,0,0);
  nodelay(gameBoard,TRUE);
  keypad(gameBoard,TRUE);
}

void SnakeGame::InitChar()
{
  //Init snake
  direction = 'l';
  if(snake.size() != 0)
    snake.clear();
  for(int i=0; i<3; i++)
    snake.push_back(CharPos(gHeight/2,gWidth/2-2+i));

  //Init Growth_Item
  growth_item.ypos=10;
  growth_item.xpos=10;
  poison_item.ypos=5;
  poison_item.xpos=50;

  //Init Gate
  gate_one.ypos=5;
  gate_one.xpos=0;
  gate_two.ypos=15;
  gate_two.xpos=58;
}

void SnakeGame::DrawMap(int n)
{
  int row=0, col=0;
  std::string mapfile = "/home/leejh7/C2021/test/1/map" + std::to_string(n) + ".txt";
  std::ifstream map(mapfile,std::ios::in);

  init_pair(1,COLOR_CYAN,COLOR_BLACK);
  init_pair(4,COLOR_CYAN,COLOR_MAGENTA);
  init_pair(5,COLOR_WHITE,COLOR_GREEN);
  init_pair(6,COLOR_BLACK,COLOR_RED);
  while((c=map.get())!=EOF) {
    if(c=='1'){
      mapData[row][col++] = c;
      wattron(gameBoard,COLOR_PAIR(1));
      wprintw(gameBoard,"%c",'X');
      wattroff(gameBoard,COLOR_PAIR(1));
    }
    else if(c=='0'){
      mapData[row][col++] = c;
      wall.push_back(CharPos(row,col));
      wattron(gameBoard,COLOR_PAIR(1));
      wprintw(gameBoard,"%c",'@');
      wattroff(gameBoard,COLOR_PAIR(1));
    }
    else if(c=='2') {
      mapData[row][col++] = c;
      wprintw(gameBoard,"%c",' ');
    }
    else {
      row++; col=0;
      wprintw(gameBoard,"%c",c);
    }
  }

  mapData[10][10]='3';
  mapData[5][50]='4';
  wattron(gameBoard,COLOR_PAIR(5));
  mvwprintw(gameBoard,10,10,"G");
  wattroff(gameBoard,COLOR_PAIR(5));
  wattron(gameBoard,COLOR_PAIR(6));
  mvwprintw(gameBoard,5,50,"P");
  wattroff(gameBoard,COLOR_PAIR(6));

  mapData[5][0]='5';
  mapData[15][58]='6';
  wattron(gameBoard,COLOR_PAIR(4));
  mvwprintw(gameBoard,5,0,"T");
  mvwprintw(gameBoard,15,58,"T");
  wattroff(gameBoard,COLOR_PAIR(4));

  map.close();
  wrefresh(gameBoard);
}

void SnakeGame::StartGame()
{
  iter=0;
  DrawMap(1);
  DrawSnake();
  while(true)
  {
    iter++;
    if(iter>=50) MakeItem();
    //if(iter%5==0) MakeGate();
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
      wattroff(gameBoard,COLOR_PAIR(2));
    }
    else {
      wattron(gameBoard,COLOR_PAIR(3));
      wmove(gameBoard,snake[i].ypos,snake[i].xpos);
      waddch(gameBoard,'O');
      wattroff(gameBoard,COLOR_PAIR(3));
    }
  }

  wmove(gameBoard,snake[0].ypos,snake[0].xpos);
  wrefresh(gameBoard);
}

void SnakeGame::MoveSnake()
{
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = end - start;
  bool flag = false;
  int key = 0;
  while(diff.count()<0.3)
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

  while(flag && diff.count()<0.3)
  {
    end = std::chrono::system_clock::now();
    diff = end - start;
  }

  switch(direction)
  {
    case 'u':
      snake.insert(snake.begin(),CharPos(snake[0].ypos-1,snake[0].xpos));
      break;
    case 'd':
      snake.insert(snake.begin(),CharPos(snake[0].ypos+1,snake[0].xpos));
      break;
    case 'l':
      snake.insert(snake.begin(),CharPos(snake[0].ypos,snake[0].xpos-1));
      break;
    case 'r':
      snake.insert(snake.begin(),CharPos(snake[0].ypos,snake[0].xpos+1));
      break;
    default:
      break;
  }
  flag=true;
  Check(flag);
  RenewMap();
  if(flag) snake.pop_back();
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

void SnakeGame::Check(bool &flag)
{
  wmove(gameBoard,snake[0].ypos,snake[0].xpos);
  int x,y;
  getyx(gameBoard,y,x);
  if(mapData[y][x]=='3')
  {
    flag=false;
    MakeItem();
  }
  else if(mapData[y][x]=='4')
  {
    RenewMap();
    snake.pop_back();
    MakeItem();
  }
  else if(mapData[y][x]=='5')
  {
    snake.erase(snake.begin());
    TeleportRule(1);
  }
  else if(mapData[y][x]=='6')
  {
    snake.erase(snake.begin());
    TeleportRule(2);
  }
  else return;
}

void SnakeGame::TeleportRule(int n)
{
  int basex[4]={0,1,0,-1};
  int basey[4]={-1,0,1,0};
  int dx[4], dy[4];
  switch(direction)
  {
    case 'u':
      for(int i=0;i<4;i++)
      {
        int idx=0+i;
        if(idx>=4) idx-=4;
        dx[i]=basex[idx];
        dy[i]=basey[idx];
      }
      break;
    case 'd':
      for(int i=0;i<4;i++)
      {
        int idx=2+i;
        if(idx>=4) idx-=4;
        dx[i]=basex[idx];
        dy[i]=basey[idx];
      }
      break;
    case 'l':
      for(int i=0;i<4;i++)
      {
        int idx=3+i;
        if(idx>=4) idx-=4;
        dx[i]=basex[idx];
        dy[i]=basey[idx];
      }
      break;
    case 'r':
      for(int i=0;i<4;i++)
      {
        int idx=1+i;
        if(idx>=4) idx-=4;
        dx[i]=basex[idx];
        dy[i]=basey[idx];
      }
      break;
    default:
      break;
  }

  if(n==1)
  {
    int row,col;
    for(int i=0;i<4;i++)
    {
      row=gate_two.ypos+dy[i];
      col=gate_two.xpos+dx[i];
      if(mapData[row][col]=='2')
      {
        snake.insert(snake.begin(),CharPos(row,col));
        break;
      }
    }
  }
  else if(n==2)
  {
    int row,col;
    for(int i=0;i<4;i++)
    {
      row=gate_one.ypos+dy[i];
      col=gate_one.xpos+dx[i];
      if(mapData[row][col]=='2')
      {
        snake.insert(snake.begin(),CharPos(row,col));
        break;
      }
    }
  }

}

void SnakeGame::RenewMap()
{
  wmove(gameBoard,snake.back().ypos,snake.back().xpos);
  int x,y;
  getyx(gameBoard,y,x);
  if(mapData[y][x]=='5'||mapData[y][x]=='6')
  {
    // wattron(gameBoard,COLOR_PAIR(4));
    // waddch(gameBoard,'T');
    // wattroff(gameBoard,COLOR_PAIR(4));
  }
  else if(mapData[y][x]!='0',mapData[y][x]!='1') waddch(gameBoard,' ');
}


void SnakeGame::MakeItem()
{
  iter=0;
  //Growth item
  while(true)
  {
    int row,col;
    bool flag=true;
    row=rand()%20+1;
    col=rand()%59+1;
    for(int i=0;i<snake.size();i++)
    {
      if(row==snake[i].ypos&&col==snake[i].xpos)
      {
        flag=false;
        break;
      }
    }
    if(mapData[row][col]=='2'&&flag)
    {
      mapData[growth_item.ypos][growth_item.xpos]='2';
      mvwprintw(gameBoard,growth_item.ypos,growth_item.xpos," ");
      mapData[row][col]='3';
      growth_item.ypos=row;
      growth_item.xpos=col;
      wattron(gameBoard,COLOR_PAIR(5));
      mvwprintw(gameBoard,growth_item.ypos,growth_item.xpos,"G");
      wattroff(gameBoard,COLOR_PAIR(5));
      break;
    }
  }
  //Poison item
  while(true)
  {
    int row,col;
    bool flag=true;
    row=rand()%20+1;
    col=rand()%59+1;
    for(int i=0;i<snake.size();i++)
    {
      if(row==snake[i].ypos&&col==snake[i].xpos)
      {
        flag=false;
        break;
      }
    }
    if(mapData[row][col]=='2'&&flag)
    {
      mapData[poison_item.ypos][poison_item.xpos]='2';
      mvwprintw(gameBoard,poison_item.ypos,poison_item.xpos," ");
      mapData[row][col]='4';
      poison_item.ypos=row;
      poison_item.xpos=col;
      wattron(gameBoard,COLOR_PAIR(6));
      mvwprintw(gameBoard,poison_item.ypos,poison_item.xpos,"P");
      wattroff(gameBoard,COLOR_PAIR(6));
      break;
    }
  }
}

void SnakeGame::MakeGate()
{
  //Gate 1
  while(true)
  {
    int row,col,idx;
    idx=rand()%wall.size();
    row=wall[idx].ypos;
    col=wall[idx].xpos;
    if(mapData[row][col]=='0')
    {
      mapData[gate_one.ypos][gate_one.xpos]='0';
      wattron(gameBoard,COLOR_PAIR(1));
      mvwprintw(gameBoard,gate_one.ypos,gate_one.xpos,"@");
      wattroff(gameBoard,COLOR_PAIR(1));
      mapData[row][col]='5';
      gate_one.ypos=row;
      gate_one.xpos=col;
      wattron(gameBoard,COLOR_PAIR(4));
      mvwprintw(gameBoard,gate_one.ypos,gate_one.xpos,"T");
      wattroff(gameBoard,COLOR_PAIR(4));
      break;
    }
  }
  //Gate 2
  while(true)
  {
    int row,col,idx;
    idx=rand()%wall.size();
    row=wall[idx].ypos;
    col=wall[idx].xpos;
    if(mapData[row][col]=='0')
    {
      mapData[gate_two.ypos][gate_two.xpos]='0';
      wattron(gameBoard,COLOR_PAIR(1));
      mvwprintw(gameBoard,gate_two.ypos,gate_two.xpos,"@");
      wattroff(gameBoard,COLOR_PAIR(1));
      mapData[row][col]='6';
      gate_two.ypos=row;
      gate_two.xpos=col;
      wattron(gameBoard,COLOR_PAIR(4));
      mvwprintw(gameBoard,gate_two.ypos,gate_two.xpos,"T");
      wattroff(gameBoard,COLOR_PAIR(4));
      break;
    }
  }
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
  // condition of crash body
  for(int i=1; i<snake.size(); i++)
  {
    if(y==snake[i].ypos&&x==snake[i].xpos) return true;
  }
  // condition of crash wall
  if(mapData[y][x]=='0'||mapData[y][x]=='1') return true;

  return false;
}
