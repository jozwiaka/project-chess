#ifndef CNNMODEL_H
#define CNNMODEL_H

#include <QObject>

class CNNModel : public QObject
{
    Q_OBJECT
public:
    explicit CNNModel(QObject *parent = nullptr);

signals:
};

#endif // CNNMODEL_H
