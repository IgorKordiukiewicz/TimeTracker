#include "ApplicationSettingsEdit.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QColorDialog>
#include <QComboBox>

ApplicationSettingsEdit::ApplicationSettingsEdit(const QString& appName, ApplicationSettings& appSettings, const QStringList& categoriesList, QWidget* parent)
    : QWidget(parent)
    , appName(appName)
    , appSettings(appSettings)
{
    auto* appNameLabel = new QLabel(appName);
    appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    appDisplayNameEdit = new QLineEdit;
    appDisplayNameEdit->setText(appName);

    categoriesComboBox = new QComboBox;
    categoriesComboBox->addItems(categoriesList);
    categoriesComboBox->setCurrentText(appSettings.categoryName);

    colorButton = new QPushButton("Color");
    QPixmap pixmap(16, 16);
    pixmap.fill(appSettings.chartColor);
    colorButton->setIcon(pixmap);

    auto* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(appNameLabel);
    mainLayout->addWidget(appDisplayNameEdit);
    mainLayout->addWidget(categoriesComboBox);
    mainLayout->addWidget(colorButton);
    setLayout(mainLayout);

    connect(appDisplayNameEdit, &QLineEdit::textChanged, this, &ApplicationSettingsEdit::onAppDisplayNameEditChanged);
    connect(categoriesComboBox, &QComboBox::currentTextChanged, this, &ApplicationSettingsEdit::onCategoryComboBoxCurrentTextChanged);
    connect(colorButton, &QPushButton::clicked, this, &ApplicationSettingsEdit::onColorButtonClicked);
}

void ApplicationSettingsEdit::addCategory(const QString &categoryName)
{
    categoriesComboBox->addItem(categoryName);
}

void ApplicationSettingsEdit::removeCategory(const QString &categoryName)
{
    if(int idx = categoriesComboBox->findText(categoryName); idx != -1) {
        categoriesComboBox->removeItem(idx);
        if(appSettings.categoryName == categoryName) {
            appSettings.categoryName.clear();
            categoriesComboBox->setCurrentIndex(0);
        }
    }
}

void ApplicationSettingsEdit::onAppDisplayNameEditChanged(const QString& text)
{
    if(text.isEmpty()) {
        appDisplayNameEdit->setText(appName);
        return;
    }

    appSettings.displayName = text;
}

void ApplicationSettingsEdit::onCategoryComboBoxCurrentTextChanged(const QString& text)
{
    appSettings.categoryName = text;
}

void ApplicationSettingsEdit::onColorButtonClicked()
{
    QColorDialog colorDialog(appSettings.chartColor);
    if(colorDialog.exec()) {
        appSettings.chartColor = colorDialog.selectedColor();

        QPixmap pixmap(16, 16);
        pixmap.fill(appSettings.chartColor);
        colorButton->setIcon(pixmap);
    }
}
