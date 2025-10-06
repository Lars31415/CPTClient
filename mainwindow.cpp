#include "mainwindow.h"

#include "./ui_ipdialog.h"
#include "./ui_mainwindow.h"
#include "RainbowGradient.hpp"
#include "gradientdialog.h"

#include <QAbstractSocket>
#include <QLayout>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include "ipdialog.h"
#include <cmath>
#include <iostream>

#ifdef __ANDROID__
#include "Toast.h"
#include <QCoreApplication>
#include <QJniObject>
#endif

float normdist(float x, float s)
{
    float r = std::exp(-x * x / 2 / s * s) / std::sqrt(2 * M_PI);
    return r;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , changed(0)
    , host_addr("127.0.0.1")
{
    ui->setupUi(this);

    qDebug() << QT_VERSION_STR;
    qDebug() << __cplusplus;

    // QString path;
    // path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    // ini_file = path + "/geo/cpt_test.ini";
    load_settings();
    qDebug() << palet;

#ifndef __ANDROID__
    QAction *exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, &QMainWindow::close);
    QMenu *fileMenu = ui->menuBar->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);
#else
    qDebug() << "__ANDROID__";
    ui->menuBar->hide();
#endif // __ANDROID__

    connect(ui->pushButtonReset, &QPushButton::clicked, this, &MainWindow::reset);

    grad.setCenter(QPointF(0, 0));
    grad.setCenterRadius(50);
    grad.setColorAt(0, QColor(255, 255, 255, a));
    grad.setColorAt(1, QColor(255, 255, 255, 0));

    label_con.setPixmap(QPixmap(":/uncon.png"));
    label_con.setFixedSize(1.7 * ui->menuBar->rect().height(), ui->menuBar->rect().height());
    label_con.setScaledContents(true);
    label_con.setToolTip(host_addr.toString());
    ui->statusBar->addWidget(&label_con, 1);
    ui->statusBar->addWidget(&label_fn, 1);
    ui->statusBar->addWidget(&label_no, 1);
    ui->statusBar->addWidget(&label_d, 1);
    ui->statusBar->addWidget(&label_qt, 1);
    ui->statusBar->addWidget(&label_ft, 1);
    ui->statusBar->addWidget(&label_rft, 1);
    QLayout *l = ui->statusBar->layout();
    l->setSpacing(16);
    ui->statusBar->setLayout(l);

#ifdef __ANDROID__
    // ui->label->grabGesture(Qt::TapAndHoldGesture);
    // ui->label->grabGesture(Qt::TapGesture);
    // connect(ui->label, &click_label::tapandhold_finished, this,
    //         &MainWindow::label_tap_and_hold);
    // connect(ui->label, &click_label::tap_finished, this,
    //         &MainWindow::label_tap);
    label_con.grabGesture(Qt::TapAndHoldGesture);
    label_con.grabGesture(Qt::TapGesture);
    ui->label->grabGesture(Qt::TapAndHoldGesture);
    ui->label->grabGesture(Qt::TapGesture);
    connect(&label_con, &::ClickLabel::tapandhold_finished, this, &MainWindow::label_tap_and_hold);
    connect(&label_con, &::ClickLabel::tap_finished, this, &MainWindow::label_tap);
    connect(ui->label, &::ClickLabel::tapandhold_finished, this, &MainWindow::graph_tap_and_hold);
#else
    connect(&label_con, &ClickLabel::click_right, this, &MainWindow::label_click_right);
    connect(&label_con, &ClickLabel::click_left, this, &MainWindow::label_click_left);
    connect(ui->label, &ClickLabel::click_right, this, &MainWindow::graph_click);
#endif

    connect(ui->horizontalSlider_w, &QSlider::sliderMoved, this, &MainWindow::slider1);
    connect(ui->horizontalSlider_a, &QSlider::sliderMoved, this, &MainWindow::slider_a);

    bgImage.load(":/charts/Robertson.png");
    QPixmap pm;
    pm.convertFromImage(bgImage);
    ui->label->setPixmap(pm);
    robertson.load(":/charts/RobertsonChart3.png");

    std::cout << normdist(0, 1) << std::endl;

    // cpt_test.load_file("..\\..\\cpt\\WTG_CPT039_2023-07-08_0821.CPT");

    connect(&socket, &QTcpSocket::connected, this, &MainWindow::socket_connected);
    connect(&socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);
    connect(&socket, &QTcpSocket::hostFound, this, &MainWindow::socket_host_found);
    connect(&socket, &QAbstractSocket::errorOccurred, this, &MainWindow::socket_error);
    connect(&socket, &QTcpSocket::readyRead, this, &MainWindow::socket_read);
    cpt_test.clear();
    last_count = 0;

    robertson_bar = new QImage(ui->label_2->rect().width(), bgImage.height(),
                               QImage::Format_RGB32);
    robertson_bar->fill(QColor(255,255,255));
    depth_bar = new QImage(ui->label_3->rect().width(), bgImage.height(),
                               QImage::Format_RGB32);
    depth_bar->fill(QColor(255,255,255));

    last_pos_w = ui->horizontalSlider_w->value();
    last_pos_a = ui->horizontalSlider_a->value();
    QTimer::singleShot(100, this, &MainWindow::timer1);
    QTimer::singleShot(1000, this, &MainWindow::timer2);
}

