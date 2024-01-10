#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(bool *computerTurn, bool *promotionProcedure, QWidget *parent)
    : QMainWindow(parent), m_Ui(new Ui::ChessView()), m_ComputerTurn(computerTurn), m_PromotionProcedure(promotionProcedure)
{
    m_Ui->setupUi(this);
}

ChessView::~ChessView()
{
    delete m_Ui;
    delete ChessboardLayout;
    delete m_MenuLayout;
}

// void ChessView::CreateMenuGraphics(QVector<QVector<ChessSquare *>> chessboard)
// {

// }

void ChessView::CreateChessboardGraphics(QVector<QVector<ChessSquare *>> chessboard)
{
    ChessboardLayout = new QGridLayout(m_Ui->centralwidget);
    ChessboardLayout->setSpacing(0);
    ChessboardLayout->setAlignment(Qt::AlignCenter);
    ChessboardLayout->setContentsMargins(0, 0, 0, 0);

    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
            ChessboardLayout->addWidget(chessboard[row][col], *m_ComputerTurn ? row : 8 - 1 - row, *m_ComputerTurn ? 8 - 1 - col : col);
            connect(chessboard[row][col], &ChessSquare::Clicked, this, &ChessView::OnSquareClicked);
        }
    }
}

void ChessView::OnSquareClicked(const ChessSquare::Position &position)
{
    // if (!*m_ComputerTurn) //TODO
    {
        emit SquareClicked(position);
    }
}

void ChessView::UpdateChessboardGraphics()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ChessSquare *square = qobject_cast<ChessSquare *>(ChessboardLayout->itemAtPosition(row, col)->widget());

            switch (square->GetStatus())
            {
            case ChessSquare::Status::Normal:
                square->SetNormalBackgroundColor();
                break;
            case ChessSquare::Status::Active:
            case ChessSquare::Status::PreviousMove:
                square->SetHighlightBackgroundColor();
                break;
            case ChessSquare::Status::ValidMove:
            case ChessSquare::Status::ValidCapture:
                square->SetValidBackgroundColor();
                break;
            case ChessSquare::Status::Check:
                square->SetCheckBackgroundColor();
            default:
                break;
            }

            ChessPiece *piece = square->GetChessPiece();
            if (piece)
            {
                piece->show();
            }
        }
    }
}
