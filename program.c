#include <ncurses.h>

const char *create_input_box(int y, int x);

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
  WINDOW *tasks_win = newwin(row/2, col, 0, 0);
  box(tasks_win, 0, 0);
  mvwprintw(tasks_win, 0, 1, "PENDING TASKS");  
  wmove(tasks_win, 1, 1);
  wrefresh(tasks_win);
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
      printw(create_input_box(y, x)); 
      wmove(tasks_win, 1, 1);
      wprintw(tasks_win, "Test");
      box(tasks_win, 0, 0);
      mvwprintw(tasks_win, 0, 1, "PENDING TASKS");  
      wrefresh(tasks_win);
      move(row/2, col/2);
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

const char *create_input_box(int y, int x) {
  static char task[30];
  WINDOW *input_box = newwin(3, 20, y-3, x-10);
  box(input_box, 0, 0);
  mvwprintw(input_box, 0, 1, "Name of task");  
  refresh();
  wmove(input_box, 1, 1);
  wrefresh(input_box);  
  echo();
  wgetstr(input_box, task);
  wclear(input_box);
  wrefresh(input_box);
  delwin(input_box);
  noecho();
  return task;
}
