#include <iomanip>
#include <iostream>

const int cPrecision = 1;

double Mean(const int size, const int numbers[]) {
  double summ = 0;
  for (int i = 0; i < size; i++) {
    summ += numbers[i];
  }
  return summ / size;
}

int* MinMax(const int size, const int numbers[]) {
  int* answer{new int[2]};
  answer[0] = numbers[0];
  answer[1] = numbers[0];
  for (int i = 1; i < size; i++) {
    if (numbers[i] < answer[0]) {
      answer[0] = numbers[i];
    }
    if (numbers[i] > answer[1]) {
      answer[1] = numbers[i];
    }
  }
  return answer;
}

int NumberOfNegativeElements(const int size, const int numbers[]) {
  int answer = 0;
  for (int i = 0; i < size; i++) {
    if (numbers[i] < 0) {
      ++answer;
    }
  }
  return answer;
}

int main() {
  int n;
  std::cin >> n;
  int* temperatures{new int[n]};
  for (int i = 0; i < n; i++) {
    std::cin >> temperatures[i];
  }

  std::cout << std::fixed << std::setprecision(cPrecision);
  std::cout << Mean(n, temperatures) << "\n";

  int* min_max = MinMax(n, temperatures);
  std::cout << min_max[0] << " " << min_max[1] << "\n";
  delete[] min_max;

  std::cout << NumberOfNegativeElements(n, temperatures);

  delete[] temperatures;
}