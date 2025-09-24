#include <ncurses.h>

void create_input_box(int y, int x);

//----------------------------------------------------
// Main function
//----------------------------------------------------

int main(){
  int row,col;
  int ch=0;
  int x,y;

  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  getmaxyx(stdscr, row, col);
  mvprintw(row-1, 0, "Press q to quit");
  refresh();
  move(row/2, col/2);
  while (ch!=113) {
    ch = getch();
    switch (ch) {
      case 106: // j 
      getyx(stdscr, y, x);
      move(y+1, x);
      break;

      case 107: // k 
      getyx(stdscr, y, x);
      move(y-1, x);
      break;

      case 113: // q 
      break;

      case 97: // a 
      getyx(stdscr, y, x);
      create_input_box(y, x);
      break;
    }
  }
  printw("You really pressed q...");
  mvprintw(row-1,0,"Press any key to exit");
  refresh();
  getch();
  endwin();
  return 0;
}

//----------------------------------------------------
// Input box function
//----------------------------------------------------

void create_input_box(int y, int x) {
  WINDOW *local_win = newwin(3, 20, y-3, x-10);
  box(local_win, 0, 0);
  mvwprintw(local_win, 1, 1, "Hello");  
  refresh();
  wrefresh(local_win);  
  delwin(local_win);
}
