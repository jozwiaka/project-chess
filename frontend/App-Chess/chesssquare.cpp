#include "chesssquare.h"
#include <QMouseEvent>

ChessSquare::ChessSquare(const QString &imagePath, const QString &position, QWidget *parent)
    : QLabel(parent), squarePosition(position)
{
    setFixedSize(80,80);
    setMargin(0);
    setPixmap(QPixmap(imagePath).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setScaledContents(true); // Ensure the square image fits the label size
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);

    originalColor = palette().color(QPalette::Window);
}

void ChessSquare::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    // Example: Highlight the square with a yellow color
    highlightSquare(Qt::yellow);

    // Additional logic for handling the chess piece click
    emit clicked(squarePosition);
}

void ChessSquare::highlightSquare(const QColor &color)
{
    // Set the square color to the specified color
    setStyleSheet(QString("background-color: %1;").arg(color.name()));
}

void ChessSquare::resetSquareColor()
{
    // Reset the square color to the original color
    setStyleSheet(QString("background-color: %1;").arg(originalColor.name()));
}

void ChessSquare::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);

    // Set the composition mode to use the source over the destination
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // Set the brush color to the original color
    painter.setBrush(originalColor);

    // Draw a filled rectangle with the original color over the entire QLabel
    painter.drawRect(rect());
}


void ChessSquare::setChessPiece(ChessPiece *piece)
{
    chessPiece = piece;
    if (chessPiece)
        chessPiece->setParent(this);
}

ChessPiece *ChessSquare::getChessPiece() const
{
    return chessPiece;
}
