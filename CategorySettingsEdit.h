#ifndef CATEGORIESSETTINGSEDIT_H
#define CATEGORIESSETTINGSEDIT_H

#include <QWidget>
#include "Settings.h"

class QLineEdit;
class QPushButton;

class CategorySettingsEdit : public QWidget
{
    Q_OBJECT

public:
    CategorySettingsEdit(const QString& categoryName, CategorySettings& categorySettings, QWidget* parent = nullptr);

signals:
    void deleteCategory(CategorySettingsEdit* categorySettingsEdit, const QString& categoryName);

private slots:
    void onColorButtonClicked();
    void onDeleteButtonClicked();

private:
    QString categoryName;
    CategorySettings& categorySettings;

    QLineEdit* categoryNameEdit;
    QPushButton* colorButton;
};

#endif // CATEGORIESSETTINGSEDIT_H
