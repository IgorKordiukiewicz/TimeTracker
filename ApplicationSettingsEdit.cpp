#include "ApplicationSettingsEdit.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QColorDialog>

ApplicationSettingsEdit::ApplicationSettingsEdit(const QString& appName, ApplicationSettings& appSettings, QWidget* parent)
    : QWidget(parent)
    , appName(appName)
    , appSettings(appSettings)
{
    auto* appNameLabel = new QLabel(appName);
    appNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    appDisplayNameEdit = new QLineEdit;
    appDisplayNameEdit->setText(appName);

    colorButton = new QPushButton("Color");
    QPixmap pixmap(16, 16);
    pixmap.fill(appSettings.chartColor);
    colorButton->setIcon(pixmap);

    auto* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(appNameLabel);
    mainLayout->addWidget(appDisplayNameEdit);
    mainLayout->addWidget(colorButton);
    setLayout(mainLayout);

    connect(appDisplayNameEdit, &QLineEdit::textChanged, this, &ApplicationSettingsEdit::onAppDisplayNameEditChanged);
    connect(colorButton, &QPushButton::clicked, this, &ApplicationSettingsEdit::onColorButtonClicked);
}

void ApplicationSettingsEdit::onAppDisplayNameEditChanged(const QString& text)
{
    if(text.isEmpty()) {
        appDisplayNameEdit->setText(appName);
        return;
    }

    appSettings.displayName = text;
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
