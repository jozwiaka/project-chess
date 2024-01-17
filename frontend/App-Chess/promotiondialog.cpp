#include "config.h"
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
        QString pieceImagePath = ChessPiece::GetPieceImageByColorAndType(type, color);
        button->setIcon(QIcon(pieceImagePath));
        button->setIconSize(QSize(80, 80));
        connect(button, &QPushButton::clicked, this, &PromotionDialog::OnPromotionButtonClicked);
        m_ButtonPieceTypeMap[button] = type;
        layout->addWidget(button);
        m_PromotionButtons.append(button);
    }
    setLayout(layout);
}

void PromotionDialog::OnPromotionButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (clickedButton)
    {
        ChessPiece::PieceType pieceType = m_ButtonPieceTypeMap.value(clickedButton, ChessPiece::PieceType::Pawn);

        emit PieceSelected(pieceType);
        accept();
    }
}
