#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const Position& position, QWidget *parent)
    : QLabel(parent), m_Position(position), m_Status(Status::Normal)
{
    setFixedSize(80, 80);
    setMargin(0);
    setScaledContents(true); // Ensure the square image fits the label size
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);

    m_NormalColor = dark ? QColor(101, 67, 33) : QColor(193, 154, 107);
    m_HighlightedColor = dark ? QColor(255, 220, 185) : QColor(255, 236, 210);
    m_IsHighlighted = false;
}

void ChessSquare::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit Clicked(m_Position);
}

void ChessSquare::HighlightSquare()
{
    setStyleSheet(QString("background-color: %1;").arg(m_HighlightedColor.name()));
}

void ChessSquare::ResetSquareColor()
{
    setStyleSheet(QString("background-color: %1;").arg(m_NormalColor.name()));
}

void ChessSquare::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event); // Call the base class paint event

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QBrush brush(m_IsHighlighted ? m_HighlightedColor : m_NormalColor);
    painter.setBrush(brush);
    painter.drawRect(rect());
}


void ChessSquare::SetChessPiece(std::unique_ptr<ChessPiece> piece)
{
    m_ChessPiece = std::move(piece);
    if (m_ChessPiece)
        m_ChessPiece->setParent(this);
}

ChessPiece *ChessSquare::GetChessPiece() const
{
    return m_ChessPiece.get();
}

void ChessSquare::SetStatus(Status status)
{
    m_Status = status;
}

ChessSquare::Status ChessSquare::GetStatus() const {
    return m_Status;
}
