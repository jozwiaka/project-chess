#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include "chesssquare.h"
#include "chesspiece.h"
#include <QGridLayout>
#include "chessmodel.h"
#include <QVector>
#include "promotiondialog.h"

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
    ChessView(const Chessboard::ChessboardType &board, bool *computerTurn, bool *promotionProcedure, QWidget *parent = nullptr);
    ~ChessView();
    void UpdateChessboardGraphics();

private:
    Ui::ChessView *m_Ui;
    QGridLayout *m_ChessboardLayout;
    QGridLayout *m_MenuLayout;
    PromotionDialog *m_PromotionDialog; // Assuming PromotionDialog is the class for your promotion window

    bool *m_ComputerTurn;
    bool *m_PromotionProcedure;

private:
    void CreateChessboardGraphics(const Chessboard::ChessboardType &board);
    void OnSquareClicked(const ChessSquare::SquarePosition &position);
    void ShowPromotionWindow(const ChessSquare::SquarePosition &position);

signals:
    void SquareClicked(const ChessSquare::SquarePosition &position);
};
#endif // CHESSVIEW_H
