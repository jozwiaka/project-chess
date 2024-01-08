#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const Position &position, QWidget *parent)
    : QLabel(parent), m_Position(position), m_Status(Status::Normal), m_Size(80), m_ChessPiece(nullptr)
{
    setFixedSize(m_Size, m_Size);
    setMargin(0);
    setScaledContents(true); // Ensure the square image fits the label size
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAlignment(Qt::AlignCenter);
    m_NormalColor = dark ? QColor(101, 67, 33) : QColor(193, 154, 107);
    m_HighlightedColor = dark ? QColor(255, 220, 185) : QColor(255, 236, 210);
    m_IsHighlighted = false;
    ResetSquareColor();
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

    int radius = 0;
    switch (m_Status)
    {
    case Status::ValidMove:
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::gray);
        radius = 10;
        painter.drawEllipse(m_Size / 2 - radius, m_Size / 2 - radius, 2 * radius, 2 * radius);
        break;

    case Status::ValidCapture:
        QPen pen(Qt::gray);
        pen.setWidth(10);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        radius = m_Size / 2;
        painter.drawEllipse(m_Size / 2 - radius, m_Size / 2 - radius, 2 * radius, 2 * radius);
        break;
    }

    update();
}

void ChessSquare::SetChessPiece(ChessPiece *piece)
{
    if(m_ChessPiece)
    {
        delete m_ChessPiece;
    }
    if (piece)
    {
        m_ChessPiece = piece;
        m_ChessPiece->setParent(this);
        m_ChessPiece->show();
    }
}

ChessPiece *ChessSquare::GetChessPiece()
{
    return m_ChessPiece;
}

void ChessSquare::SetStatus(Status status)
{
    m_Status = status;
}

ChessSquare::Status ChessSquare::GetStatus() const
{
    return m_Status;
}

ChessSquare::Position ChessSquare::GetPosition() const
{
    return m_Position;
}

bool ChessSquare::IsChessPiece() const
{
    return m_ChessPiece != nullptr;
}
