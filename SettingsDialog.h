#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "Settings.h"

class CategoriesTab;
class ApplicationsTab;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(ApplicationsSettings appsSettings, CategoriesSettings categoriesSettings, QWidget* parent = nullptr);

    const ApplicationsSettings& getAppsSettings() const;
    const CategoriesSettings& getCategoriesSettings() const;

private slots:
    virtual void accept() override;

private:
    CategoriesTab* categoriesTab;
    ApplicationsTab* applicationsTab;

    ApplicationsSettings appsSettings;
    CategoriesSettings categoriesSettings;
};

#endif // SETTINGSDIALOG_H
