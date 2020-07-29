#include <ccd/api.h>
#include <ccd/plugin.h>

RAGE_API rage::IPlugin *InitializePlugin(rage::IMultiplayer *mp) {
  return new CCD::Plugin(mp);
}
