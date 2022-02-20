#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QVBoxLayout;
class DateRangeSelector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateLabel();

private:
    Ui::MainWindow *ui;

    QVBoxLayout* mainLayout;
    DateRangeSelector* dateRangeSelector;
    QLabel* label;
};
#endif // MAINWINDOW_H
