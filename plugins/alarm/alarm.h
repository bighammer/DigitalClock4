#ifndef ALARM_H
#define ALARM_H

#include <QIcon>
#include <QMediaPlayer>
#include "iclock_plugin.h"
#include "plugin_settings.h"

class Alarm : public ITrayPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID TRAY_PLUGIN_INTERFACE_IID FILE "alarm.json")
  Q_INTERFACES(IClockPlugin ITrayPlugin)

public:
  Alarm();
  void Init(QSystemTrayIcon* tray_icon);
  void GetInfo(TPluginInfo* info);

public slots:
  void Start();
  void Stop();
  void Configure();
  void SettingsListener(Options, const QVariant&) {}
  void TimeUpdateListener(const QString&);

private:
  QSystemTrayIcon* tray_icon_;
  QIcon old_icon_;
  PluginSettings* settings_;
  QMediaPlayer* player_;
};

#endif // ALARM_H