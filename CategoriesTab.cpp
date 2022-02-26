#include "CategoriesTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include "CategorySettingsEdit.h"
#include "Utils.h"

CategoriesTab::CategoriesTab(CategoriesSettings& categoriesSettings, QWidget* parent)
    : QWidget(parent)
    , categoriesSettings(categoriesSettings)
{
    newCategoryNameLineEdit = new QLineEdit;
    auto* newCategoryButton = new QPushButton("Add Category");

    categoriesEditLayout = new QVBoxLayout(this);
    categoriesEditLayout->setAlignment(Qt::AlignTop);


    for(auto it = categoriesSettings.begin(); it != categoriesSettings.end(); ++it) {
        auto categorySettingsEdit = new CategorySettingsEdit(it.key(), it.value());
        categoriesEditLayout->addWidget(categorySettingsEdit);
        connect(categorySettingsEdit, &CategorySettingsEdit::deleteCategory, this, &CategoriesTab::onDeleteCategory);
    }

    auto* newCategoryLayout = new QHBoxLayout;
    newCategoryLayout->addWidget(newCategoryNameLineEdit);
    newCategoryLayout->addWidget(newCategoryButton);

    auto* scrollAreaWidget = new QWidget;
    scrollAreaWidget->setLayout(categoriesEditLayout);
    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaWidget);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(newCategoryLayout);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    connect(newCategoryButton, &QPushButton::clicked, this, &CategoriesTab::onNewCategoryButtonClicked);
}

void CategoriesTab::onNewCategoryButtonClicked()
{
    if(const QString categoryName = newCategoryNameLineEdit->text(); !categoryName.isEmpty() && !categoriesSettings.contains(categoryName)) {
        CategorySettings categorySettings;
        categorySettings.chartColor = Utils::generateRandomColorForChart();
        auto it = categoriesSettings.insert(categoryName, std::move(categorySettings));

        // Create new category settings edit widget
        auto* newCategorySettingsEdit = new CategorySettingsEdit(categoryName, it.value());
        categoriesEditLayout->addWidget(newCategorySettingsEdit);
        connect(newCategorySettingsEdit, &CategorySettingsEdit::deleteCategory, this, &CategoriesTab::onDeleteCategory);

        // Create a new scroll area widget to add the just created settings edit widget to the scroll area
        auto* oldScrollAreaWidget = scrollArea->takeWidget();
        oldScrollAreaWidget->deleteLater();
        auto* scrollAreaWidget = new QWidget;
        scrollAreaWidget->setLayout(categoriesEditLayout);
        scrollArea->setWidget(scrollAreaWidget);

        emit categoryAdded(categoryName);
    }
}

void CategoriesTab::onDeleteCategory(CategorySettingsEdit* categorySettingsEdit, const QString& categoryName)
{
    categoriesSettings.remove(categoryName);

    // Remove widget
    categoriesEditLayout->removeWidget(categorySettingsEdit);
    categorySettingsEdit->deleteLater();

    emit categoryRemoved(categoryName);
}