MainWindow::~MainWindow()
{
    delete ui;
    socket.disconnectFromHost();
}

void MainWindow::draw_data() { draw_data2(); }

void MainWindow::draw_data2()
{
    // ui->label_2->rect().setHeight(ui->verticalSlider->rect().height());
    qDebug() << ui->label_2->rect() << " " << ui->verticalSlider->rect();
    image = bgImage;
    image.fill(Qt::black);
    grad.setColorAt(0, QColor(255, 255, 255, 5 * a));
    grad.setColorAt(1, QColor(255, 255, 255, 0));

    robertson_painter.begin(robertson_bar);
    depth_painter.begin(depth_bar);
    depth_painter.setBrush(Qt::white);
    depth_painter.setPen(Qt::black);
    if (painter.begin(&image)) {
        QPen p = painter.pen();
        QBrush b = painter.brush();
        p.setWidth(0);
        b.setStyle(Qt::SolidPattern);
        w = ui->horizontalSlider_w->value();
        a = 32 * ui->horizontalSlider_a->value() / 100.0;
        float xf = image.width() / 10.0;
        float yf = image.height();
        float xfr = robertson.width() / 10.0;
        float yfr = robertson.height();
        ScanArray::iterator itr = cpt_test.data().begin();
        ScanArray::iterator last_itr = cpt_test.data().begin();
        int last_sl = 0;
        float x = 0;
        float y = 0;
        float xr = 0;
        float yr = 0;
        float lx = 0;
        float ly = 0;
        if (cpt_test.data().size() == 0) {
            robertson_bar->fill(QColor(255, 255, 255));
            depth_bar->fill(QColor(255, 255, 255));
        }
        for (; (itr != cpt_test.data().end()) && (changed == 0); ++itr) {
            float lqc = -9999;
            if ((itr->qt > 0) && (itr->rft > 0)) {
                lqc = std::log10(itr->qt);
                x = xf * itr->rft;
                y = yf * (1 - (lqc + 1) / 3);
                xr = xfr * itr->rft;
                yr = yfr * (1 - (lqc + 1) / 3);
                QColor rc = robertson.pixelColor(xr, yr);
                double ry1 = ui->label_2->rect().height() *
                             ui->verticalSlider->value() /
                             ui->verticalSlider->maximum();
                double ry2 = ui->label_2->rect().height() * last_sl /
                             ui->verticalSlider->maximum();
                robertson_painter.setBrush(rc);
                robertson_painter.setPen(rc);
                last_itr = itr;
                last_sl = ui->verticalSlider->value();
                plot(painter, x, y, w, a);
                robertson_painter.drawRect(0, ry2, ui->label_2->width(),
                                           std::max(ry1 - ry2, 1.0));
                depth_painter.drawRect(-1, 0, ui->label_2->width() + 1, ry2);
                depth_painter.drawText(
                    0, ry2 - 2,
                    " " + QString::number(ui->verticalSlider->value() / 100.0, 'f',
                                                      2));
                // depth_painter.drawLine(0, ry2, ui->label_3->width(), ry2);
                // qDebug() << ry1 << " " << ry2;
            }
            ui->verticalSlider->setValue(100 * itr->d);
        }
        painter.end();
        robertson_painter.end();
        depth_painter.end();

        QPixmap pmr;
        pmr.convertFromImage(*robertson_bar);
        ui->label_2->setPixmap(pmr);
        QPixmap pmr2;
        pmr2.convertFromImage(*depth_bar);
        ui->label_3->setPixmap(pmr2);

        QImage img = bgImage;
        for (int i = 0; (i < image.width()) && (changed == 0); ++i) {
            for (int j = 0; (j < image.height()) && (changed == 0); ++j) {
                QColor c = image.pixelColor(i, j);
                if (c.value() != 0) {
                    switch (palet) {
                    case 1:
                        c = tgradient.getColor(255 - c.red());
                        break;
                    case 2:
                        c = coldWarm(c.red() / 255.0);
                        break;
                    case 3:
                        c = tstgradient.getColor(c.red());
                        break;
                    default:
                        c = gradientColor(c.red() / 255.0);
                    }
                    img.setPixelColor(i, j, c);
                } else {
                    c = Qt::black;
                }
            }
        }
        if (painter.begin(&img)) {
            p.setWidth(3);
            p.setColor(QColor(0, 0, 0, 255));
            b.setStyle(Qt::NoBrush);
            painter.setBrush(b);
            painter.setPen(p);
            painter.drawEllipse(x - w / 2, y - w / 2, w, w);
            p.setWidth(2);
            painter.setBrush(b);
            painter.setPen(p);
            painter.drawEllipse(x - 1, y - 1, 2, 2);
            painter.end();
            if (last_itr != cpt_test.data().end()) {
                QString l = QString::number(last_itr->no);
                label_no.setText("no: " + l);
                l = QString::number(last_itr->d, 'f', 2) + "m     /    " + QString::number(cpt_test.max_depth(), 'f', 2) + "m";
                label_d.setText("d: " + l);
                l = QString::number(last_itr->qt, 'f', 4);
                label_qt.setText("qt: " + l + "MPa");
                l = QString::number(last_itr->ftm, 'f', 4);
                label_ft.setText("ft: " + l + "MPa");
                l = QString::number(last_itr->rft, 'f', 2);
                label_rft.setText("rft: " + l + "%");
            }
        }
        QPixmap pm;
        // pm.convertFromImage(image);
        pm.convertFromImage(img);
        ui->label->setPixmap(pm);
        int m = std::ceil(100*cpt_test.max_depth());
        if (m != ui->verticalSlider->maximum())
            ui->verticalSlider->setMaximum(m);
    }
}

