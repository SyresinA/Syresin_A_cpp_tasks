#pragma once

#include "exceptions.hpp"

template <typename type, int min_value, int max_value>
class ValidatedValue {
 private:
  type value_;

 public:
  ValidatedValue(type value) : value_{value} {
    if (!(value >= min_value && value <= max_value)) {
      throw InvalidArgumentError();
    }
  }
  type Get() const { return value_; }
};

class InventoryItem {
 protected:
  std::string id_;
  std::string name_;

 public:
  InventoryItem(std::string id, std::string name) : id_{id}, name_{name} {}
  virtual double DailyEnergy() const { return 0; }
  virtual bool IsForNovice() const { return false; }
  virtual std::string InventoryID() const = 0;
  virtual std::string Name() const = 0;
  virtual std::string Describe() const = 0;
  virtual double BreakDownProbability() const { return 0; }
  bool operator<(const InventoryItem& other) {
    return InventoryID() < other.InventoryID();
  }
  bool operator==(const InventoryItem& other) {
    return InventoryID() == other.InventoryID();
  }
  virtual ~InventoryItem() {}
};
std::ostream& operator<<(std::ostream& os, const InventoryItem& item) {
  os << item.Describe();
  return os;
}

class EnergyConsumer {
 protected:
  ValidatedValue<double, 0, 1000> daily_energy_;

 public:
  EnergyConsumer(double daily_energy) : daily_energy_{daily_energy} {}
  virtual double DailyEnergy() const = 0;
  virtual ~EnergyConsumer() {}
};

class Vehicle : public InventoryItem {
 protected:
  ValidatedValue<int, 1, 10> ease_of_usage_;

 public:
  Vehicle(std::string id, std::string name, int ease_of_usage)
      : InventoryItem(id, name), ease_of_usage_{ease_of_usage} {}
  virtual double BreakDownProbability() const { return 0.3; }
  virtual int EaseOfUsage() const { return ease_of_usage_.Get(); }
  virtual bool IsForNovice() const { return ease_of_usage_.Get() >= 6; }
};

class Helmet : public InventoryItem {
 public:
  Helmet(std::string id, std::string name) : InventoryItem(id, name) {}
  std::string InventoryID() const { return id_; }
  std::string Name() const { return name_; }
  std::string Describe() const { return "Helmet: " + id_ + " " + name_; }
};

class Bike : public Vehicle {
 public:
  Bike(std::string id, std::string name, int ease_of_usage)
      : Vehicle(id, name, ease_of_usage) {}
  std::string InventoryID() const { return id_; }
  std::string Name() const { return name_; }
  std::string Describe() const { return "Bike: " + id_ + " " + name_; }
};

class Dock : public InventoryItem, public EnergyConsumer {
 public:
  Dock(std::string id, std::string name, double daily_energy)
      : InventoryItem(id, name), EnergyConsumer(daily_energy) {}
  std::string InventoryID() const { return id_; }
  std::string Name() const { return name_; }
  double DailyEnergy() const { return daily_energy_.Get(); }
  std::string Describe() const { return "Dock: " + id_ + " " + name_; }
};

class Scooter : public Vehicle, public EnergyConsumer {
 public:
  Scooter(std::string id, std::string name, double daily_energy,
          int ease_of_usage)
      : Vehicle(id, name, ease_of_usage), EnergyConsumer(daily_energy) {}
  std::string InventoryID() const { return id_; }
  std::string Name() const { return name_; }
  double DailyEnergy() const { return daily_energy_.Get(); }
  std::string Describe() const { return "Scooter: " + id_ + " " + name_; }
};

class HoverBoard : public Vehicle, public EnergyConsumer {
 public:
  HoverBoard(std::string id, std::string name, double daily_energy,
             int ease_of_usage)
      : Vehicle(id, name, ease_of_usage), EnergyConsumer(daily_energy) {}
  std::string InventoryID() const { return id_; }
  std::string Name() const { return name_; }
  double DailyEnergy() const { return daily_energy_.Get(); }
  std::string Describe() const { return "Hoverboard: " + id_ + " " + name_; }
};
