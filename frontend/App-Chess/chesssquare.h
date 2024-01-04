#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    ChessSquare(const QString &imagePath, const QString &position, QWidget *parent = nullptr);

    void setChessPiece(ChessPiece *piece);
    ChessPiece *getChessPiece() const;

    void highlightSquare(const QColor &color);
    void resetSquareColor();

signals:
    void clicked(const QString &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString squarePosition;
    QColor originalColor;
    ChessPiece *chessPiece = nullptr;
};

#endif // CHESSSQUARE_H
