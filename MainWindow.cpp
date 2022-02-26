#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include "TimeTracker.h"
#include "DateRangeSelector.h"
#include "ChartsView.h"
#include <QPushButton>
#include <QFrame>
#include <QSystemTrayIcon>
#include <QAction>
#include <QCloseEvent>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dateRangeSelector = new DateRangeSelector;
    chartsView = new ChartsView{ &timeTracker };

    auto* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dateRangeSelector);
    mainLayout->addWidget(line);
    mainLayout->addWidget(chartsView);

    auto* layoutWidget = new QWidget;
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);

    minimizeAction = new QAction{ "Minimize", this };
    maximizeAction = new QAction{ "Maximize", this };
    restoreAction = new QAction{ "Restore", this };
    quitAction = new QAction{ "Quit", this };

    auto* trayIconMenu = new QMenu{ this };
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon{ this };
    trayIcon->setContextMenu(trayIconMenu);
    // TODO: create a real icon
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::red);
    trayIcon->setIcon(pixmap);
    trayIcon->show();

    auto* timer = new QTimer{ this };
    timer->start(1000);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimeTracker);
    connect(dateRangeSelector, &DateRangeSelector::dateChanged, this, &MainWindow::onDateRangeChanged);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);

    timeTracker.loadData();
    chartsView->loadSettings();
    chartsView->setDateRange(dateRangeSelector->getBeginDate(), dateRangeSelector->getEndDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if(trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::updateTimeTracker()
{
    timeTracker.update();
}

void MainWindow::onDateRangeChanged(QDate beginDate, QDate endDate)
{
    chartsView->setDateRange(beginDate, endDate);
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        show();
        break;
    default:
        break;
    }
}

void MainWindow::printData()
{
    timeTracker.print();
}

