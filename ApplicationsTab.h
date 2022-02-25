#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QWidget>
#include "ApplicationSettings.h"

class ApplicationsTab : public QWidget
{
    //Q_OBJECT

public:
    ApplicationsTab(ApplicationsSettings& appsSettings, QWidget* parent = nullptr);

private:
    ApplicationsSettings& appsSettings;
};

#endif // APPLICATIONSTAB_H
