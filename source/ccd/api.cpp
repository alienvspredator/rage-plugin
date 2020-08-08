#include <ccd/api.h>
#include <ccd/plugin.h>
#include <gsl/gsl>

RAGE_API auto InitializePlugin(rage::IMultiplayer *mp) -> gsl::owner<rage::IPlugin *> {
  return gsl::owner<CCD::Plugin*>(new CCD::Plugin(mp));
}
