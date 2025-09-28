#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define rgb(x) ((int)((float)x*3.90625))

const char *create_input_box(int y, int x);
void complete_task(FILE *file, FILE *done, int t);
void create_help_box();
void clear_done();

//----------------------------------------------------
// Main function
//----------------------------------------------------

int main(){
  int row,col;
  int ch=0;
  int x,y;
  int tasks_win_y=1;
  int tasks_win_x=1;
  int print_y;
  char mode[30]="pending";
  char tasks_string[1024];

  initscr();
  //COLORS
  start_color();
  init_color(COLOR_BLACK, rgb(25), rgb(23), rgb(36));
  init_color(COLOR_WHITE, rgb(224), rgb(222), rgb(244));
  init_color(COLOR_MAGENTA, rgb(235), rgb(188), rgb(186));
  init_color(COLOR_YELLOW, rgb(246), rgb(193), rgb(119));
  short color_muted = COLOR_WHITE + 1;
  short color_surface = COLOR_WHITE + 2;
  short color_overlay = COLOR_WHITE + 3;
  short color_highlight = COLOR_WHITE + 4;
  init_color(color_muted, rgb(110), rgb(106), rgb(134));
  init_color(color_surface, rgb(31), rgb(29), rgb(46));
  init_color(color_overlay, rgb(38), rgb(35), rgb(58));
  init_color(color_highlight, rgb(82), rgb(86), rgb(110));
  init_pair(1, COLOR_WHITE, color_highlight);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, color_muted, color_surface);
  init_pair(5, COLOR_WHITE, color_surface);
  init_pair(6, COLOR_WHITE, color_overlay);

  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  getmaxyx(stdscr, row, col);
  mvprintw(row-1, 0, "Press h for help and q to quit");
  curs_set(0);
  refresh();
  
  FILE *taskfile;
  if (access("tasks.txt", F_OK) != 0) {
  taskfile = fopen("tasks.txt", "w");
  fclose(taskfile);
  }

  FILE *donefile;
  if (access("done_tasks.txt", F_OK) != 0) {
  donefile = fopen("done_tasks.txt", "w");
  fclose(donefile);
  }

  WINDOW *tasks_win = newwin((row/2)+5, col, 0, 0);
  move(row/2, col/2);
  
  // Main loop
  while (ch!=113) {
    // Updating the tasks_win
    wclear(tasks_win);
    
    if (strcmp(mode, "pending") == 0) {
      taskfile = fopen("tasks.txt", "r");
      print_y=1;
      wattrset(tasks_win, COLOR_PAIR(5));
      wbkgd(tasks_win, COLOR_PAIR(5));
            while (fgets(tasks_string, 1024, taskfile)) {
        wmove(tasks_win, print_y, 2);
        wprintw(tasks_win, tasks_string);
        print_y += 1;
      }
      fclose(taskfile);
      box(tasks_win, 0, 0);
      mvwprintw(tasks_win, 0, 2, "PENDING TASKS");  
      mvwprintw(tasks_win, 0, 17, "COMPLETED TASKS");
      mvwchgat(tasks_win, 0, 17, 15, A_DIM, 5, NULL);
      mvwchgat(tasks_win, 0, 2, 13, A_STANDOUT | A_BOLD | A_UNDERLINE, 5, NULL);
      mvwchgat(tasks_win, tasks_win_y, 1, col-2, A_BOLD, 1, NULL);
    } else {
      donefile = fopen("done_tasks.txt", "r");
      print_y=1;
      wattrset(tasks_win, COLOR_PAIR(4));
      wbkgd(tasks_win, COLOR_PAIR(4));
            while (fgets(tasks_string, 1024, donefile)) {
        wmove(tasks_win, print_y, 2);
        wprintw(tasks_win, tasks_string);
        print_y += 1;
      }
      fclose(donefile);
      box(tasks_win, 0, 0);
      mvwprintw(tasks_win, 0, 2, "PENDING TASKS");  
      mvwprintw(tasks_win, 0, 17, "COMPLETED TASKS");
      mvwchgat(tasks_win, 0, 17, 15, A_STANDOUT | A_BOLD | A_UNDERLINE, 5, NULL);
      mvwchgat(tasks_win, 0, 2, 13, A_DIM, 5, NULL);
      mvwchgat(tasks_win, tasks_win_y, 1, col-2, A_STANDOUT | A_BOLD, 4, NULL);
    }
    
    curs_set(0);
    wrefresh(tasks_win);

    
    // Check for user input
    ch = getch();
    switch (ch) {
      case 106: // j 
      //getyx(stdscr, y, x);
      //move(y+1, x);
      tasks_win_y += 1;
      break;

      case 107: // k 
      //getyx(stdscr, y, x);
      //move(y-1, x);
      tasks_win_y -= 1;
      break;

      case 113: // q 
      break;

      case 9: // tab key
      if (strcmp(mode, "pending") == 0) {
          strcpy(mode, "completed");
        } else {
        strcpy(mode, "pending");
      }
      break;

      case 97: // a 
      getyx(stdscr, y, x);
      wmove(tasks_win, 1, 1);
      taskfile = fopen("tasks.txt", "a");
      fprintf(taskfile, "%s\n", create_input_box(y, x));
      fclose(taskfile);
      move(row/2, col/2);
      break;

      case 100: // d
      getyx(tasks_win, tasks_win_y, tasks_win_x);
      complete_task(taskfile, donefile, tasks_win_y);
      break;

      case 104: // h
      create_help_box();
      bkgd(COLOR_PAIR(0));
      refresh();
      break;

      case 120: //x
      clear_done();
      break;
    }
  }
  endwin();
  return 0;
}

