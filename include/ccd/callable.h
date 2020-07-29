#pragma once

#include <ccd/exception.h>
#include <ccd/player.h>
#include <ccd/vehicle.h>

#include <sstream>
#include <variant>

namespace CCD {
class Player;

typedef std::variant<bool, int, uint32_t, float, uint8_t, Player *, Vehicle *,
                     void *>
    ArgVal;

struct ArgumentValue {
  ArgVal arg;
  std::string str;
};

class ArgumentParseException : public Exception {
 public:
  explicit ArgumentParseException(std::string const &msg);
};

class ArgumentOutOfLimits : public ArgumentParseException {
 public:
  explicit ArgumentOutOfLimits(std::string const &msg);
};

class IArgument {
 public:
  enum class InputType { NONE, WORD, DUMP };
  explicit IArgument(InputType it);

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(std::string const &str) = 0;
  InputType getInputType() const;

 private:
  InputType inputType;
};

class IntArgument : public IArgument {
  IntArgument();

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(std::string const &str) override;

  static int prase(std::string const &str);
};

class UnsignedInt32Argument : public IArgument {
 public:
  UnsignedInt32Argument();

  virtual ArgumentValue parseValue(std::string const &str) override;

  static uint32_t parse(std::string const &str);
};

class LimitedIntArgument : public IArgument {
 public:
  LimitedIntArgument(int min, int max);

  virtual ArgumentValue parseValue(std::string const &str) override;

  static int parse(std::string const &str, int min, int max);

  void setMin(int min);
  void setMax(int max);

  int getMin() const;
  int getMax() const;

 private:
  int min;
  int max;
};

/**
 * Same as LimitedIntArgument, but instead of throwing exception returns
 * specified default value.
 */
class LimitedDefaultIntArgument : public IArgument {
 public:
  LimitedDefaultIntArgument(int min, int max, int def);

  virtual ArgumentValue parseValue(const std::string &str) override;

  static float parse(std::string const &str, int min, int max, int def);

  int getMin() const;
  int getMax() const;
  int getDefault() const;

 private:
  int min;
  int max;
  int def;
};

class ByteArgument : public IArgument {
 public:
  ByteArgument();

  virtual ArgumentValue parseValue(std::string const &str) override;

  static uint8_t parse(std::string const &str);
};

class FloatArgument : public IArgument {
 public:
  FloatArgument();

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(const std::string &str) override;

  static float parse(std::string const &str);
};

class LimitedFloatArgument : public IArgument {
 public:
  LimitedFloatArgument(float min, float max);

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(const std::string &str) override;

  static float parse(std::string const &str, float min, float max);

  float getMin() const;
  float getMax() const;

 private:
  float min;
  float max;
};

class LimitedDefaultFloatArgument : public IArgument {
 public:
  LimitedDefaultFloatArgument(float min, float max, float def);

  virtual ArgumentValue parseValue(const std::string &str) override;

  static float parse(std::string &str, float min, float max, float def);

  float getDefault() const;

 private:
  float min;
  float max;
  float def;
};

class StringArgument : public IArgument {
 public:
  StringArgument();

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(const std::string &str) override;
};

class StringDumpArgument : public IArgument {
 public:
  StringDumpArgument();

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(const std::string &str) override;
};

class BoolArgument : public IArgument {
 public:
  BoolArgument();

  /**
   * @throws ArgumentParseException
   */
  virtual ArgumentValue parseValue(const std::string &str) override;

  static bool parse(std::string const &str);
};

class PlayerArgument : public IArgument {
 public:
  PlayerArgument();

  virtual ArgumentValue parseValue(const std::string &str) override;

  static Player *parse(std::string const &str, PlayerManager const *m);
};

class VehicleArgument : public IArgument {
 public:
  VehicleArgument();

  virtual ArgumentValue parseValue(const std::string &str) override;

