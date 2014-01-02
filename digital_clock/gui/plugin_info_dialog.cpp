#include "plugin_info_dialog.h"
#include "ui_plugin_info_dialog.h"

namespace digital_clock {

PluginInfoDialog::PluginInfoDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::PluginInfoDialog) {
  ui->setupUi(this);
}

PluginInfoDialog::~PluginInfoDialog() {
  delete ui;
}

void PluginInfoDialog::SetInfo(const TPluginInfo& info) {
  ui->name_value->setText(info.gui_info.display_name);
  ui->version_value->setText(tr("version: %1").arg(info.metadata[PI_VERSION]));
  if (info.gui_info.icon.isNull()) ui->icon_label->hide();
  else ui->icon_label->setPixmap(QPixmap::fromImage(info.gui_info.icon));
  ui->description_value->setText(info.gui_info.description);
  ui->author_value->setText(info.metadata[PI_AUTHOR]);
  ui->email_value->setText(info.metadata[PI_EMAIL]);
  adjustSize();
  setMaximumSize(sizeHint());
}

} // namespace digital_clock