#pragma once
#include <plog/Log.h>

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

#include "pluginAttachment.h"

enum { FILE_LOG = 1 };

namespace CCD {
class Console : public PluginAttachment {
 public:
  explicit Console(Plugin *plugin);
  ~Console();

  static Console *getInstance() { return Console::instance; }

  template <typename T>
  inline Console const *debug(T const &log) const {
    PLOGD << log;
    PLOGD_(FILE_LOG) << log;
    return this;
  }

  template <typename T>
  inline Console const *info(T const &log) const {
    PLOGI << log;
    PLOGI_(FILE_LOG) << log;
    return this;
  }

  template <typename T>
  inline Console const *warn(T const &log) const {
    PLOGW << log;
    PLOGW_(FILE_LOG) << log;
    return this;
  }

  template <typename T>
  inline Console const *error(T const &log) const {
    PLOGE << log;
    PLOGE_(FILE_LOG) << log;
    return this;
  }

  template <typename T>
  inline Console const *critical(T const &log) const {
    PLOGE << log;
    PLOGE_(FILE_LOG) << log;
    return this;
  }

 private:
  friend class Plugin;

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
