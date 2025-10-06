#include "click_label.h"

ClickLabel::ClickLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent)
{}

ClickLabel::~ClickLabel() {}

bool ClickLabel::gestureEvent(QGestureEvent *event)
{
    // qDebug() << __FUNCTION__ << ": " << event;
    if (QGesture *tah = event->gesture(Qt::TapAndHoldGesture))
        tapandhold(static_cast<QTapAndHoldGesture *>(tah));
    if (QGesture *ta = event->gesture(Qt::TapGesture))
        tap(static_cast<QTapGesture *>(ta));
    return true;
}

void ClickLabel::tapandhold(QTapAndHoldGesture *g)
{
    if (g != nullptr) {
        // qDebug() << __FUNCTION__ << ": " << g;
    } else {
        // qDebug() << __FUNCTION__ << ": nullptr!";
        return;
    }
    if (g->state() == Qt::GestureStarted) {
        qDebug() << "Started";
        emit tapandhold_started();
    }
    if (g->state() == Qt::GestureFinished) {
        qDebug() << "Finished";
        emit tapandhold_finished();
    }
}

void ClickLabel::tap(QTapGesture *g)
{
    if (g != nullptr) {
        // qDebug() << __FUNCTION__ << ": " << g;
    } else {
        // qDebug() << __FUNCTION__ << ": nullptr!";
        return;
    }
    if (g->state() == Qt::GestureStarted) {
        qDebug() << "Started";
        emit tap_started();
    }
    if (g->state() == Qt::GestureFinished) {
        qDebug() << "Finished";
        emit tap_finished();
    }
}

bool ClickLabel::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent *>(event));
    return QWidget::event(event);
}

void ClickLabel::mousePressEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::RightButton)
    // qDebug() << event->button();
     // qDebug() << event;
    if (event->button() == Qt::RightButton)
        emit click_right();
    if (event->button() == Qt::LeftButton)
        emit click_left();
}
