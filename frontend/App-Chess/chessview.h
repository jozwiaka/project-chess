#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include "chesssquare.h"
#include "chesspiece.h"
#include <QGridLayout>
#include "chessmodel.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChessView;
}
QT_END_NAMESPACE

class ChessView : public QMainWindow
{
    Q_OBJECT

public:
    ChessView(QWidget *parent = nullptr);
    void CreateChessboardGraphics(QVector<QVector<ChessSquare*>> chessboard, bool reversed = false);

    void CreateMenuGraphics();

    void OnSquareClicked(const ChessSquare::Position& position);

    void UpdateChessboardGraphics();

    ~ChessView();

signals:
    void SquareClicked(const ChessSquare::Position& position);

private:
    Ui::ChessView* m_Ui;
    QGridLayout* m_ChessboardLayout;
    QGridLayout* m_MenuLayout;

};
#endif // CHESSVIEW_H
