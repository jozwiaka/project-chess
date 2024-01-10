#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(const Chessboard::ChessboardType &board, bool reversedBoardView, bool *computerTurn, bool *promotionProcedure, QWidget *parent)
    : QMainWindow(parent), m_Ui(new Ui::ChessView()), m_ComputerTurn(computerTurn), m_PromotionProcedure(promotionProcedure)
{
    m_Ui->setupUi(this);
    CreateChessboardGraphics(board, reversedBoardView);
}

ChessView::~ChessView()
{
    delete m_Ui;
    delete m_ChessboardLayout;
    delete m_MenuLayout;
}

void ChessView::CreateChessboardGraphics(const Chessboard::ChessboardType &board, bool reversedBoardView)
{
    m_ChessboardLayout = new QGridLayout(m_Ui->centralwidget);
    m_ChessboardLayout->setSpacing(0);
    m_ChessboardLayout->setAlignment(Qt::AlignCenter);
    m_ChessboardLayout->setContentsMargins(0, 0, 0, 0);

    for (char row = 0; row < 8; ++row)
    {
        for (char col = 0; col < 8; ++col)
        {
            m_ChessboardLayout->addWidget(board[row][col], reversedBoardView ? row : 8 - 1 - row, reversedBoardView ? 8 - 1 - col : col);
            connect(board[row][col], &ChessSquare::Clicked, this, &ChessView::OnSquareClicked);
        }
    }
}

void ChessView::OnSquareClicked(const ChessSquare::SquarePosition &position)
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
            ChessSquare *square = qobject_cast<ChessSquare *>(m_ChessboardLayout->itemAtPosition(row, col)->widget());

            switch (square->Status)
            {
            case ChessSquare::SquareStatus::Normal:
                square->SetNormalBackgroundColor();
                break;
            case ChessSquare::SquareStatus::Active:
            case ChessSquare::SquareStatus::PreviousMove:
                square->SetHighlightBackgroundColor();
                break;
            case ChessSquare::SquareStatus::ValidMove:
            case ChessSquare::SquareStatus::ValidCapture:
                square->SetValidBackgroundColor();
                break;
            case ChessSquare::SquareStatus::Check:
                square->SetCheckBackgroundColor();
            default:
                break;
            }

            ChessPiece *piece = square->GetPiece();
            if (piece)
            {
                piece->show();
            }
        }
    }
}
