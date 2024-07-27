#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <string>
#include <vector>

const int height = 3;

inline void box(WINDOW* win) noexcept {
  werase(win);
  box(win, 0, 0);
  mvwprintw(win, 0, 2, "Input");
  wrefresh(win);
}

inline void startui() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

inline void cleanup() {
  endwin();
}

inline void update(WINDOW* chat_win, const std::vector<std::string>& messages, int start_line) {
  werase(chat_win);
  int line_count = 0;
  for (size_t i = start_line; i < messages.size(); ++i, ++line_count) {
    mvwprintw(chat_win, line_count, 0, "%s", messages[i].c_str());
  }
  wrefresh(chat_win);
}

inline std::string getinp(WINDOW* input_win) {
  std::string input;
  int ch;
  while ((ch = wgetch(input_win)) != '\n') {
    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (!input.empty()) {
        input.pop_back();
        int cur_x = getcurx(input_win);
        mvwdelch(input_win, 1, cur_x - 1);
        }
    } else {
        input.push_back(static_cast<char>(ch));
        waddch(input_win, ch);
      }
  wrefresh(input_win);
  }
  return input;
}

#endif
