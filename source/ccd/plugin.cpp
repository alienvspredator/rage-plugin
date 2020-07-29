#include <ccd/console.h>
#include <ccd/player.h>
#include <ccd/plugin.h>

CCD::Plugin::Plugin(rage::IMultiplayer *mp) {
  this->_mp = mp;
  mp->AddEventHandler(this);

  this->_console = new Console(this);
  this->_playerManager = new PlayerManager(this);

  this->initialized = true;
}

CCD::Plugin::~Plugin() {
  delete this->_console;
  // delete this->_playerManager;

  std::exit(0);
}

auto CCD::Plugin::GetTickHandler() -> rage::ITickHandler * { return this; }

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

auto CCD::Plugin::getMP() const -> rage::IMultiplayer * { return this->_mp; }

auto CCD::Plugin::getPlayerManager() const -> PlayerManager * {
  return this->_playerManager;
}

auto CCD::Plugin::getConsole() const -> Console * { return this->_console; }

auto CCD::Plugin::isInitialized() const -> bool { return this->initialized; }
