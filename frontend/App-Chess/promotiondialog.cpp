#include "promotiondialog.h"
#include <QHBoxLayout>
#include <QMap>

PromotionDialog::PromotionDialog(const ChessPiece::PieceColor &color, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);

    QHBoxLayout *layout = new QHBoxLayout(this);

    QList<ChessPiece::PieceType> promotionChoices = {ChessPiece::PieceType::Queen, ChessPiece::PieceType::Rook, ChessPiece::PieceType::Bishop, ChessPiece::PieceType::Knight};

    for (const auto &type : promotionChoices)
    {
        auto button = new QPushButton(this);
        QString pieceImagePath = ChessPiece::GetPieceImageByColorAndType(type, color); // Use a method to get the image path
        button->setIcon(QIcon(pieceImagePath));
        button->setIconSize(QSize(80, 80));
        connect(button, &QPushButton::clicked, this, &PromotionDialog::OnPromotionButtonClicked);

        // Associate each button with its ChessPiece::PieceType using a QMap
        m_ButtonPieceTypeMap[button] = type;

        // Add the button to the horizontal layout
        layout->addWidget(button);

        m_PromotionButtons.append(button);
    }

    // Set the layout for the dialog
    setLayout(layout);
}

void PromotionDialog::OnPromotionButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        // Retrieve the ChessPiece::PieceType associated with the clicked button
        ChessPiece::PieceType pieceType = m_ButtonPieceTypeMap.value(clickedButton, ChessPiece::PieceType::Pawn);

        emit PieceSelected(pieceType);
        accept();
    }
}
