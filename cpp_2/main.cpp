#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "InventoryTypes.hpp"
#include "exceptions.hpp"
#include "fleet.hpp"

class CommandHandler {
 private:
  bool end_ = false;
  Fleet fleet_;

  template <typename type>
  type ParseNumericType(const std::string& str) {
    std::size_t pos;
    try {
      type value;
      if constexpr (std::is_same_v<type, int>) {
        value = std::stoi(str, &pos);
      } else {
        value = std::stod(str, &pos);
      }
      if (pos != str.size()) {
        throw WrongArgumentTypeError();
      }
      return value;

    } catch (const std::invalid_argument&) {
      throw WrongArgumentTypeError();
    } catch (const std::out_of_range&) {
      throw WrongArgumentTypeError();
    }
  }

  void ArgumentNumberCheck(std::string command, unsigned excpected,
                           unsigned got) {
    if (excpected != got) {
      throw WrongArgumentsNumberError(command, got, excpected);
    }
  }

  void ParseAdd(const std::vector<std::string>& arguments) {
    if (arguments.size() == 0) {
      throw InvalidArgumentError();
    }
    std::string item_type = arguments[0];
    if (item_type == "helmet") {
      ArgumentNumberCheck("add helmet", 2, arguments.size() - 1);
      fleet_.AddItem<Helmet>(arguments[1], arguments[2]);
      return;
    }
    if (item_type == "bike") {
      ArgumentNumberCheck("add bike", 3, arguments.size() - 1);
      fleet_.AddItem<Bike>(arguments[1], arguments[2],
                           ParseNumericType<int>(arguments[3]));
      return;
    }
    if (item_type == "dock") {
      ArgumentNumberCheck("add dock", 3, arguments.size() - 1);
      fleet_.AddItem<Dock>(arguments[1], arguments[2],
                           ParseNumericType<double>(arguments[3]));
      return;
    }
    if (item_type == "scooter") {
      ArgumentNumberCheck("add scooter", 4, arguments.size() - 1);
      fleet_.AddItem<Scooter>(arguments[1], arguments[2],
                              ParseNumericType<double>(arguments[3]),
                              ParseNumericType<int>(arguments[4]));
      return;
    }
    if (item_type == "hoverboard") {
      ArgumentNumberCheck("add hoverboard", 4, arguments.size() - 1);
      fleet_.AddItem<HoverBoard>(arguments[1], arguments[2],
                                 ParseNumericType<double>(arguments[3]),
                                 ParseNumericType<int>(arguments[4]));
      return;
    }
    throw InvalidArgumentError();
  }
  void ParseEnergy(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("energy", 0, arguments.size());
    fleet_.Energy();
  }
  void ParseNovice(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("novice", 0, arguments.size());
    fleet_.Novice();
  }
  void ParseNoviceCount(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("novice_count", 0, arguments.size());
    fleet_.NoviceCount();
  }

  void ParseList(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("list", 0, arguments.size());
    fleet_.List();
  }

  void ParseRemove(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("remove", 1, arguments.size());
    fleet_.RemoveId(arguments.back());
  }

  void ParseFocus(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("focus", 1, arguments.size());
    fleet_.FocusId(arguments.back());
  }
  void ParseShowFocus(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("show_focus", 0, arguments.size());
    fleet_.ShowFocus();
  }

  void ParseQuit(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("quit", 0, arguments.size());
    end_ = true;
  }
  void ParseHelp(const std::vector<std::string>& arguments) {
    ArgumentNumberCheck("help", 0, arguments.size());
    std::cout << "add helmet id name\n";
    std::cout << "add bike id name ease\n";
    std::cout << "add dock id name energy\n";
    std::cout << "add scooter id name energy ease\n";
    std::cout << "add hoverboard id name energy ease\n";
    std::cout << "energy\n";
    std::cout << "novice\n";
    std::cout << "novice_count\n";
    std::cout << "list\n";
    std::cout << "focus id\n";
    std::cout << "show_focus\n";
    std::cout << "remove id\n";
    std::cout << "quit\n";
  }

  std::unordered_map<std::string,
                     void (CommandHandler::*)(const std::vector<std::string>&)>
      command_map{{"add", &CommandHandler::ParseAdd},
                  {"energy", &CommandHandler::ParseEnergy},
                  {"novice", &CommandHandler::ParseNovice},
                  {"novice_count", &CommandHandler::ParseNoviceCount},
                  {"list", &CommandHandler::ParseList},
                  {"remove", &CommandHandler::ParseRemove},
                  {"focus", &CommandHandler::ParseFocus},
                  {"show_focus", &CommandHandler::ParseShowFocus},
                  {"quit", &CommandHandler::ParseQuit},
                  {"help", &CommandHandler::ParseHelp}};

 public:
  bool End() { return end_; }
  void ParseCommand(std::string line) {
    std::string command;
    std::istringstream stream(line);
    stream >> command;
    if (command == "") {
      return;
    }
    auto it = command_map.find(command);
    if (it == command_map.end()) {
      throw CommandNotFoundError(command);
    }

    std::vector<std::string> arguments;
    std::string argument;
    while (stream >> argument) {
      arguments.push_back(argument);
    }
    (this->*(it->second))(arguments);
  }
};

int main() {
  CommandHandler handler;
  std::string current_line;
  while (!handler.End() && std::getline(std::cin, current_line)) {
    try {
      handler.ParseCommand(current_line);
    } catch (const FleetOperationError& error) {
      std::cout << "error: " << error.what() << '\n';
    }
  }
}