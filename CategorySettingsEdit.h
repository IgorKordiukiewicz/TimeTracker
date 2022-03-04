#ifndef CATEGORIESSETTINGSEDIT_H
#define CATEGORIESSETTINGSEDIT_H

#include <QWidget>
#include "Settings.h"

class QPushButton;
class QLabel;

class CategorySettingsEdit : public QWidget
{
    Q_OBJECT

public:
    CategorySettingsEdit(const QString& categoryName, CategorySettings& categorySettings, QWidget* parent = nullptr);

    void applyChanges();

signals:
    void deleteCategory(CategorySettingsEdit* categorySettingsEdit, const QString& categoryName);

private slots:
    void onColorButtonClicked();
    void onDeleteButtonClicked();

private:
    QString categoryName;
    CategorySettings& categorySettings;

    QColor selectedColor;

    QLabel* categoryNameLabel;
    QPushButton* colorButton;
};

#endif // CATEGORIESSETTINGSEDIT_H
