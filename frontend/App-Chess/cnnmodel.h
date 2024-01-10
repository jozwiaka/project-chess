#ifndef CNNMODEL_H
#define CNNMODEL_H

#include <QObject>
#include <QDebug>

class CNNModel : public QObject
{
    Q_OBJECT
public:
    explicit CNNModel(QObject *parent = nullptr);

    void Log() { qDebug() << "Test"; }

signals:
};

#endif // CNNMODEL_H
