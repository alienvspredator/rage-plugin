#include "plugin.h"

CCD::Plugin::Plugin(rage::IMultiplayer *mp) {
  this->_mp = mp;
  mp->AddEventHandler(this);

  this->_console = new Console(this);
  this->_playerManager = new PlayerManager(this);

  this->initialized = true;
}

CCD::Plugin::~Plugin() {
  delete this->_console;
  delete this->_playerManager;

  std::exit(0);
}

rage::ITickHandler *CCD::Plugin::GetTickHandler() { return this; }

void CCD::Plugin::Tick() { this->getConsole()->update(); }

void CCD::Plugin::unloadScripts() {
  this->getConsole()->info("Unloading scripts...");
  this->getConsole()->info("Server shutdown");

  delete this;
}

void CCD::Plugin::Unload() {
  this->getMP()->AddRemoteEventHandler(
      u8"plugin:hi", [this](rage::IPlayer *player, const rage::args_t &args) {
        this->getConsole()->info("plugin:hi called");
      });

  this->getConsole()->info("Unloading plugin");

  this->initialized = false;
  this->unloadScripts();
}

rage::IMultiplayer *CCD::Plugin::getMP() const { return this->_mp; }

CCD::Console *CCD::Plugin::getConsole() const { return this->_console; }

bool CCD::Plugin::isInitialized() const { return this->initialized; }
