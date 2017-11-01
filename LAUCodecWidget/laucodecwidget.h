#ifndef LAUCODECWIDGET_H
#define LAUCODECWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QDir>
#include <QMessageBox>
#include <QDialog>
#include <QSettings>
#include <QPushButton>
#include <QStringList>
#include <QListWidget>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QResizeEvent>
#include <QStandardPaths>
#include <QDialogButtonBox>
#include <tiffio.h>

class LAUCodecWidget : public QWidget
{
    Q_OBJECT

public:
    LAUCodecWidget(QStringList strings = QStringList(), QWidget *parent = 0);
    ~LAUCodecWidget();

    bool isValid() const {
        return(files.isEmpty() == false);
    }

    bool isNull() const {
        return(files.isEmpty());
    }

    bool save(QString filename = QString());
    bool load(QString filename = QString());

private:
    QStringList files;
};

class LAUCodecDialog : public QDialog
{
    Q_OBJECT

public:
    LAUCodecDialog(QStringList strings = QStringList(), QWidget *parent = 0) : QDialog(parent)
    {
        qDebug() << "HERE";

        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6, 6, 6, 6);

        widget = new LAUCodecWidget(strings);
        this->layout()->addWidget(widget);

        QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        QPushButton *button = new QPushButton(QString("Expand"));
        box->addButton(button, QDialogButtonBox::ActionRole);

        connect(button, SIGNAL(clicked()), this, SLOT(onExpand()));
        connect(box->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(box->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
        this->layout()->addWidget(box);
    }

    ~LAUCodecDialog()
    {
        ;
    }

    bool isValid() const {
        return(widget->isValid());
    }

    bool isNull() const {
        return(widget->isNull());
    }

protected:
    void accept()
    {
        if (widget->save(QString())){
            ;
        }
    }

    void reject()
    {
        QDialog::reject();
    }

public slots:
    void onExpand()
    {
        if (widget->load(QString())){
            ;
        }
    }

private:
    LAUCodecWidget *widget;

signals:
    void emitInt(int);
};

#endif // LAUCODECWIDGET_H
