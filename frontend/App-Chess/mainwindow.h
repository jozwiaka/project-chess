#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "chesssquare.h"
#include <memory>

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
    void onSquareClicked(const QString &position);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QGridLayout> chessboardLayout;

    void initializeChessboard();
    // Add any other private methods for game logic
};

#endif // MAINWINDOW_H
