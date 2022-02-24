#include "ChartsView.h"
#include "TimeTracker.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QVBoxLayout>

ChartsView::ChartsView(TimeTracker* timeTracker, QWidget* parent)
    : QWidget(parent)
    , timeTracker(timeTracker)
{
    chart = new QChart;

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
}

void ChartsView::setDateRange(const QDate& beginDate, const QDate& endDate)
{
    appData = timeTracker->getDataInRange(beginDate, endDate);
    updateChart();
}

void ChartsView::updateChart()
{
    // Initialize bar series
    auto* barSeries = new QBarSeries;
    const auto keys = appData.keys();
    for(const auto& key : keys) {
        auto* barSet = new QBarSet(key);
        int seconds = 0;
        for(const auto& dateRange : appData.value(key)) {
            seconds += dateRange.first.secsTo(dateRange.second);
        }
        qDebug() << seconds;
        *barSet << seconds; // TODO: Change to minutes
        barSeries->append(barSet);
    }

    chart = new QChart;
    chart->setTitle("Application time");
    chart->addSeries(barSeries);

    auto* oldChart = chartView->chart();
    chartView->setChart(chart);
    delete oldChart;
}
