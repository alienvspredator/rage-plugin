#include "console.h"

#include <plog/Appenders/ColorConsoleAppender.h>

#include <filesystem>

CCD::Console *CCD::Console::instance = nullptr;

void CCD::Console::threadFunc() {
  while (this->_threadShouldRun) {
    std::string input;
    std::getline(std::cin, input, '\n');

    this->_outputLock.lock();
    this->_output.push(input);
    this->_outputLock.unlock();

    if (input == "exit" || input == "stop") {
      break;
    }
  }
}

void CCD::Console::update() {
  if (this->_outputLock.try_lock()) {
    std::vector<std::string> tasks;

    while (!this->_output.empty()) {
      tasks.push_back(this->_output.back());
      this->_output.pop();
    }

    this->_outputLock.unlock();
  }
}

#define HELP_CMD_COUNT_PER_PAGE 10

CCD::Console::Console(Plugin *plugin) : PluginAttachment(plugin) {
  Console::instance = this;

  this->initializeLogger();
  this->_threadShouldRun = true;
  this->_thread = std::thread(&Console::threadFunc, this);
}

CCD::Console::~Console() {
  this->info("Waiting for Console thread to join...");
  this->_threadShouldRun = false;
  this->_thread.join();
  this->info("Console thread has joined.");
  this->info("Server has been shutdown");
  this->disposeLogger();
}

void CCD::Console::initializeLogger() {
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::info, &consoleAppender);

  if (!std::filesystem::exists("logs"))
    std::filesystem::create_directory("logs");

  plog::init<FILE_LOG>(plog::debug, "logs/ccd.log");

  this->debug("Logger has been initalized");
}

void CCD::Console::disposeLogger() { this->debug("Logger has been destroyed"); }
