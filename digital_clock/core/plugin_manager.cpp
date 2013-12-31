#include <QFile>
#include <QJsonObject>
#include "../gui/main_window.h"
#include "plugin_manager.h"

PluginManager::PluginManager(QObject *parent)
  : QObject(parent) {
}

void PluginManager::SetInitData(const TPluginData& data) {
  data_ = data;
}

void PluginManager::AddPluginsDir(const QDir& dir) {
  plugins_dirs_.append(dir);
}

void PluginManager::DelPluginsDir(const QDir& dir) {
  plugins_dirs_.removeOne(dir);
}

void PluginManager::ListAvailable() {
  available_.clear();
  QList<QPair<TPluginInfo, bool> > plugins;
  for (auto& dir : plugins_dirs_) {
    QStringList files = dir.entryList(QDir::Files);
    for (auto& file : files) {
      QString abs_path = dir.filePath(file);
      QPluginLoader loader(abs_path);
      IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader.instance());
      if (plugin) {
        QJsonObject metadata = loader.metaData().value("MetaData").toObject();
        TPluginInfo info;
        info.metadata[PI_NAME] = metadata.value("name").toString();
        info.metadata[PI_VERSION] = metadata.value("version").toString();
        info.metadata[PI_AUTHOR] = metadata.value("author").toString();
        info.metadata[PI_EMAIL] = metadata.value("email").toString();
        info.gui_info = plugin->GetInfo();
        available_[info.metadata[PI_NAME]] = abs_path;
        plugins.append(qMakePair(info, metadata.value("configurable").toBool()));
        loader.unload();
      }
    }
  }
  emit SearchFinished(plugins);
}

void PluginManager::LoadPlugins(const QStringList& names) {
  for (auto& name : names) LoadPlugin(name);
}

void PluginManager::UnloadPlugins(const QStringList& names) {
  if (!names.isEmpty()) {
    for (auto& name : names) UnloadPlugin(name);
  } else {
    QList<QString> plugins = loaded_.keys();
    for (auto& plugin : plugins) UnloadPlugin(plugin);
  }
}

void PluginManager::EnablePlugin(const QString& name, bool enable) {
  enable ? LoadPlugin(name) : UnloadPlugin(name);
}

void PluginManager::ConfigurePlugin(const QString& name) {
  if (loaded_.contains(name)) {
    IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loaded_[name]->instance());
    if (plugin) plugin->Configure();
  } else {
    QString file = available_[name];
    if (!QFile::exists(file)) return;
    QPluginLoader* loader = new QPluginLoader(file, this);
    IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
    if (plugin) {
      InitPlugin(plugin, false);
      plugin->Configure();
    }
  }
}

void PluginManager::LoadPlugin(const QString& name) {
  QString file = available_[name];
  if (!QFile::exists(file)) return;
  QPluginLoader* loader = new QPluginLoader(file, this);
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    InitPlugin(plugin, true);
    plugin->Start();
    loaded_[name] = loader;
  }
}

void PluginManager::UnloadPlugin(const QString& name) {
  QPluginLoader* loader = loaded_[name];
  if (!loader) return;
  IClockPlugin* plugin = qobject_cast<IClockPlugin*>(loader->instance());
  if (plugin) {
    disconnect(data_.settings, SIGNAL(OptionChanged(Options,QVariant)),
               plugin, SLOT(SettingsListener(Options,QVariant)));
    disconnect(data_.clock, SIGNAL(ImageNeeded(QString)),
               plugin, SLOT(TimeUpdateListener()));
    plugin->Stop();
    loader->unload();
    loaded_.remove(name);
  }
}

void PluginManager::InitPlugin(IClockPlugin* plugin, bool connected) {
  // connect slots which are common for all plugins
  if (connected) {
    connect(data_.settings, SIGNAL(OptionChanged(Options,QVariant)),
            plugin, SLOT(SettingsListener(Options,QVariant)));
    connect(data_.clock, SIGNAL(ImageNeeded(QString)),
            plugin, SLOT(TimeUpdateListener()));
  }
  // init settings plugins
  ISettingsPlugin* sp = qobject_cast<ISettingsPlugin*>(plugin);
  if (sp) {
    sp->Init(data_.settings->GetSettings());
    if (connected) {
      connect(sp, SIGNAL(OptionChanged(Options,QVariant)),
              data_.window, SLOT(SettingsListener(Options,QVariant)));
    }
  }
  // init tray plugins
  ITrayPlugin* tp = qobject_cast<ITrayPlugin*>(plugin);
  if (tp) {
    tp->Init(data_.tray);
  }
  // init widget plugins
  IWidgetPlugin* wp = qobject_cast<IWidgetPlugin*>(plugin);
  if (wp) {
    wp->Init(data_.window);
  }
  // pass current clock settings to plugin
  data_.settings->EmitSettings();
}
