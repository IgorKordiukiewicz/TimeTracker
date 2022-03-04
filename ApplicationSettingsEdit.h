#ifndef APPLICATIONSETTINGSEDIT_H
#define APPLICATIONSETTINGSEDIT_H

#include <QWidget>
#include "Settings.h"
#include <QColor>

class QLineEdit;
class QPushButton;
class QComboBox;

class ApplicationSettingsEdit : public QWidget
{
    Q_OBJECT

public:
    ApplicationSettingsEdit(const QString& appName, ApplicationSettings& appSettings, const QStringList& categoriesList, QWidget* parent = nullptr);

    void addCategory(const QString& categoryName);
    void removeCategory(const QString& categoryName);

    void applyChanges();

private slots:
    void onColorButtonClicked();

private:
    QString appName;
    ApplicationSettings& appSettings;

    QColor selectedColor;

    QLineEdit* appDisplayNameEdit;
    QComboBox* categoriesComboBox;
    QPushButton* colorButton;
};

#endif // APPLICATIONSETTINGSEDIT_H
