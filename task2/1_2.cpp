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
void Deposit(int& balance, std::istream& in) {
  int k;
  in >> k;
  if (k <= 0) {
    std::cout << "error" << "\n";
    return;
  }
  balance += k;
}
void Withdraw(int& balance, std::istream& in) {
  int k;
  in >> k;
  if (k <= 0) {
    std::cout << "error" << "\n";
    return;
  }
  if (k > balance) {
    std::cout << "error" << "\n";
    return;
  }
  balance -= k;
}
void Balance(int& balance, std::istream&) { std::cout << balance << "\n"; }
void Reset(int& balance, std::istream&) { balance = 0; }
void Inc(int& balance, std::istream&) { ++balance; }

int main() {
  const char* keys[]{"deposit", "withdraw", "balance", "reset", "inc"};
  void (*operations[])(int&, std::istream&){Deposit, Withdraw, Balance, Reset,
                                            Inc};
  int balance{0};

  char command[100];

  while (std::cin >> std::setw(100) >> command) {
    void (*operation)(int&, std::istream&){nullptr};

    if (command_equal(command, "stop")) {
      break;
    }
    for (int i = 0; i < int(std::size(keys)); i++) {
      if (command_equal(command, keys[i])) {
        operation = operations[i];
      }
    }
    if (operation == nullptr) {
      std::cout << "error" << "\n";
      continue;
    }
    operation(balance, std::cin);
  }
}