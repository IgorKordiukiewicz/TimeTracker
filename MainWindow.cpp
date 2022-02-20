#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include "TimeTracker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    label = new QLabel("Placeholder");

    mainLayout = new QHBoxLayout();
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

