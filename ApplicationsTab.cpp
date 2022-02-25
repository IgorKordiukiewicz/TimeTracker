#include "ApplicationsTab.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include "ApplicationSettingsEdit.h"

ApplicationsTab::ApplicationsTab(ApplicationsSettings& appsSettings, QWidget* parent)
    : QWidget(parent)
    , appsSettings(appsSettings)
{
    auto* editLayout = new QVBoxLayout;

    auto it = appsSettings.begin();
    while(it != appsSettings.end()) {
        auto* appSettingsEdit = new ApplicationSettingsEdit(it.key(), it.value());
        editLayout->addWidget(appSettingsEdit);
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
