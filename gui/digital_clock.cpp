#include <QHBoxLayout>
#include <QTime>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget* parent)
  : QWidget(parent) {
  display_ = new QLabel(this);
  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->addWidget(display_);
  setLayout(main_layout);

  timer_ = new QTimer(this);
  timer_->setInterval(500);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_->start();

  sep_visible_ = false;
  sep_flashes_ = true;
}

DigitalClock::~DigitalClock() {
  timer_->stop();
}

void DigitalClock::DrawImage(const QPixmap* image) {
  display_->setPixmap(*image);
}

void DigitalClock::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void DigitalClock::TimeoutHandler() {
  QString time = QTime::currentTime().toString("hh:mm");
  if (sep_flashes_) {
    time[2] = sep_visible_ ? ':' : ' ';
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(time);
}