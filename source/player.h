#pragma once

#include "entity.h"

namespace CCD {
class PlayerManager : SmartEntityManager,
                      rage::IEventHandler,
                      rage::IPlayerHandler {
 public:
  explicit PlayerManager(Plugin *plugin);
  virtual ~PlayerManager() = default;
};
}  // namespace CCD