//----------------------------------------------------
// Input box function
//----------------------------------------------------

const char *create_input_box(int y, int x) {
  static char task[30];
  WINDOW *input_box = newwin(3, 50, y-3, x-25);
  wbkgd(input_box, COLOR_PAIR(6));
  box(input_box, 0, 0);
  mvwprintw(input_box, 0, 1, "Name of task");  
  refresh();
  wmove(input_box, 1, 1);
  wrefresh(input_box);  
  echo();
  curs_set(2);
  wgetstr(input_box, task);
  wclear(input_box);
  wrefresh(input_box);
  delwin(input_box);
  noecho();
  return task;
}

//----------------------------------------------------
// Complete task function
//----------------------------------------------------

void complete_task(FILE *file, FILE *done, int t) {
  char buffer[2048];
  int line=1;
  FILE *temp;
  file = fopen("tasks.txt", "r");
  temp = fopen("temp_tasks.txt", "w");
  done = fopen("done_tasks.txt", "a");
  if (file == NULL || temp == NULL) {
    printf("Error opening file(s)\n");
  }
  
  while (!feof(file)) {
    fgets(buffer, 2048, file);
    if (line != t && !feof(file)) {
      fprintf(temp, "%s", buffer);
    } else if (line == t && !feof(file)) {
      fprintf(done, "%s", buffer);
    }
    line += 1;
  }

  fclose(file);
  fclose(temp);
  fclose(done);
  remove("tasks.txt");
  rename("temp_tasks.txt", "tasks.txt");
}

//----------------------------------------------------
// Help box function
//----------------------------------------------------

void create_help_box() {
  int row, col;
  getmaxyx(stdscr, row, col);
  WINDOW *help_box = newwin(2*(row/3), 2*(col/3), row/6, col/6);
  wbkgd(help_box, COLOR_PAIR(6));
  box(help_box, 0, 0);
  mvwprintw(help_box, 0, 2, "Tasklet - help");  
  mvwprintw(help_box, 1, 2, "j: move down");
  mvwprintw(help_box, 2, 2, "k: move up");
  mvwprintw(help_box, 3, 2, "a: add new task");
  mvwprintw(help_box, 4, 2, "d: mark selected task as done");
  mvwprintw(help_box, 5, 2, "tab: switch between pending and completed tasks");
  mvwprintw(help_box, 6, 2, "x: clear all completed tasks");
  mvwprintw(help_box, 7, 2, "h: help");
  mvwprintw(help_box, 8, 2, "q: quit");
  refresh();
  wmove(help_box, 1, 1);
  wrefresh(help_box);
  getch();
  wclear(help_box);
  wrefresh(help_box);
  delwin(help_box);
}

//----------------------------------------------------
// Clear tasks function
//----------------------------------------------------

void clear_done() {
  FILE *temp;
  temp = fopen("temp_done_tasks.txt", "w");
  fclose(temp);
  remove("done_tasks.txt");
  rename("temp_done_tasks.txt", "done_tasks.txt");
}

//----------------------------------------------------
// Warning box function
//----------------------------------------------------

