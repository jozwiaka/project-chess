#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QLabel>
#include "chesspiece.h"
#include <QColor>
#include <memory>

class ChessSquare : public QLabel
{
    Q_OBJECT

public:
    ChessSquare(bool dark, const QString &position, QWidget *parent = nullptr);

    void setChessPiece(std::unique_ptr<ChessPiece> piece);
    ChessPiece *getChessPiece() const;

    void highlightSquare();
    void resetSquareColor();

signals:
    void clicked(const QString &position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString squarePosition;
    QColor normalColor;
    QColor highlightedColor;
    bool isHighlighted;
    std::unique_ptr<ChessPiece> chessPiece;
};

#endif // CHESSSQUARE_H
