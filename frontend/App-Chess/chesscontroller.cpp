#include "chesscontroller.h"
#include "chessboard.h"

ChessController::ChessController(ChessModel *model, ChessView *view, QObject *parent)
    : m_Model{model}, m_View{view}, QObject{parent}
{
    connect(m_View, &ChessView::SquareClicked, m_Model, &ChessModel::UpdateModelOnSquareClick);
    connect(m_Model, &ChessModel::UpdateChessboardGraphics, m_View, &ChessView::UpdateChessboardGraphics);

    connect(m_View, &ChessView::PromotedPieceSelected, m_Model, &ChessModel::OnPromotionPieceSelected);
    connect(m_Model, &ChessModel::ShowPromotionDialog, m_View, &ChessView::CreatePromotionDialog);

    connect(m_Model, &ChessModel::ShowEndGameDialog, m_View, &ChessView::CreateEndGameDialog);

    Chessboard &board = Chessboard::GetInstance();
    for (char x = 0; x < 8; ++x)
    {
        for (char y = 0; y < 8; ++y)
        {
            connect(board[x][y], &ChessSquare::Clicked, m_View, &ChessView::OnSquareClicked);
        }
    }
}