void MainWindow::timer1()
{
    if ((last_pos_w != ui->horizontalSlider_w->value())
        || (last_pos_a != ui->horizontalSlider_a->value())) {
        save_settings();
        ++changed;
    }
    if (last_count != cpt_test.data().size())
        ++changed;

    if (changed) {
        changed = 0;
        label_fn.setText(cpt_test.cpt_name());
        draw_data();
        last_pos_w = ui->horizontalSlider_w->value();
        last_pos_a = ui->horizontalSlider_a->value();
        last_count = cpt_test.data().size();
    }

    QTimer::singleShot(100, this, &MainWindow::timer1);
}

void MainWindow::timer2()
{
    QAbstractSocket::SocketState stat = socket.state();
    if (stat == QAbstractSocket::UnconnectedState) {
        socket.connectToHost(host_addr, 1024);
        label_con.setToolTip(host_addr.toString() + " connecting");
        qDebug() << "Connecting " << host_addr.toString() << " " << 1024;
        socket.open(QIODeviceBase::ReadOnly);
        qDebug() << socket.waitForConnected(5000);
        stat = socket.state();
        qDebug() << stat;
        qDebug() << "Done connecting";
    }
    QTimer::singleShot(5000, this, &MainWindow::timer2);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->centralwidget->setGeometry(rect());
}

void MainWindow::slider1(int position) { ++changed; }

void MainWindow::slider_a(int position)
{
    ++changed;
}

void MainWindow::socket_connected()
{
    qDebug() << "Connected";
    label_con.setPixmap(QPixmap(":/connected.png"));
    label_con.setToolTip(host_addr.toString() + " connected");
    label_con.update();
}

void MainWindow::socket_disconnected()
{
    qDebug() << "Disconnected";
    label_con.setPixmap(QPixmap(":/uncon.png"));
    label_con.setToolTip(host_addr.toString() + " disconnected");
    label_con.update();
}

void MainWindow::socket_host_found() { qDebug() << "Host found"; }

void MainWindow::socket_error(QAbstractSocket::SocketError error)
{
    qDebug() << "Error";
    label_con.setToolTip(host_addr.toString() + " error");
}

void MainWindow::socket_read()
{
    while (socket.canReadLine()) {
        QByteArray d = socket.readLine();
        // qDebug() << d;
        cpt_test.add_line(d);
    }
    ++changed;
}

void MainWindow::plot(QPainter &painter, int x, int y, int r, int a)
{
    grad.setCenter(QPointF(x, y));
    grad.setFocalPoint(QPointF(x, y));
    grad.setCenterRadius(r / 2);
    QBrush brush(grad);
    painter.setBrush(brush);
    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter.setPen(pen);
    painter.drawEllipse(x - r / 2, y - r / 2, r, r);
}

