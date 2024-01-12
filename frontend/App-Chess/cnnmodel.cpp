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
    CNNModel::ReadImage();
    return CNNModel::GenerateMove();
}

QByteArray CNNModel::ReadImage() {
    return CNNModel::RunPythonScript("../../scripts/opencv.py");
}

ChessSquare::SquarePosition CNNModel::GenerateMove()
{
    QByteArray output = CNNModel::RunPythonScript("../../scripts/cnn_model.py");
    ChessSquare::SquarePosition position{output[1]-'1',output[0]-'A'};
    return position;
}

QByteArray CNNModel::RunPythonScript(const QString& path, const QStringList& args)
{
    QProcess process;
    process.start("python", QStringList() << path << args);
    process.waitForFinished(-1);
    auto output = process.readAllStandardOutput();
    qDebug()<<output<<"\n";
    return output;
}
