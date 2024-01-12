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
private:
    static QByteArray ReadImage();
    static ChessSquare::SquarePosition GenerateMove();
    static QByteArray RunPythonScript(const QString& path, const QStringList& args = QStringList());

signals:
};

#endif // CNNMODEL_H
