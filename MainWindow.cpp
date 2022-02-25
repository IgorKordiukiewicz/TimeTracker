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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dateRangeSelector = new DateRangeSelector;
    chartsView = new ChartsView(&timeTracker);

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

    auto* timer = new QTimer(this);
    timer->start(1000);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimeTracker);
    connect(dateRangeSelector, &DateRangeSelector::dateChanged, this, &MainWindow::onDateRangeChanged);

    timeTracker.loadData();
    chartsView->loadAppsSettings();
    chartsView->setDateRange(dateRangeSelector->getBeginDate(), dateRangeSelector->getEndDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimeTracker()
{
    timeTracker.update();
}

void MainWindow::onDateRangeChanged(QDate beginDate, QDate endDate)
{
    chartsView->setDateRange(beginDate, endDate);
}

void MainWindow::printData()
{
    timeTracker.print();
}

