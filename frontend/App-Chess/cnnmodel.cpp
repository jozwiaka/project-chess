#include "cnnmodel.h"
#include <QProcess>
#include <QDebug>
#include <QString>

CNNModel::CNNModel(QObject *parent)
    : QObject{parent}
{
}

ChessSquare::SquarePosition CNNModel::Run()
{
    QProcess process;
    QString pythonScriptPath = "../../scripts/cnn_model.py";
    QStringList arguments;
    process.start("python", QStringList() << pythonScriptPath << arguments);
    process.waitForFinished(-1);
    QByteArray output = process.readAllStandardOutput();

    ChessSquare::SquarePosition position{output[1]-'1',output[0]-'A'};

    qDebug() << position.x<<position.y;

    return position;
}
