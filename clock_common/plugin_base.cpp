#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include "plugin_base.h"

PluginBase::PluginBase() : translator_(0) {
}

PluginBase::~PluginBase() {
  if (translator_) QApplication::removeTranslator(translator_);
}

const TPluginGUIInfo& PluginBase::GetInfo() const {
  return info_;
}

void PluginBase::InitTranslator(const QLatin1String& prefix) {
  translator_ = new QTranslator(this);
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (translator_->load(prefix + locale)) {
      QApplication::installTranslator(translator_);
      break;
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    }
  }
}