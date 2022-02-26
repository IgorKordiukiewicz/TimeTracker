#ifndef APPLICATIONSETTINGSEDIT_H
#define APPLICATIONSETTINGSEDIT_H

#include <QWidget>
#include "Settings.h"

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

private slots:
    void onAppDisplayNameEditChanged(const QString& text);
    void onCategoryComboBoxCurrentTextChanged(const QString& text);
    void onColorButtonClicked();

private:
    QString appName;
    ApplicationSettings& appSettings;

    QLineEdit* appDisplayNameEdit;
    QComboBox* categoriesComboBox;
    QPushButton* colorButton;
};

#endif // APPLICATIONSETTINGSEDIT_H
