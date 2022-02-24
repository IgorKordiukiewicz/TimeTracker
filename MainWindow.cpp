#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include "TimeTracker.h"
#include "DateRangeSelector.h"
#include "ChartsView.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* refreshButton = new QPushButton("Refresh");

    dateRangeSelector = new DateRangeSelector;
    chartsView = new ChartsView(&timeTracker);

    label = new QLabel("Placeholder");

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dateRangeSelector);
    mainLayout->addWidget(chartsView);
    mainLayout->addWidget(refreshButton);
    mainLayout->addWidget(label);

    auto* layoutWidget = new QWidget;
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);

    auto* timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeTracker()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(printData()));
    connect(dateRangeSelector, SIGNAL(dateChanged(QDate, QDate)), this, SLOT(onDateRangeChanged(QDate, QDate)));

    timeTracker.loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimeTracker()
{
    label->setText(TimeTracker::getCurrentApplicationName());
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

