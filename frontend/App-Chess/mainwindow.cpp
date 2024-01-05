#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chesspiece.h"
#include "chesssquare.h"
#include <QTextStream>
#include <QApplication>
#include <QPainter>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chessboardLayout = std::make_unique<QGridLayout>(ui->centralwidget);
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
    bool dark = false;
    for (char row = '1'; row <= '8'; ++row)
    {
        dark = !dark;
        for (char col = 'A'; col <= 'H'; ++col)
        {
            QString position = QString("%1%2").arg(col).arg(row);
            ChessSquare *square = new ChessSquare(dark, position, this);
            dark = !dark;
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

            auto piece = std::make_unique<ChessPiece>(pieceImagePath, pieceType, square);
            square->setChessPiece(std::move(piece));
            connect(square, &ChessSquare::clicked, this, &MainWindow::onSquareClicked);
        }
    }
}

void MainWindow::updateChessboard() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessSquare *square = qobject_cast<ChessSquare*>(chessboardLayout->itemAtPosition(row, col)->widget());

            if (square) {
                ChessPiece *piece = square->getChessPiece();

                // Update the chess piece representation based on the game state
                if (piece) {
                    // Example: Change the image path of the piece
                    QTextStream(stdout) << "Type: " << piece->getPieceType() << "\n";
                    square->highlightSquare();

                } else {
                    // Example: Set an empty image path if the square has no piece

                }
            }
        }
    }
}


void MainWindow::onSquareClicked(const QString &position)
{
    ChessSquare *clickedSquare = qobject_cast<ChessSquare *>(sender());

    //clickedSquare = qobject_cast<ChessSquare*>(chessboardLayout->itemAtPosition('1','A')->widget());

    if (clickedSquare)
    {
        ChessPiece *clickedPiece = clickedSquare->getChessPiece();
        clickedSquare->setChessPiece(nullptr);
        QTextStream(stdout) << "Position: " << position << "\n";
        // updateChessboard();

        // Implement your game logic here
        // You can use clickedPiece->getPieceType(), clickedPiece->isWhite(), etc.
    }
}
