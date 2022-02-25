#include "ChartsView.h"
#include "TimeTracker.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QTabWidget>

ChartsView::ChartsView(TimeTracker* timeTracker, QWidget* parent)
    : QWidget(parent)
    , timeTracker(timeTracker)
{
    chart = new QChart;

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto* groupByLabel = new QLabel("Group by:");

    auto* groupByComboBox = new QComboBox;
    // Items have to be added manually because in QHash the items are arbitrarily ordered
    groupByComboBox->addItem("None");
    groupByComboBox->addItem("Day");
    groupByComboBox->addItem("Week");
    groupByComboBox->addItem("Month");
    groupByComboBox->addItem("Year");

    auto* chartDataTypeComboBox = new QComboBox;
    chartDataTypeComboBox->addItem("Applications");
    chartDataTypeComboBox->addItem("Categories");
    chartDataTypeComboBox->addItem("Activity");

    auto* groupByLayout = new QHBoxLayout;
    groupByLayout->setAlignment(Qt::AlignLeft);
    groupByLayout->addWidget(chartDataTypeComboBox);
    groupByLayout->addWidget(groupByLabel);
    groupByLayout->addWidget(groupByComboBox);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(groupByLayout);
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);

    connect(groupByComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGroupByComboBoxTextChanged(QString)));
    connect(chartDataTypeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onChartDataTypeComboBoxTextChanged(QString)));
}

void ChartsView::setDateRange(const QDate& beginDate, const QDate& endDate)
{
    this->beginDate = beginDate;
    this->endDate = endDate;
    updateData();
}

void ChartsView::onGroupByComboBoxTextChanged(const QString& text)
{
    auto it = groupByNames.find(text);
    if(it != groupByNames.end()) {
        groupBy = it.value();
    }
    else {
        groupBy = GroupBy::None;
    }

    updateChart();
}

void ChartsView::onChartDataTypeComboBoxTextChanged(const QString& text)
{
    const ChartDataType newChartDataType = [&text](){
        if(text == "Applications") {
            return ChartDataType::Applications;
        }
        else if(text == "Categories") {
            return ChartDataType::Categories;
        }
        else if(text == "Activity") {
            return ChartDataType::Activity;
        }
        else {
            return ChartDataType::Applications;
        }
    }();

    if(newChartDataType != chartDataType) {
        chartDataType = newChartDataType;
        updateData();
    }
}

void ChartsView::updateData()
{
    appData = timeTracker->getDataInRange(beginDate, endDate);

    if(chartDataType == ChartDataType::Categories) {

    }
    else if(chartDataType == ChartDataType::Activity) {
        TimeTracker::AppData newAppData;
        auto dateRangesIt = newAppData.insert("Activity", QVector<TimeTracker::DateTimeRange>());
        auto it = appData.constBegin();
        while(it != appData.constEnd()) {
            for(auto dateRange : appData.value(it.key())) {
                dateRangesIt->push_back(std::move(dateRange));
            }
            ++it;
        }

        // Date ranges have to be sorted in increasing order to work properly
        std::sort(dateRangesIt->begin(), dateRangesIt->end(), [](const auto& range1, const auto& range2){
            return range1.first < range2.first;
        });

        appData = std::move(newAppData);
    }

    updateChart();
}

