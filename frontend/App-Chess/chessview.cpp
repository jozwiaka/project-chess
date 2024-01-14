#include "chessview.h"
#include "./ui_chessview.h"

ChessView::ChessView(const Chessboard &board, bool *computerTurn, QWidget *parent)
    : QMainWindow(parent), m_Ui(new Ui::ChessView()), m_ComputerTurn(computerTurn)
{
    m_Ui->setupUi(this);
    CreateChessboardGraphics(board);
}

ChessView::~ChessView()
{
    delete m_Ui;
}

void ChessView::CreateChessboardGraphics(const Chessboard &board)
{
    m_ChessboardLayout = new QGridLayout(m_Ui->centralwidget);
    m_ChessboardLayout->setSpacing(0);
    m_ChessboardLayout->setAlignment(Qt::AlignCenter);
    m_ChessboardLayout->setContentsMargins(0, 0, 0, 0);

    for (char x = 0; x < 8; ++x)
    {
        for (char y = 0; y < 8; ++y)
        {
            m_ChessboardLayout->addWidget(board[x][y], *m_ComputerTurn ? x : 8 - 1 - x, *m_ComputerTurn ? 8 - 1 - y : y);
        }
    }
}

void ChessView::OnPromotionPieceSelected(const ChessPiece::PieceType &type)
{
    emit PromotedPieceSelected(type);
}

void ChessView::CreatePromotionDialog(const ChessPiece::PieceColor &yor)
{
    m_PromotionDialog = new PromotionDialog(yor, this);
    connect(m_PromotionDialog, &PromotionDialog::PieceSelected, this, &ChessView::OnPromotionPieceSelected);
    m_PromotionDialog->exec();
}

void ChessView::CreateEndGameDialog(const QString &message)
{
    m_EndGameDialog = new EndGameDialog(message, this);
    m_EndGameDialog->exec();
}

void ChessView::OnSquareClicked(const ChessSquare::SquarePosition &position)
{
    // if (!*m_ComputerTurn) // TODO
    {
        emit SquareClicked(position);
    }
}

void ChessView::UpdateChessboardGraphics()
{
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            ChessSquare *square = qobject_cast<ChessSquare *>(m_ChessboardLayout->itemAtPosition(x, y)->widget());

            switch (square->Status)
            {
            case ChessSquare::SquareStatus::Normal:
                square->SetNormalBackgroundColor();
                break;
            case ChessSquare::SquareStatus::PreviousMove:
                square->SetHighlightBackgroundColor();
                break;
            case ChessSquare::SquareStatus::Check:
                square->SetCheckBackgroundColor();
            default:
                break;
            }

            switch (square->StatusTemporary)
            {
            case ChessSquare::SquareStatusTemporary::Active:
                square->SetHighlightBackgroundColor();
                break;
            case ChessSquare::SquareStatusTemporary::ValidMove:
            case ChessSquare::SquareStatusTemporary::ValidCapture:
                square->SetValidBackgroundColor();
                break;
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
