#include "chesssquare.h"
#include <QMouseEvent>
#include <QColor>

ChessSquare::ChessSquare(bool dark, const Position& position, QWidget *parent)
    : QLabel(parent), m_Position(position), m_Status(Status::Normal), m_Size(80)
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

    if(m_Status == Status::AvailableMove)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::gray);
        int radius = 10;
        painter.drawEllipse(m_Size/2 - radius, m_Size/2 - radius, 2 * radius, 2 * radius);
    }

    update();
}


void ChessSquare::SetChessPiece(std::unique_ptr<ChessPiece> piece)
{
    if(piece)
    {
        m_ChessPiece = std::move(piece);
        m_ChessPiece->setParent(this);
        m_ChessPiece->show();
    }
}

std::unique_ptr<ChessPiece> ChessSquare::GetChessPiece()
{
    return std::move(m_ChessPiece);
}

const ChessPiece& ChessSquare::GetChessPieceRef() const
{
    if(m_ChessPiece)
    {
        return *m_ChessPiece;
    }

}

void ChessSquare::SetStatus(Status status)
{
    m_Status = status;
}

ChessSquare::Status ChessSquare::GetStatus() const {
    return m_Status;
}

ChessSquare::Position ChessSquare::GetPosition() const {
    return m_Position;
}

bool ChessSquare::IsChessPiece() const {
    return m_ChessPiece != nullptr;
}
