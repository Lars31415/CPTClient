#include "ipdialog.h"
#include "ui_ipdialog.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QRegularExpressionValidator>

class MyIPValidator : public QRegularExpressionValidator
{
    // https://www.qtcentre.org/threads/6228-Ip-Address-Validation
public:
    MyIPValidator(const QString &defaultString, QObject *parent = Q_NULLPTR)
        : QRegularExpressionValidator(QRegularExpression(
                                          "^0*(2(5[0-5]|[0-4]\\d)|1?\\d{1,2})(\\.0*(2(5[0-"
                                          "5]|[0-4]\\d)|1?\\d{1,2})){3}$"),
                                      parent)
        , m_defaultString(defaultString)
    {}

    void fixup(QString &t) const override { t = m_defaultString; }

protected:
    const QString m_defaultString;
};

IpDialog::IpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IpDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText("not an ip");
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, &IpDialog::finished);
}

bool IpDialog::hasAcceptableInput() const
{
    return ui->lineEdit->hasAcceptableInput();
}

QString IpDialog::ip() const { return ui->lineEdit->text(); }
void IpDialog::setIp(QString ip)
{
    ui->lineEdit->setText(ip);
}

IpDialog::~IpDialog() { delete ui; }

void IpDialog::finished() { accept(); }

void IpDialog::accept()
{
    qDebug() << __FUNCTION__;
    MyIPValidator v("127.0.0.1");
    ui->lineEdit->setValidator(&v);
    if (!ui->lineEdit->hasAcceptableInput()) {
        QMessageBox::StandardButton reply;
        QString question = "\"" + ui->lineEdit->text()
                           + "\" does not look like an IP-adress of the form "
                             "\"xxx.xxx.xxx.xxx\".\nUse it anyway?";
        reply = QMessageBox::question(this, "Test", question, QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            QDialog::accept();
        } else {
            qDebug() << "Yes was *not* clicked";
        }
    } else {
        QDialog::accept();
    }
}
