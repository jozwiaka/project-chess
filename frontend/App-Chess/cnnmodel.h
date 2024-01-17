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

    static std::tuple<ChessSquare::SquarePosition,ChessSquare::SquarePosition> Run(const QString &data);

signals:

private:
    static std::tuple<ChessSquare::SquarePosition,ChessSquare::SquarePosition> GenerateMove(const QStringList &args);
    static QByteArray RunPythonScript(const QString &path, const QStringList &args = QStringList());
};

#endif // CNNMODEL_H
