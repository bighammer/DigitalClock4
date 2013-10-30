#include "any_zoom_settings.h"
#include "any_zoom.h"

AnyZoom::AnyZoom() {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  is_enabled_ = false;
  last_zoom_ = 1.0;
}

void AnyZoom::Init(const QMap<Options, QVariant>& current_settings, QWidget* parent) {
  parent_ = parent;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);

  settings_->Load();
  last_zoom_ = current_settings[OPT_ZOOM].toReal();
}

void AnyZoom::GetInfo(TPluginInfo* info) {
  info->insert(PI_NAME, "Any zoom");
  info->insert(PI_TYPE, "settings");
  info->insert(PI_VERSION, "1.0");
  info->insert(PI_AUTHOR, "Nick Korotysh");
  info->insert(PI_EMAIL, "nick.korotysh@gmail.com");
  info->insert(PI_COMMENT, "Can set any clock zoom.");
  info->insert(PI_CONFIG, "true");
}

void AnyZoom::Start() {
  is_enabled_ = true;
  emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
}

void AnyZoom::Stop() {
  is_enabled_ = false;
  emit OptionChanged(OPT_ZOOM, last_zoom_);
}

void AnyZoom::Configure() {
  if (settings_dlg_) {
    settings_dlg_->activateWindow();
  } else {
    settings_dlg_ = new QInputDialog(parent_);
    settings_dlg_->setAttribute(Qt::WA_DeleteOnClose);

    settings_dlg_->setWindowTitle("Any zoom settings");
    settings_dlg_->setLabelText("zoom:");

    settings_dlg_->setInputMode(QInputDialog::IntInput);
    settings_dlg_->setIntRange(1, 1000000);
    settings_dlg_->setIntValue(settings_->GetOption(OPT_CURRENT_ZOOM).toInt());

    connect(settings_dlg_, SIGNAL(intValueChanged(int)), this, SLOT(TrackChange(int)));
    connect(settings_dlg_, SIGNAL(intValueSelected(int)), this, SLOT(TrackChange(int)));
    connect(settings_dlg_, SIGNAL(accepted()), settings_, SLOT(Save()));
    connect(settings_dlg_, SIGNAL(rejected()), this, SLOT(RevertSettings()));

    settings_dlg_->show();
  }
}

void AnyZoom::TrackChange(int new_zoom) {
  settings_->SetOption(OPT_CURRENT_ZOOM, new_zoom);
  if (is_enabled_) emit OptionChanged(OPT_ZOOM, new_zoom / 100.);
}

void AnyZoom::RevertSettings() {
  settings_->Load();
  if (is_enabled_)
    emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
}

void AnyZoom::SettingsListener(Options option, const QVariant& value) {
  if (option == OPT_ZOOM && is_enabled_) {
    last_zoom_ = value.toReal();
    emit OptionChanged(OPT_ZOOM, settings_->GetOption(OPT_CURRENT_ZOOM).toInt() / 100.);
  }
}