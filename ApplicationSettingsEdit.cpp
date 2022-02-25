#include "ApplicationSettingsEdit.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

ApplicationSettingsEdit::ApplicationSettingsEdit(const QString& appName, QWidget* parent)
    : QWidget(parent)
    , appName(appName)
{
    auto* appNameLabel = new QLabel(appName);

    appDisplayNameEdit = new QLineEdit;
    appDisplayNameEdit->setText(appName);

    auto* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(appNameLabel);
    mainLayout->addWidget(appDisplayNameEdit);
    setLayout(mainLayout);

    connect(appDisplayNameEdit, &QLineEdit::textChanged, this, &ApplicationSettingsEdit::onAppDisplayNameEditChanged);
}

void ApplicationSettingsEdit::onAppDisplayNameEditChanged(const QString& text)
{
    if(text.isEmpty()) {
        appDisplayNameEdit->setText(appName);
        return;
    }

    emit appDisplayNameChanged(appName, text);
}