  static Vehicle *parse(std::string const &str, VehicleManager const *m);
};

class CallableArgumentCountException : public ChatException {
 public:
  explicit CallableArgumentCountException(
      std::initializer_list<std::string> const &exception);
  explicit CallableArgumentCountException(
      std::vector<std::string> const &exception);
};

template <typename T>
class Callable {
 public:
  explicit Callable(std::vector<IArgument *> const &mandatoryArgs,
                    std::vector<IArgument *> const &optionalArgs);
  virtual ~Callable();

  /**
   * Only throws CallableArgumentCountException,
   * if mandatory arguments are not provided.
   *
   * @throws CallableArgumentCountException
   * @throws ArgumentParseException
   */
  virtual T onExecute(std::string const &str);
  virtual T onExecute(std::vector<std::string> const &arg);
  virtual T onExecute(std::vector<ArgumentValue> const &arg) = 0;

  std::vector<IArgument *> const &getMandatoryArgs() const;
  std::vector<IArgument *> const &getOptionalArgs() const;

 protected:
  [[noreturn]] virtual void throwArgsCountException();

 private:
  std::vector<IArgument *> mandatoryArgs;
  std::vector<IArgument *> optionalArgs;
};
}  // namespace CCD

template <typename T>
[[noreturn]] void CCD::Callable<T>::throwArgsCountException() {
  throw CallableArgumentCountException(
      {u8"wrong number of arguments was passed to callable"});
}

template <typename T>
CCD::Callable<T>::Callable(std::vector<IArgument *> const &mandatoryArgs,
                           std::vector<IArgument *> const &optArgs) {
  this->mandatoryArgs = mandatoryArgs;
  this->optionalArgs = optArgs;
}

template <typename T>
CCD::Callable<T>::~Callable() {
  for (auto arg : this->getMandatoryArgs()) delete arg;
  this->getMandatoryArgs().clear();

  for (auto arg : this->getOptionalArgs()) delete arg;
  this->getOptionalArgs().clear();
}

template <typename T>
T CCD::Callable<T>::onExecute(std::string const &str) {
  std::vector<std::string> res;
  std::stringstream args(str);

  std::string arg;
  while (args >> arg) res.push_back(arg);

  return this->onExecute(res);
}

template <typename T>
T CCD::Callable<T>::onExecute(std::vector<std::string> const &args) {
  size_t len = 0;
  for (auto arg : this->getMandatoryArgs())
    if (arg->getInputType() != IArgument::InputType::NONE) len++;

  if (args.size() < len) this->throwArgsCountException();

  std::vector<ArgumentValue> result;

  size_t argc = 0;
  for (size_t i = 0; i < args.size(); i++) {
    IArgument *argv = nullptr;
    while (true) {
      if (i < this->getMandatoryArgs().size())
        argv = this->getMandatoryArgs().at(i);
      else if (i <
               this->getMandatoryArgs().size() + this->getOptionalArgs().size())
        argv = this->getOptionalArgs().at(i - this->getMandatoryArgs().size());
      else
        break;

      if (argv->getInputType() != IArgument::InputType::NONE) break;

      result.push_back(argv->parseValue(""));
      argc++;
    }

    if (argv == nullptr) break;

    if (argv->getInputType() == IArgument::InputType::DUMP) {
      std::string constructedString = args[i];
      for (size_t j = i + 1; j < args.size(); j++)
        constructedString += " " + args[j];

      result.push_back(argv->parseValue(constructedString));
      break;
    }

    result.push_back(argv->parseValue(args.at(i)));
    argc++;
  }

  return this->onExecute(result);
}

template <typename T>
std::vector<CCD::IArgument *> const &CCD::Callable<T>::getMandatoryArgs()
    const {
  return this->mandatoryArgs;
}

template <typename T>
std::vector<CCD::IArgument *> const &CCD::Callable<T>::getOptionalArgs() const {
  return this->optionalArgs;
}
