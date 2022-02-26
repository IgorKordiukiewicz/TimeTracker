#include "ApplicationsTab.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QStringList>
#include "ApplicationSettingsEdit.h"

ApplicationsTab::ApplicationsTab(ApplicationsSettings& appsSettings, const CategoriesSettings& categoriesSettings, QWidget* parent)
    : QWidget(parent)
    , appsSettings(appsSettings)
{
    auto* editLayout = new QVBoxLayout;

    const QStringList categoriesList = [&categoriesSettings]() {
        QStringList list;
        list.append("");
        for(auto it{ categoriesSettings.constBegin() }; it != categoriesSettings.constEnd(); ++it) {
            list.append(it.key());
        }
        return list;
    }();

    for(auto it{ appsSettings.begin() }; it != appsSettings.end(); ++it) {
        auto* appSettingsEdit = new ApplicationSettingsEdit{ it.key(), it.value(), categoriesList };
        appSettingsEdits.push_back(appSettingsEdit);
        editLayout->addWidget(appSettingsEdit);
    }

    auto* scrollAreaWidget = new QWidget;
    scrollAreaWidget->setLayout(editLayout);

    auto* scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaWidget);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

void ApplicationsTab::onCategoryAdded(const QString& categoryName)
{
    for(ApplicationSettingsEdit* appSettingsEdit : appSettingsEdits) {
        appSettingsEdit->addCategory(categoryName);
    }
}

void ApplicationsTab::onCategoryRemoved(const QString& categoryName)
{
    for(ApplicationSettingsEdit* appSettingsEdit : appSettingsEdits) {
        appSettingsEdit->removeCategory(categoryName);
    }
}
