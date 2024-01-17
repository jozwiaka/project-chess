#include "cnnmodel.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <tuple>

CNNModel::CNNModel(QObject *parent)
    : QObject{parent}
{
}

std::tuple<ChessSquare::SquarePosition, ChessSquare::SquarePosition> CNNModel::Run(const QString &data)
{
    QStringList args;
    args.append(data);
    return CNNModel::GenerateMove(args);
}

std::tuple<ChessSquare::SquarePosition, ChessSquare::SquarePosition> CNNModel::GenerateMove(const QStringList &args)
{
    CNNModel::RunPythonScript("../../backend/scripts/move.py", args);
    QFile file("../../backend/out/move.out");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open the file.";
    }
    QTextStream in(&file);
    QString fileContents = in.readAll();
    file.close();

    auto positionFrom = ChessSquare::SquarePosition::StrToPosition(fileContents.mid(0, 2));
    auto positionTo = ChessSquare::SquarePosition::StrToPosition(fileContents.mid(2, 2));
    return std::make_tuple(positionFrom, positionTo);
}

QByteArray CNNModel::RunPythonScript(const QString &path, const QStringList &args)
{
    QProcess process;
    process.start("python", QStringList() << path << args);
    process.waitForFinished(-1);
    auto output = process.readAllStandardOutput();
    return output;
}
