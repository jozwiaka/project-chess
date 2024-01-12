#ifndef CNNMODEL_H
#define CNNMODEL_H

#include <QObject>
#include <QDebug>
#include "chesssquare.h"

class CNNModel : public QObject
{
    Q_OBJECT

public:
    explicit CNNModel(QObject *parent = nullptr);

    static ChessSquare::SquarePosition Run();

signals:
};

#endif // CNNMODEL_H
