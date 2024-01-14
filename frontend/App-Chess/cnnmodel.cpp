#include "cnnmodel.h"
#include <QProcess>
#include <QDebug>
#include <QString>

CNNModel::CNNModel(QObject *parent)
    : QObject{parent}
{
}

ChessSquare::SquarePosition CNNModel::Run(const QString &data)
{
    QStringList args;
    args.append(data);
    return CNNModel::GenerateMove(args);
}

ChessSquare::SquarePosition CNNModel::GenerateMove(const QStringList &args)
{
    QByteArray output = CNNModel::RunPythonScript("../../backend/scripts/cnn_model.py", args);
    ChessSquare::SquarePosition position{output[1] - '1', output[0] - 'A'};
    return position;
}

QByteArray CNNModel::RunPythonScript(const QString &path, const QStringList &args)
{
    QProcess process;
    process.start("python", QStringList() << path << args);
    process.waitForFinished(-1);
    auto output = process.readAllStandardOutput();
    qDebug() << output << "\n";
    return output;
}
