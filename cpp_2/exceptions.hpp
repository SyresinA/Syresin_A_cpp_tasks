#pragma once

#include <stdexcept>
#include <string>

class FleetOperationError : public std::runtime_error {
 public:
  FleetOperationError(std::string message) : std::runtime_error{message} {}
};

class RejectedInspectionError : public FleetOperationError {
 public:
  RejectedInspectionError() : FleetOperationError("inspection rejected") {}
};

class DuplicatedIdError : public FleetOperationError {
 public:
  DuplicatedIdError()
      : FleetOperationError("object with this ID already in fleet") {}
};

class IdNotFoundError : public FleetOperationError {
 public:
  IdNotFoundError() : FleetOperationError("object with this ID not found") {}
};

class CommandError : public FleetOperationError {
 public:
  CommandError(std::string message) : FleetOperationError(message) {}
};

class InvalidArgumentError : public CommandError {
 public:
  InvalidArgumentError() : CommandError("invalid arguments") {}
};

class CommandNotFoundError : public CommandError {
 public:
  CommandNotFoundError(std::string command)
      : CommandError("command '" + command +
                     "' not found. 'help' for command list") {}
};

class WrongArgumentsNumberError : public CommandError {
 public:
  WrongArgumentsNumberError(std::string command, unsigned arguments_number,
                            unsigned expected_arguments_number)
      : CommandError("command '" + command + "' takes " +
                     std::to_string(expected_arguments_number) +
                     " arguments, but " + std::to_string(arguments_number) +
                     " were given") {}
};

class WrongArgumentTypeError : public CommandError {
 public:
  WrongArgumentTypeError() : CommandError("wrong type of arguments") {}
};
