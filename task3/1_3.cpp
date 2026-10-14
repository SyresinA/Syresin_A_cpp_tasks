#include <iomanip>
#include <iostream>

bool command_equal(const char* c1, const char* c2) {
  int i = 0;
  while (c1[i] != '\0' && c2[i] != '\0') {
    if (c1[i] != c2[i]) {
      return false;
    }
    ++i;
  }
  return c1[i] == c2[i];
}

bool Clear(char display[][10], std::istream&) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      display[i][j] = ' ';
    }
  }
  return true;
}

bool Print(char display[][10], std::istream&) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      std::cout << display[i][j];
    }
    std::cout << "\n";
  }
  return true;
}
bool Put(char display[][10], std::istream& in) {
  int x;
  int y;
  char c;
  in >> x >> y >> c;
  if (x > 9 || x < 0 || y > 4 || y < 0) {
    return false;
  }
  display[y][x] = c;
  return true;
}
bool Text(char display[][10], std::istream& in) {
  int x;
  int y;
  char word[10];
  in >> x >> y;
  char cur_char;
  int cur_char_pos = x - 1;
  in.ignore();
  while (in.get(cur_char) && cur_char != '\n') {
    ++cur_char_pos;
    if (cur_char_pos <= 9 && cur_char_pos >= 0) {
      word[cur_char_pos] = cur_char;
    }
  }
  if (x > 9 || x < 0 || y > 4 || y < 0) {
    return false;
  }
  if (cur_char_pos > 9) {
    return false;
  }
  for (int i = x; i <= cur_char_pos; ++i) {
    display[y][i] = word[i];
  }
  return true;
}

int main() {
  const char* keys[]{"clear", "put", "text", "print"};
  bool (*operations[])(char (*)[10], std::istream&){Clear, Put, Text, Print};
  char display[5][10];
  Clear(display, std::cin);

  char command[100];
  while (std::cin >> std::setw(100) >> command) {
    bool (*operation)(char (*)[10], std::istream&){nullptr};
    if (command_equal(command, "quit")) {
      break;
    }
    for (int i = 0; i < int(std::size(keys)); i++) {
      if (command_equal(keys[i], command)) {
        operation = operations[i];
      }
    }
    if (operation == nullptr) {
      std::cout << "error" << "\n";
      continue;
    }

    if (!operation(display, std::cin)) {
      std::cout << "error" << "\n";
    }
  }
}