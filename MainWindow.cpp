#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include "TimeTracker.h"
#include "DateRangeSelector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dateRangeSelector = new DateRangeSelector;

    label = new QLabel("Placeholder");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(dateRangeSelector);
    mainLayout->addWidget(label);

    QWidget* layoutWidget = new QWidget();
    layoutWidget->setLayout(mainLayout);
    setCentralWidget(layoutWidget);

    QTimer* timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateLabel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLabel()
{
    label->setText(TimeTracker::getCurrentApplicationName());
}

