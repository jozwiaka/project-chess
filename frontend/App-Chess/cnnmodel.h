#ifndef CNNMODEL_H
#define CNNMODEL_H

#include <QObject>
#include <QDebug>
#include "chesssquare.h"
#include <tuple>

class CNNModel : public QObject
{
    Q_OBJECT

public:
    explicit CNNModel(QObject *parent = nullptr);

    static std::tuple<ChessSquare::SquarePosition,ChessSquare::SquarePosition> GenerateMove(const QString& data);

signals:

private:
};

#endif // CNNMODEL_H
