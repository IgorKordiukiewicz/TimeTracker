#ifndef CHARTSVIEW_H
#define CHARTSVIEW_H

#include <QWidget>
#include <QDate>
#include "TimeTracker.h"

class QChartView;
class QChart;

class ChartsView : public QWidget
{
    Q_OBJECT

public:
    ChartsView(TimeTracker* timeTracker, QWidget* parent = nullptr);

    void setDateRange(const QDate& fromDate, const QDate& toDate);

private:
    void updateChart();

private:
    enum class GroupBy
    {
        None,
        Day,
        Week,
        Month,
        Year
    } groupBy{ GroupBy::None };

    TimeTracker* timeTracker;
    TimeTracker::AppData appData;

    QChartView* chartView;
    QChart* chart;
};

#endif // CHARTSVIEW_H
