#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>

namespace Ui {
class IpDialog;
}

class IpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IpDialog(QWidget *parent = nullptr);
    ~IpDialog();

    bool hasAcceptableInput() const;

    QString ip() const;
    void setIp(QString ip);

protected:
    void accept() override;

private:
    Ui::IpDialog *ui;
    void finished();
};

#endif // IPDIALOG_H
