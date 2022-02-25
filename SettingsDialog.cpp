#include "SettingsDialog.h"
#include <QTabWidget>
#include "CategoriesTab.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include "ApplicationsTab.h"

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    categoriesTab = new CategoriesTab;
    applicationsTab = new ApplicationsTab;

    auto* tabWidget = new QTabWidget;
    tabWidget->addTab(categoriesTab, "Categories");
    tabWidget->addTab(applicationsTab, "Applications");

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle("Settings");

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
