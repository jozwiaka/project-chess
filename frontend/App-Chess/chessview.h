#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include <memory>
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
    void CreateChessboardGraphics(QVector<QVector<std::shared_ptr<ChessSquare>>> chessboard);

    void OnSquareClicked(const ChessSquare::Position& position);

    void UpdateChessboardGraphics(const ChessSquare::Position& position);

    ~ChessView();

signals:
    void SquareClicked(const ChessSquare::Position& position);

private:
    std::unique_ptr<Ui::ChessView> m_Ui;
    std::unique_ptr<QGridLayout> m_ChessboardLayout;

};
#endif // CHESSVIEW_H
