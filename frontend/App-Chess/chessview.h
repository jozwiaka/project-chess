#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include "chesssquare.h"
#include "chesspiece.h"
#include <QGridLayout>
#include "chessmodel.h"
#include <QVector>

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
    ChessView(bool *computerTurn, bool *promotionProcedure, QWidget *parent = nullptr);
    void CreateChessboardGraphics(Chessboard::ChessboardType chessboard);

    void CreateMenuGraphics();

    void OnSquareClicked(const ChessSquare::SquarePosition &position);

    void UpdateChessboardGraphics();

    ~ChessView();

signals:
    void SquareClicked(const ChessSquare::SquarePosition &position);

private:
    Ui::ChessView *m_Ui;
    QGridLayout *m_ChessboardLayout;
    QGridLayout *m_MenuLayout;

    bool *m_ComputerTurn;
    bool *m_PromotionProcedure;
};
#endif // CHESSVIEW_H
