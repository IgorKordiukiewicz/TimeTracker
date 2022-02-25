#include "SettingsDialog.h"
#include <QTabWidget>
#include "CategoriesTab.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "ApplicationsTab.h"

SettingsDialog::SettingsDialog(ApplicationsSettings& appsSettings, QWidget* parent)
    : QDialog(parent)
{
    categoriesTab = new CategoriesTab;
    applicationsTab = new ApplicationsTab(appsSettings);

    auto* tabWidget = new QTabWidget;
    tabWidget->addTab(categoriesTab, "Categories");
    tabWidget->addTab(applicationsTab, "Applications");

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle("Settings");

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
