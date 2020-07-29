#include <ccd/pluginAttachment.h>

CCD::PluginAttachment::PluginAttachment(CCD::Plugin *plugin) {
  this->plugin = plugin;
}

CCD::Plugin *CCD::PluginAttachment::getPlugin() const { return this->plugin; }
