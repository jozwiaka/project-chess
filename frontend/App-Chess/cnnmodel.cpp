#include "cnnmodel.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>

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
    CNNModel::RunPythonScript("../../backend/scripts/move.py", args);
    QFile file("../../backend/out/move.out");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open the file.";
    }
    QTextStream in(&file);
    QString fileContents = in.readAll();
    file.close();
    qDebug() << "File Contents:\n" << fileContents;

    auto position = ChessSquare::SquarePosition::StrToPosition(fileContents);
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
