#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chesspiece.h"
#include "chesssquare.h"
#include <QTextStream>
#include <QApplication>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chessboardLayout = new QGridLayout(ui->centralwidget);
    chessboardLayout->setSpacing(0);
    chessboardLayout->setAlignment(Qt::AlignCenter);
    initializeChessboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeChessboard()
{
    bool black = false;
    for (char row = '1'; row <= '8'; ++row)
    {
        black = !black;
        for (char col = 'A'; col <= 'H'; ++col)
        {
            QString position = QString("%1%2").arg(col).arg(row);
            QString squareImagePath;
            if (black)
            {
                squareImagePath = "../App-Chess/resources/square_brown_dark_1x_ns.png";
            }
            else
            {
                squareImagePath = "../App-Chess/resources/square_brown_light_1x_ns.png";
            }
            black = !black;
            ChessSquare *square = new ChessSquare(squareImagePath, position, this);
            chessboardLayout->addWidget(square, '8' - row, col - 'A');

            ChessPiece::PieceType pieceType;
            QString pieceImagePath;

            switch (row)
            {
            case '1':
            case '8':
                switch (col)
                {
                case 'A':
                case 'H':
                    pieceType = ChessPiece::Rook;
                    pieceImagePath = (row == '1') ? "../App-Chess/resources/w_rook_1x_ns.png" : "../App-Chess/resources/b_rook_1x_ns.png";
                    break;
                case 'B':
                case 'G':
                    pieceType = ChessPiece::Knight;
                    pieceImagePath = (row == '1') ? "../App-Chess/resources/w_knight_1x_ns.png" : "../App-Chess/resources/b_knight_1x_ns.png";
                    break;
                case 'C':
                case 'F':
                    pieceType = ChessPiece::Bishop;
                    pieceImagePath = (row == '1') ? "../App-Chess/resources/w_bishop_1x_ns.png" : "../App-Chess/resources/b_bishop_1x_ns.png";
                    break;
                case 'D':
                    pieceType = ChessPiece::Queen;
                    pieceImagePath = (row == '1') ? "../App-Chess/resources/w_queen_1x_ns.png" : "../App-Chess/resources/b_queen_1x_ns.png";
                    break;
                case 'E':
                    pieceType = ChessPiece::King;
                    pieceImagePath = (row == '1') ? "../App-Chess/resources/w_king_1x_ns.png" : "../App-Chess/resources/b_king_1x_ns.png";
                    break;
                }
                break;

            case '2':
            case '7':
                pieceType = ChessPiece::Pawn;
                pieceImagePath = (row == '2') ? "../App-Chess/resources/w_pawn_1x_ns.png" : "../App-Chess/resources/b_pawn_1x_ns.png";
                break;

            default:
                continue;
            }

            ChessPiece *piece = new ChessPiece(pieceImagePath, pieceType, square);
            square->setChessPiece(piece);
            connect(square, &ChessSquare::clicked, this, &MainWindow::onSquareClicked);
        }
    }
}

void MainWindow::onSquareClicked(const QString &position)
{
    ChessSquare *clickedSquare = qobject_cast<ChessSquare *>(sender());
    if (clickedSquare)
    {
        ChessPiece *clickedPiece = clickedSquare->getChessPiece();
        QTextStream(stdout) << "Position: " << position << "\n";
        // Implement your game logic here
        // You can use clickedPiece->getPieceType(), clickedPiece->isWhite(), etc.
    }
}
