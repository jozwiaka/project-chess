#include "config.h"
#include "cnnmodel.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <tuple>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

CNNModel::CNNModel(QObject *parent)
    : QObject{parent}
{
}

std::tuple<ChessSquare::SquarePosition, ChessSquare::SquarePosition> CNNModel::GenerateMove(const QString &data)
{
    QNetworkAccessManager manager;
    QString serverAddress = qEnvironmentVariable("DOCKER_BACKEND_SERVER_ADDRESS");
    if (serverAddress.isEmpty())
    {
        serverAddress = "http://127.0.0.1:5000";
    }

    QNetworkRequest request{QUrl(serverAddress + "/generate_move")};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["fen_data"] = data;

    QNetworkReply *reply = manager.post(request, QJsonDocument(json).toJson());
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    auto output = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(output);
    QJsonObject jsonObject = jsonResponse.object();

    auto positionFrom = ChessSquare::SquarePosition::StrToPosition(jsonObject["positionFrom"].toString());
    auto positionTo = ChessSquare::SquarePosition::StrToPosition(jsonObject["positionTo"].toString());

    return std::make_tuple(positionFrom, positionTo);
}
