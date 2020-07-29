#pragma once

#include <rage/rage.hpp>

namespace CCD {
class PlayerManager;
class Console;

class Plugin : public rage::IPlugin,
               public rage::IEventHandler,
               public rage::ITickHandler,
               public rage::IPlayerHandler {
  void unloadScripts();

 public:
  explicit Plugin(rage::IMultiplayer *mp);
  ~Plugin();

  rage::ITickHandler *GetTickHandler() override;

  void Tick() override;
  void Unload() override;

  rage::IMultiplayer *getMP() const;
  PlayerManager *getPlayerManager() const;
  Console *getConsole() const;

  bool isInitialized() const;

 private:
  rage::IMultiplayer *_mp;
  PlayerManager *_playerManager;
  Console *_console;

  bool initialized = false;
};
}  // namespace CCD
