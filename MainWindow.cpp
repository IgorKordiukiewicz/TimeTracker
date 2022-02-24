#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include "TimeTracker.h"
#include "DateRangeSelector.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* refreshButton = new QPushButton("Refresh");

    dateRangeSelector = new DateRangeSelector;

    label = new QLabel("Placeholder");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(dateRangeSelector);
    mainLayout->addWidget(refreshButton);
    mainLayout->addWidget(label);

    auto* layoutWidget = new QWidget();
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);

    auto* timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeTracker()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(printData()));

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

void MainWindow::printData()
{
    timeTracker.print();
}

