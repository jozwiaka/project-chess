#ifndef CNNMODEL_H
#define CNNMODEL_H

#include <QObject>
#include <QObject>
#include <QString>
#include <QImage>
#include <QVector>
#include <QProcess>
#include <QTextStream>

// #include <opencv2/opencv.hpp> // Assuming you use OpenCV for image processing

class CNNModel : public QObject
{
    Q_OBJECT
public:
    explicit CNNModel(QObject *parent = nullptr);

    void move()
    {
        QString pyScriptPath = "../../scripts/cnn_model.py";

        QProcess process;

        QTextStream(stdout)<<"Starting...\n";
        process.start("python", QStringList()<<pyScriptPath);

        process.waitForFinished(-1);

        QString result = process.readAllStandardOutput();

        QTextStream(stdout)<<result;

    }

signals:
};

#endif // CNNMODEL_H
