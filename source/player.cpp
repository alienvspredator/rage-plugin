#include "player.h"
#include "plugin.h"

CCD::PlayerManager::PlayerManager(Plugin *plugin) : SmartEntityManager(plugin) {
  plugin->getMP()->AddEventHandler(this);
}
