#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "InventoryTypes.hpp"
#include "exceptions.hpp"

double Random() {
  static std::mt19937 generator(std::random_device{}());
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator);
}

class Fleet {
 private:
  std::vector<std::shared_ptr<InventoryItem>> items;
  std::weak_ptr<InventoryItem> focused;

 public:
  template <typename type, typename... Args>
  void AddItem(Args&&... args) {
    std::unique_ptr<InventoryItem> ptr = std::make_unique<type>(args...);
    if (IsIdExists(ptr)) {
      throw DuplicatedIdError();
    }
    if (!TechnicalInspection(ptr)) {
      throw RejectedInspectionError();
    }
    items.push_back(std::move(ptr));
  }
  void RemoveId(std::string id) {
    for (unsigned i = 0; i < items.size(); ++i) {
      if (items[i]->InventoryID() == id) {
        items.erase(items.begin() + i);
        return;
      }
    }
    throw IdNotFoundError();
  }
  void FocusId(std::string id) {
    for (unsigned i = 0; i < items.size(); ++i) {
      if (items[i]->InventoryID() == id) {
        focused = items[i];
        return;
      }
    }
    throw IdNotFoundError();
  }
  void ShowFocus() {
    if (focused.lock() == nullptr) {
      std::cout << "removed\n";
      return;
    }
    std::cout << *focused.lock() << '\n';
  }

  void Energy() {
    double answer = 0;
    for (std::shared_ptr<InventoryItem>& item : items) {
      answer += item->DailyEnergy();
    }
    std::cout << std::fixed;
    std::cout.precision(1);

    std::cout << answer << '\n';
  }
  void Novice() {
    int count = 0;
    for (std::shared_ptr<InventoryItem>& item : items) {
      if (item->IsForNovice()) {
        std::cout << *item << "\n";
        ++count;
      }
    }
    if (count == 0) {
      std::cout << "no transport for novice\n";
    }
  }
  void NoviceCount() {
    int count = 0;
    for (std::shared_ptr<InventoryItem>& item : items) {
      if (item->IsForNovice()) {
        ++count;
      }
    }
    std::cout << count << '\n';
  }
  void List() {
    if (items.empty()) {
      std::cout << "fleet is empty\n";
      return;
    }
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) { return *a < *b; });

    for (std::shared_ptr<InventoryItem>& item : items) {
      std::cout << *item << "\n";
    }
  }
  bool TechnicalInspection(const std::unique_ptr<InventoryItem>& item) {
    return Random() > item->BreakDownProbability();
  }
  bool IsIdExists(std::unique_ptr<InventoryItem>& item) {
    for (std::shared_ptr<InventoryItem>& current_item : items) {
      if (*item == *current_item) {
        return true;
      }
    }
    return false;
  }
};
