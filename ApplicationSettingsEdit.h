#ifndef APPLICATIONSETTINGSEDIT_H
#define APPLICATIONSETTINGSEDIT_H

#include <QWidget>

class QLineEdit;

class ApplicationSettingsEdit : public QWidget
{
    Q_OBJECT

public:
    ApplicationSettingsEdit(const QString& appName, QWidget* parent = nullptr);

signals:
    void appDisplayNameChanged(const QString& appName, const QString& newAppDisplayName);

private slots:
    void onAppDisplayNameEditChanged(const QString& text);

private:
    QString appName;

    QLineEdit* appDisplayNameEdit;
};

#endif // APPLICATIONSETTINGSEDIT_H
