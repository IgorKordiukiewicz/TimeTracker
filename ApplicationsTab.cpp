#include "ApplicationsTab.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include "ApplicationSettingsEdit.h"

ApplicationsTab::ApplicationsTab(ApplicationsSettings& appsSettings, QWidget* parent)
    : QWidget(parent)
    , appsSettings(appsSettings)
{
    auto* editLayout = new QVBoxLayout;

    auto it = appsSettings.constBegin();
    while(it != appsSettings.constEnd()) {
        auto* appSettingsEdit = new ApplicationSettingsEdit(it.key());
        editLayout->addWidget(appSettingsEdit);

        connect(appSettingsEdit, &ApplicationSettingsEdit::appDisplayNameChanged, this, &ApplicationsTab::onAppDisplayNameChanged);

        ++it;
    }

    auto* scrollAreaWidget = new QWidget;
    scrollAreaWidget->setLayout(editLayout);

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaWidget);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

void ApplicationsTab::onAppDisplayNameChanged(const QString& appName, const QString& newAppDisplayName)
{
    if(auto it = appsSettings.find(appName); it != appsSettings.end()) {
        it->displayName = newAppDisplayName;
    }
}
