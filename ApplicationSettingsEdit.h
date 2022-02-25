#ifndef APPLICATIONSETTINGSEDIT_H
#define APPLICATIONSETTINGSEDIT_H

#include <QWidget>
#include "ApplicationSettings.h"

class QLineEdit;
class QPushButton;

class ApplicationSettingsEdit : public QWidget
{
    Q_OBJECT

public:
    ApplicationSettingsEdit(const QString& appName, ApplicationSettings& appSettings, QWidget* parent = nullptr);

signals:
    void appDisplayNameChanged(const QString& appName, const QString& newAppDisplayName);

private slots:
    void onAppDisplayNameEditChanged(const QString& text);
    void onColorButtonClicked();

private:
    QString appName;
    ApplicationSettings& appSettings;

    QLineEdit* appDisplayNameEdit;
    QPushButton* colorButton;
};

#endif // APPLICATIONSETTINGSEDIT_H
