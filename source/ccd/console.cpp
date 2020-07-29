#include <ccd/console.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <iostream>

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
  spdlog::stdout_color_mt(CONSOLE_LOG);
  auto consoleLogger = spdlog::get(CONSOLE_LOG);
  consoleLogger->set_level(spdlog::level::info);

  if (!std::filesystem::exists("logs"))
    std::filesystem::create_directory("logs");

  spdlog::basic_logger_mt(FILE_LOG, "logs/server.log");

  auto fileLogger = spdlog::get(FILE_LOG);
  fileLogger->flush_on(spdlog::level::err);
  fileLogger->set_level(spdlog::level::trace);

  this->trace("Logger has been initialized!");
}

void CCD::Console::disposeLogger() { this->debug("Logger has been destroyed"); }
