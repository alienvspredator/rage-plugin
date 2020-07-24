#pragma once

namespace CCD {
class Plugin;

class PluginAttachment {
  Plugin *plugin;

 public:
  explicit PluginAttachment(Plugin *plugin);

  Plugin *getPlugin() const;
};
}  // namespace CCD
