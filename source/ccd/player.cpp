#include <ccd/player.h>
#include <ccd/plugin.h>

CCD::PlayerManager::PlayerManager(Plugin *plugin) : SmartEntityManager(plugin) {
  plugin->getMP()->AddEventHandler(this);
}
