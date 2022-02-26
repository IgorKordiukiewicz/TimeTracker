#include "SettingsDialog.h"
#include <QTabWidget>
#include "CategoriesTab.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "ApplicationsTab.h"

SettingsDialog::SettingsDialog(ApplicationsSettings& appsSettings, CategoriesSettings& categoriesSettings, QWidget* parent)
    : QDialog(parent)
{
    categoriesTab = new CategoriesTab(categoriesSettings);
    applicationsTab = new ApplicationsTab(appsSettings, categoriesSettings);

    auto* tabWidget = new QTabWidget;
    tabWidget->addTab(applicationsTab, "Applications");
    tabWidget->addTab(categoriesTab, "Categories");

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle("Settings");

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(categoriesTab, &CategoriesTab::categoryAdded, applicationsTab, &ApplicationsTab::onCategoryAdded);
    connect(categoriesTab, &CategoriesTab::categoryRemoved, applicationsTab, &ApplicationsTab::onCategoryRemoved);
}
