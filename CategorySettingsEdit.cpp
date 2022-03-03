#include "CategorySettingsEdit.h"
#include <QLineEdit>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QImageReader>

CategorySettingsEdit::CategorySettingsEdit(const QString& categoryName, CategorySettings& categorySettings, QWidget* parent)
    : QWidget(parent)
    , categoryName(categoryName)
    , categorySettings(categorySettings)
{
    categoryNameEdit = new QLineEdit;
    categoryNameEdit->setText(categoryName);
    categoryNameEdit->setFixedWidth(250);

    colorButton = new QPushButton{ "Color" };
    QPixmap pixmap{ 16, 16 };
    pixmap.fill(categorySettings.chartColor);
    colorButton->setIcon(pixmap);

    auto* deleteButton = new QPushButton{ "" };
    QIcon deleteButtonIcon("resources/delete.png");
    deleteButton->setIcon(deleteButtonIcon);

    auto* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(categoryNameEdit);
    mainLayout->addWidget(colorButton);
    mainLayout->addWidget(deleteButton);
    setLayout(mainLayout);

    connect(colorButton, &QPushButton::clicked, this, &CategorySettingsEdit::onColorButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &CategorySettingsEdit::onDeleteButtonClicked);
}

void CategorySettingsEdit::onColorButtonClicked()
{
    QColorDialog colorDialog{ categorySettings.chartColor };
    if(colorDialog.exec()) {
        categorySettings.chartColor = colorDialog.selectedColor();

        QPixmap pixmap{ 16, 16 };
        pixmap.fill(categorySettings.chartColor);
        colorButton->setIcon(pixmap);
    }
}

void CategorySettingsEdit::onDeleteButtonClicked()
{
    emit deleteCategory(this, categoryName);
}
