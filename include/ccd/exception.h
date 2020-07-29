#pragma once

#include <iostream>

namespace CCD {
class Exception {
 public:
  Exception(std::string const &message);
  std::string getMessage() const;

 private:
  std::string message;
};

class ChatException : Exception {
 public:
  ChatException(std::vector<std::string> const &messages);

  std::vector<std::string> &getMessages() const;

 private:
  std::vector<std::string> const &messages;
};
}  // namespace CCD
