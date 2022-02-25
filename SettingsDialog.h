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
    SettingsDialog(ApplicationsSettings& appsSettings, CategoriesSettings& categoriesSettings, QWidget* parent = nullptr);

private:
    CategoriesTab* categoriesTab;
    ApplicationsTab* applicationsTab;
};

#endif // SETTINGSDIALOG_H
