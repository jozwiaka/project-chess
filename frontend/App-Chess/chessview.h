#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QMainWindow>
#include <memory>

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
    ~ChessView();

private:
    std::unique_ptr<Ui::ChessView> ui;
};
#endif // CHESSVIEW_H
