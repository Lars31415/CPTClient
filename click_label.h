#ifndef CLICK_LABEL_H
#define CLICK_LABEL_H

#include <QGestureEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <Qt>

class ClickLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickLabel();

signals:
    void click_left();
    void click_right();
    void tapandhold_started();
    void tapandhold_finished();
    void tap_started();
    void tap_finished();

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void tapandhold(QTapAndHoldGesture *g);
    void tap(QTapGesture *g);
};

#endif // CLICK_LABEL_H
