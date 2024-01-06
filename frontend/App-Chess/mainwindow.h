#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "chesssquare.h"
#include <memory>
#include "ChessGame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateChessboard();

private slots:
    void onSquareClicked(const ChessSquare::Position& position);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QGridLayout> chessboardLayout;
    std::unique_ptr<ChessGame> chessGame;

    void initializeChessboard();
};

#endif // MAINWINDOW_H
