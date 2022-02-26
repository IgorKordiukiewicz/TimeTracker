#ifndef CHARTSVIEW_H
#define CHARTSVIEW_H

#include <QWidget>
#include "TimeTracker.h"
#include "Settings.h"

class QChartView;
class QChart;

class ChartsView : public QWidget
{
    Q_OBJECT

public:
    ChartsView(TimeTracker* timeTracker, QWidget* parent = nullptr);
    ~ChartsView();

    void saveSettings();
    void loadSettings();
    void setDateRange(const QDate& beginDate, const QDate& endDate);

private slots:
    void onGroupByComboBoxTextChanged(const QString& text);
    void onChartDataTypeComboBoxTextChanged(const QString& text);
    void onSettingsButtonClicked();
    void onNewAppTracked(const QString& appName);

private:
    void saveAppsSettings();
    void saveCategoriesSettings();
    void loadAppsSettings();
    void loadCategoriesSettings();
    void updateData();
    void updateChart();

private:   
    enum class ChartDataType
    {
        Applications,
        Categories,
        Activity
    } chartDataType{ ChartDataType::Applications };

    enum class GroupBy
    {
        None,
        Day,
        Week,
        Month,
        Year
    } groupBy{ GroupBy::None };

    QHash<QString, GroupBy> groupByNames{
        {"None", GroupBy::None},
        {"Day", GroupBy::Day},
        {"Week", GroupBy::Week},
        {"Month", GroupBy::Month},
        {"Year", GroupBy::Year},
    };

    ApplicationsSettings appsSettings;
    CategoriesSettings categoriesSettings;

    QColor activityChartColor{ QColor::fromHslF(0.55, 0.70, 0.55) };
    int maxBarSets{ 5 };

    TimeTracker* timeTracker;
    TimeTracker::AppData appData;
    QDate beginDate;
    QDate endDate;

    QChartView* chartView;
    QChart* chart;
};

#endif // CHARTSVIEW_H
