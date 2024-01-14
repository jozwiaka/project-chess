#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include "chesssquare.h"
#include "chesspiece.h"
#include <QGridLayout>
#include "chessmodel.h"
#include <QVector>
#include "promotiondialog.h"
#include "endgamedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ChessView;
}
QT_END_NAMESPACE

class ChessView : public QMainWindow
{
    Q_OBJECT

public:
    ChessView( bool *computerTurn, QWidget *parent = nullptr);
    ~ChessView();
    void CreatePromotionDialog(const ChessPiece::PieceColor &color);
    void CreateEndGameDialog(const QString& message);
    void UpdateChessboardGraphics();
    void OnSquareClicked(const ChessSquare::SquarePosition &position);

private:
    Ui::ChessView *m_Ui;
    QGridLayout *m_ChessboardLayout;
    PromotionDialog *m_PromotionDialog;
    EndGameDialog *m_EndGameDialog;
    bool *m_ComputerTurn;

private:
    void CreateChessboardGraphics();
    void OnPromotionPieceSelected(const ChessPiece::PieceType &type);

signals:
    void SquareClicked(const ChessSquare::SquarePosition &position);
    void PromotedPieceSelected(const ChessPiece::PieceType &type);
};
#endif // CHESSVIEW_H
