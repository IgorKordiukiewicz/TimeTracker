#ifndef CATEGORIESTAB_H
#define CATEGORIESTAB_H

#include <QWidget>
#include "Settings.h"

class QLineEdit;
class QVBoxLayout;
class QScrollArea;
class CategorySettingsEdit;

class CategoriesTab : public QWidget
{
    Q_OBJECT

public:
    CategoriesTab(CategoriesSettings& categoriesSettings, QWidget* parent = nullptr);

private slots:
    void onNewCategoryButtonClicked();
    void onDeleteCategory(CategorySettingsEdit* categorySettingsEdit, const QString& categoryName);

private:
    QLineEdit* newCategoryNameLineEdit;
    QVBoxLayout* categoriesEditLayout;
    QScrollArea* scrollArea;

    CategoriesSettings& categoriesSettings;
};

#endif // CATEGORIESTAB_H
