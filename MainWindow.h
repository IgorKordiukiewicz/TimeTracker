#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TimeTracker.h"
#include <QSystemTrayIcon>

class QLabel;
class QVBoxLayout;
class DateRangeSelector;
class ChartsView;
class QAction;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void setVisible(bool visible) override;

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    void onDateRangeChanged(QDate beginDate, QDate endDate);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    TimeTracker timeTracker;

    Ui::MainWindow *ui;

    QVBoxLayout* mainLayout;
    DateRangeSelector* dateRangeSelector;
    ChartsView* chartsView;
    QLabel* label;

    QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* restoreAction;
    QAction* quitAction;
    QSystemTrayIcon* trayIcon;
};
#endif // MAINWINDOW_H
