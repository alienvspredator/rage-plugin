#pragma once
#include <ccd/exception.h>
#include <ccd/pluginAttachment.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

#define CONSOLE_LOG "console"
#define FILE_LOG "file"

namespace CCD {
class Console : public PluginAttachment {
 public:
  friend class Plugin;

  explicit Console(Plugin *plugin);
  ~Console();

  static Console *getInstance() { return Console::instance; }

  template <typename T>
  inline Console const *trace(T const &log) const {
    spdlog::get(CONSOLE_LOG)->trace(log);
    spdlog::get(FILE_LOG)->trace(log);

    return this;
  }

  template <typename T>
  inline Console const *debug(T const &log) const {
    spdlog::get(CONSOLE_LOG)->debug(log);
    spdlog::get(FILE_LOG)->debug(log);

    return this;
  }

  template <typename T>
  inline Console const *info(T const &log) const {
    spdlog::get(CONSOLE_LOG)->info(log);
    spdlog::get(FILE_LOG)->info(log);

    return this;
  }

  template <typename T>
  inline Console const *warn(T const &log) const {
    spdlog::get(CONSOLE_LOG)->warn(log);
    spdlog::get(FILE_LOG)->warn(log);

    return this;
  }

  template <typename T>
  inline Console const *error(T const &log) const {
    spdlog::get(CONSOLE_LOG)->error(log);
    spdlog::get(FILE_LOG)->error(log);

    return this;
  }

  template <typename T>
  inline Console const *critical(T const &log) const {
    spdlog::get(CONSOLE_LOG)->critical(log);
    spdlog::get(FILE_LOG)->critical(log);

    return this;
  }

 private:
  static Console *instance;

  std::thread _thread;
  std::atomic<bool> _threadShouldRun;

  std::mutex _outputLock;
  std::queue<std::string> _output;

  void update();
  void threadFunc();

  void initializeLogger();
  void disposeLogger();
};
}  // namespace CCD
