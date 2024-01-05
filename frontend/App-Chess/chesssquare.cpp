#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const QString &position, QWidget *parent)
    : QLabel(parent), squarePosition(position)
{
    setFixedSize(80, 80);
    setMargin(0);
    setScaledContents(true); // Ensure the square image fits the label size
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);

    normalColor = dark ? QColor(101, 67, 33) : QColor(193, 154, 107);
    highlightedColor = dark ? QColor(255, 220, 185) : QColor(255, 236, 210);
    isHighlighted = false;
}

void ChessSquare::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(squarePosition);
}

void ChessSquare::highlightSquare()
{
    setStyleSheet(QString("background-color: %1;").arg(highlightedColor.name()));
}

void ChessSquare::resetSquareColor()
{
    setStyleSheet(QString("background-color: %1;").arg(normalColor.name()));
}

void ChessSquare::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event); // Call the base class paint event

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QBrush brush(isHighlighted ? highlightedColor : normalColor);
    painter.setBrush(brush);
    painter.drawRect(rect());
}


void ChessSquare::setChessPiece(std::unique_ptr<ChessPiece> piece)
{
    chessPiece = std::move(piece);
    if (chessPiece)
        chessPiece->setParent(this);
}

ChessPiece *ChessSquare::getChessPiece() const
{
    return chessPiece.get();
}
