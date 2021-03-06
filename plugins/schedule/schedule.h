/*
    Digital Clock: schedule plugin
    Copyright (C) 2013-2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCHEDULE_SCHEDULE_H
#define SCHEDULE_SCHEDULE_H

#include "iclock_plugin.h"
#include "iplugin_init.h"

#include <QPointer>
#include <QSystemTrayIcon>

#include "core/task.h"

class QHotkey;
class QMediaPlayer;

namespace schedule {

class TasksStorage;
class TasksInvoker;

class Schedule : public IClockPlugin, public ITrayPluginInit
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID CLOCK_PLUGIN_INTERFACE_IID FILE "schedule.json")
  Q_INTERFACES(IClockPlugin ITrayPluginInit)

public:
  Schedule();

  void Init(QSystemTrayIcon* tray_icon) override;

  void InitSettings(SettingsStorage* backend, const QString& name) override;

public slots:
  void Start()     override;
  void Stop()      override;

  void Configure() override;

  void TimeUpdateListener() override;

signals:
  void defaultNotificationChanged(const Notification& n);

private slots:
  void TrayActivated(QSystemTrayIcon::ActivationReason reason);
  void TaskCompleted(const TaskPtr& task);
  void ShowSettingsDialog();
  void onPluginOptionChanged(const QString& key, const QVariant& value);
  void AddTask();

private:
  QPointer<QSystemTrayIcon> tray_icon_;
  QSystemTrayIcon* clock_icon_ = nullptr;
  QMenu* tray_menu_;

  TasksStorage* backend_;
  TasksInvoker* invoker_;

  QMediaPlayer* player_;

  Notification default_notification_;
  QHotkey* add_task_hotkey_ = nullptr;
};

} // namespace schedule

#endif // SCHEDULE_SCHEDULE_H