void ChartsView::updateChart()
{
    QVector<QPair<QDate, QDate>> dateGroups;
    QStringList categories;
    if(groupBy == GroupBy::None) {
        dateGroups.push_back({beginDate, endDate});
        categories.append(beginDate.toString() + " - " + endDate.toString());
    }
    else {
        QDate currentDate = beginDate;
        switch(groupBy) {
        case GroupBy::Week:
            currentDate = currentDate.addDays(-currentDate.dayOfWeek() + 1);
            break;
        case GroupBy::Month:
            currentDate.setDate(currentDate.year(), currentDate.month(), 1);
            break;
        case GroupBy::Year:
            currentDate.setDate(currentDate.year(), 1, 1);
            break;
        default:
            break;
        }

        // Initialize date groups
        while(currentDate <= endDate) {
            const QDate groupBeginDate = currentDate;
            const QDate groupEndDate = [&groupBeginDate, this](){
                QDate date = groupBeginDate;
                switch(groupBy) {
                case GroupBy::Week:
                    date = date.addDays(6); // last day of the week
                    break;
                case GroupBy::Month:
                    date = date.addMonths(1).addDays(-1); // last day of the month
                    break;
                case GroupBy::Year:
                    date = date.addYears(1).addDays(-1); // last day of the year
                    break;
                default:
                    break;
                }
                return date;
            }();

            dateGroups.push_back({groupBeginDate, groupEndDate});
            if(groupBeginDate == groupEndDate) {
                categories.append(groupBeginDate.toString());
            }
            else {
                categories.append(groupBeginDate.toString() + " - " + groupEndDate.toString());
            }

            currentDate = groupEndDate.addDays(1);
        }
    }

    float maxTime = 0.f;
    auto* barSeries = new QBarSeries;
    auto it = appData.constBegin();
    while(it != appData.constEnd()) {
        auto* barSet = new QBarSet(it.key());
        if(groupBy == GroupBy::None) {
            int seconds = 0;
            for(const auto& dateRange : appData.value(it.key())) {
                seconds += dateRange.first.secsTo(dateRange.second);
            }
            const float hours = static_cast<float>(seconds) / 3600.f; // TODO: change to hours instead of minutes (hours as float)
            maxTime = qMax(maxTime, hours);
            barSet->append(hours);
        }
        else {
            const auto dateRanges = appData.value(it.key());
            auto dateRangesIt = dateRanges.constBegin();
            // For each date group try to find app usage date ranges that are within the group's date range
            for(int i = 0; i < dateGroups.size(); ++i) {
                int seconds = 0;
                while(dateRangesIt != dateRanges.constEnd()) {
                    const QDate dateRangeBeginDate = dateRangesIt->first.date();
                    const QDate dateRangeEndDate = dateRangesIt->second.date();
                    // Date range is completely inside the date group
                    if(dateRangeBeginDate >= dateGroups[i].first && dateRangeEndDate <= dateGroups[i].second) {
                        seconds += dateRangesIt->first.secsTo(dateRangesIt->second);
                        ++dateRangesIt;
                    }
                    // First part of the date range is inside the date group, while the second part is in the next date group
                    else if(dateRangeBeginDate >= dateGroups[i].first && dateRangeEndDate > dateGroups[i].second
                            && (i == dateGroups.size() - 1 || (i < dateGroups.size() - 1 && dateRangeBeginDate < dateGroups[i + 1].first))) {
                        seconds += dateRangesIt->first.secsTo(QDateTime(dateGroups[i].second.addDays(1), QTime(0, 0)));
                        break;
                    }
                    // Second part of the date range is inside the date group, while the first part is in the previous date group
                    else if(dateRangeBeginDate < dateGroups[i].first && dateRangeEndDate <= dateGroups[i].second) {
                        seconds += QDateTime(dateGroups[i].first, QTime(0, 0)).secsTo(dateRangesIt->second);
                        ++dateRangesIt;
                    }
                    // Current date range is completely outside of the date group, break and go to the next date group
                    else {
                        break;
                    }
                }

                const float hours = static_cast<float>(seconds) / 3600.f;;
                maxTime = qMax(maxTime, hours);
                barSet->append(hours);
            }
        }

        barSeries->append(barSet);
        ++it;
    }

    // Create axis
    auto* xAxis = new QBarCategoryAxis;
    xAxis->append(categories);
    auto* yAxis = new QValueAxis;
    yAxis->setRange(0, maxTime);

    // Initialize chart
    chart = new QChart;
    chart->setTitle("Application time");
    chart->addSeries(barSeries);
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    barSeries->attachAxis(xAxis);
    barSeries->attachAxis(yAxis);

    auto* oldChart = chartView->chart();
    chartView->setChart(chart);
    delete oldChart;
}
