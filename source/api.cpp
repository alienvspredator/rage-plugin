#include "api.h"

#include "plugin.h"

RAGE_API rage::IPlugin *InitializePlugin(rage::IMultiplayer *mp) {
  return new CCD::Plugin(mp);
}
