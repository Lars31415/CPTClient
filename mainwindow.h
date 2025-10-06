#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "click_label.h"
#include "records/cpttest.h"
#include "temperaturegradient.h"
#include "testgradient.h"

#include <QFile>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QRadialGradient>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void socket_connected();
    void socket_disconnected();
    void socket_host_found();
    void socket_error(QAbstractSocket::SocketError error);
    void socket_read();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    // QString ini_file;
    QFile in;
    QPainter painter;
    QPainter robertson_painter;
    QPainter depth_painter;
    QImage bgImage;
    QImage image;
    QImage robertson;
    QImage* robertson_bar;
    QImage* depth_bar;
    float w;
    float a;
    float last_pos_w;
    float last_pos_a;
    size_t last_count;
    int changed;

    CPTTest cpt_test;
    QTcpSocket socket;

    ClickLabel label_con;
    QLabel label_fn;
    QLabel label_no;
    QLabel label_d;
    QLabel label_qt;
    QLabel label_ft;
    QLabel label_rft;

    QRadialGradient grad;

    QHostAddress host_addr;

    TemperatureGradient tgradient;
    TestGradient tstgradient;

    int palet;

    void plot(QPainter &painter, int x, int y, int r, int a);

    void draw_data();
    void draw_data2();

    void timer1();
    void timer2();
    void slider1(int position);
    void slider_a(int position);
    void label_click_right();
    void label_click_left();
    void label_tap_and_hold();
    void label_tap();
    void graph_click();
    void graph_tap_and_hold();
    void reset(bool);
    void save_settings();
    void load_settings();
};

#endif // MAINWINDOW_H
