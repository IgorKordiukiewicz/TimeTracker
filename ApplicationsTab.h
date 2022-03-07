#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QWidget>
#include "Settings.h"

class ApplicationSettingsEdit;

class ApplicationsTab : public QWidget
{
    Q_OBJECT

public:
    ApplicationsTab(ApplicationsSettings& appsSettings, const CategoriesSettings& categoriesSettings, QWidget* parent = nullptr);

    void ensureCorrectChanges();

public slots:
    void onCategoryAdded(const QString& categoryName);
    void onCategoryRemoved(const QString& categoryName);

private:
    ApplicationsSettings& appsSettings;
    QVector<ApplicationSettingsEdit*> appSettingsEdits;
};

#endif // APPLICATIONSTAB_H