void MainWindow::label_click_right() {
#ifndef __ANDROID__
    qDebug() << "click right";
    IpDialog d;
    qDebug() << __FUNCTION__ << " " << __LINE__;
    d.setIp(host_addr.toString());
    qDebug() << __FUNCTION__ << " " << __LINE__;
    if (d.exec()) {
        host_addr = QHostAddress(d.ip());
        save_settings();
        // QSettings mySettings("Geo", "cpt_test");
        // mySettings.setValue("host", host_addr.toString());
        // qDebug() << "Saved: " << mySettings.value("host").toString();
        socket.close();
    }
#endif
}

void MainWindow::label_click_left()
{
#ifndef __ANDROID__
    qDebug() << "click left";
    // qDebug() << "Show toast";
    // Toast* toast = new Toast(this);
    // toast->setDuration(5000);  // Hide after 2 seconds
    // toast->setIcon(ToastIcon::INFORMATION);
    // toast->setTitle("Host");
    // toast->setText(host_addr.toString());
    // toast->setPosition(ToastPosition::CENTER);
    // toast->applyPreset(ToastPreset::INFORMATION);  // Apply style preset
    // toast->show();
#endif
}

void MainWindow::label_tap_and_hold()
{
#ifdef __ANDROID__
    qDebug() << "tap and hold";
    IpDialog d;
    d.setIp(host_addr.toString());
    if (d.exec()) {
        host_addr = QHostAddress(d.ip());
        save_settings();
        // QSettings mySettings(ini_file, QSettings::NativeFormat);
        // mySettings.setValue("host", host_addr.toString());
        // mySettings.sync();
        socket.close();
    }
#endif
}

void MainWindow::label_tap()
{
#ifdef __ANDROID__
    qDebug() << "tap";
    qDebug() << "Show toast";
    Toast *toast = new Toast(this);
    toast->setDuration(5000); // Hide after 2 seconds
    toast->setIcon(ToastIcon::INFORMATION);
    toast->setTitle("Host");
    if (socket.state() == QAbstractSocket::ConnectedState)
        toast->setText(host_addr.toString() + " connected");
    else
        toast->setText(host_addr.toString() + " not connected");
    toast->setPosition(ToastPosition::BOTTOM_LEFT);
    toast->applyPreset(ToastPreset::INFORMATION); // Apply style preset
    toast->show();
#endif
}

void MainWindow::graph_click()
{
    GradientDialog form;
    form.setPalet(palet);
    if (form.exec()) {
        palet = form.palet();
        ++changed;
        save_settings();
    }
}

void MainWindow::graph_tap_and_hold()
{
#ifdef __ANDROID__
    qDebug() << "graph tap and hold";
    GradientDialog form;
    form.setPalet(palet);
    if (form.exec()) {
        palet = form.palet();
        ++changed;
        save_settings();
    }
#endif
}

void MainWindow::reset(bool)
{
    ui->horizontalSlider_w->setSliderPosition(16);
    ui->horizontalSlider_a->setSliderPosition(14);
    palet = 0;
    ++changed;
    save_settings();
}

void MainWindow::save_settings()
{
    // QSettings mySettings(ini_file, QSettings::NativeFormat);
    QSettings mySettings(QSettings::NativeFormat, QSettings::UserScope, "Geo", "cpt_test");
    if (mySettings.isWritable()) {
        mySettings.setValue("host", host_addr.toString());
        mySettings.setValue("palet", palet);
        mySettings.setValue("markerWidth", ui->horizontalSlider_w->sliderPosition());
        mySettings.setValue("markerAlpha", ui->horizontalSlider_a->sliderPosition());
        mySettings.sync();
        qDebug() << "Saved: " << host_addr.toString();
        qDebug() << "Saved: " << palet;
        qDebug() << "Saved: " << ui->horizontalSlider_w->sliderPosition();
        qDebug() << "Saved: " << ui->horizontalSlider_a->sliderPosition();
    } else
        qDebug() << "not writable";
}

void MainWindow::load_settings()
{
    // QSettings mySettings(ini_file, QSettings::NativeFormat);
    QSettings mySettings(QSettings::NativeFormat, QSettings::UserScope, "Geo", "cpt_test");
    QString h = mySettings.value("host", "127.0.0.1").toString();
    host_addr = QHostAddress(h);
    palet = mySettings.value("palet", 0).toInt();
    ui->horizontalSlider_w->setSliderPosition(mySettings.value("markerWidth", 16).toInt());
    ui->horizontalSlider_a->setSliderPosition(mySettings.value("markerAlpha", 14).toInt());
    qDebug() << "loaded: " << host_addr.toString();
    qDebug() << "Loaded: " << palet;
    qDebug() << "Loaded: " << ui->horizontalSlider_w->sliderPosition();
    qDebug() << "Loaded: " << ui->horizontalSlider_a->sliderPosition();
    ++changed;
}
