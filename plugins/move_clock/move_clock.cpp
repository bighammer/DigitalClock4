/*
    Digital Clock: move clock plugin
    Copyright (C) 2014-2016  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "move_clock.h"

#include <cmath>

#include <QWidget>

#include "plugin_settings.h"

#include "move_clock_settings.h"

#include "gui/settings_dialog.h"

namespace move_clock {

double x(double c, double k, double a, double n, double t)
{
  return c + k*t + a*cos(n*t);
}

double y(double c, double k, double a, double n, double t)
{
  return c + k*t + a*sin(n*t);
}

MoveClock::MoveClock() : t_(0.0), dx_(0), dy_(0)
{
  timer_.setSingleShot(false);
  connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));

  InitTranslator(QLatin1String(":/move_clock/move_clock_"));
  info_.display_name = tr("Moving clock");
  info_.description = tr("Moves the clock.");
  InitIcon(":/move_clock/icon.svg");
}

void MoveClock::Init(QWidget* main_wnd)
{
  clock_wnd_ = main_wnd;
  old_pos_ = main_wnd->pos();

  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          this, SLOT(SettingsListener(QString,QVariant)));

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
}

void MoveClock::Start()
{
  settings_->Load();
  timer_.start();
}

void MoveClock::Stop()
{
  timer_.stop();
  clock_wnd_->move(old_pos_);
}

void MoveClock::Configure()
{
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          dialog, SLOT(SettingsListener(QString,QVariant)));
  settings_->Load();
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void MoveClock::SettingsListener(const QString& key, const QVariant& value)
{
  if (key == OPT_X0) x0_ = value.toInt();
  if (key == OPT_KX) kx_ = value.toReal();
  if (key == OPT_AX) ax_ = value.toInt();
  if (key == OPT_NX) nx_ = value.toReal();
  if (key == OPT_Y0) y0_ = value.toInt();
  if (key == OPT_KY) ky_ = value.toReal();
  if (key == OPT_AY) ay_ = value.toInt();
  if (key == OPT_NY) ny_ = value.toReal();
  if (key == OPT_TIMEOUT) timer_.setInterval(value.toInt());
}

void MoveClock::TimeoutHandler()
{
  int cx = x(x0_, kx_, ax_, nx_, t_) + dx_;
  int sw = desktop_.screen()->width();
  int cw = clock_wnd_->width();
  if (cx > sw) {
    cx = -cw;
    dx_ -= sw + cw;
  }
  if (cx < -cw) {
    cx = sw;
    dx_ += sw + cw;
  }
  int cy = y(y0_, ky_, ay_, ny_, t_) + dy_;
  int sh = desktop_.screen()->height();
  int ch = clock_wnd_->height();
  if (cy > sh) {
    cy = -ch;
    dy_ -= sh + ch;
  }
  if (cy < -ch) {
    cy = sh;
    dy_ += sh + ch;
  }
  clock_wnd_->move(cx, cy);
  t_ += 0.5;
}

} // namespace move_clock
