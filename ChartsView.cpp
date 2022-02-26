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
#include <QPushButton>
#include "SettingsDialog.h"
#include <QDataStream>
#include <QFile>
#include <random>
#include "Utils.h"

ChartsView::ChartsView(TimeTracker* timeTracker, QWidget* parent)
    : QWidget(parent)
    , timeTracker(timeTracker)
{
    chart = new QChart;

    chartView = new QChartView{ chart };
    chartView->setRenderHint(QPainter::Antialiasing);

    auto* groupByLabel = new QLabel{ "Group by:" };

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

    auto* settingsButton = new QPushButton{ "Settings" };

    auto* optionsLayout = new QHBoxLayout;
    optionsLayout->setAlignment(Qt::AlignLeft);
    optionsLayout->addWidget(chartDataTypeComboBox);
    optionsLayout->addWidget(groupByLabel);
    optionsLayout->addWidget(groupByComboBox);
    optionsLayout->addWidget(settingsButton);

    auto* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(optionsLayout);
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);

    connect(groupByComboBox, &QComboBox::currentTextChanged, this, &ChartsView::onGroupByComboBoxTextChanged);
    connect(chartDataTypeComboBox, &QComboBox::currentTextChanged, this, &ChartsView::onChartDataTypeComboBoxTextChanged);
    connect(settingsButton, &QPushButton::clicked, this, &ChartsView::onSettingsButtonClicked);
    connect(timeTracker, &TimeTracker::newAppTracked, this, &ChartsView::onNewAppTracked);
}

ChartsView::~ChartsView()
{
    saveSettings();
}

void ChartsView::saveSettings()
{
    saveCategoriesSettings();
    saveAppsSettings();
}

void ChartsView::loadSettings()
{
    loadCategoriesSettings();
    loadAppsSettings();
}

void ChartsView::saveAppsSettings()
{
    QFile file{ "appSettings" };

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file 'appSettings'";
        return;
    }

    QDataStream stream{ &file };
    stream.setVersion(QDataStream::Qt_6_2);

    // TEMPORARY
    ApplicationsSettings appsSettingsTEMP;
    appsSettingsTEMP.insert("App 1", ApplicationSettings{"App 1", "", QColor(200, 0, 0)});
    appsSettingsTEMP.insert("App 2", ApplicationSettings{"App 2", "", QColor(0, 200, 0)});
    stream << appsSettingsTEMP;
    file.flush();
    file.close();
}

void ChartsView::saveCategoriesSettings()
{
    QFile file{ "categoriesSettings" };

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file 'categoriesSettings'";
        return;
    }

    QDataStream stream{ &file };
    stream.setVersion(QDataStream::Qt_6_2);

    stream << categoriesSettings;
    file.flush();
    file.close();
}

void ChartsView::loadAppsSettings()
{
    QFile file{ "appSettings" };

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file 'appSettings'";
        return;
    }

    QDataStream stream{ &file };
    stream.setVersion(QDataStream::Qt_6_2);

    stream >> appsSettings;
    file.close();

    // Update apps settings if it is not complete (probably should never happen outside of debugging after data file is reset, TODO: remove later?)
    const auto& appNames{ timeTracker->getData().keys() };
    for(const QString& appName : appNames) {
        if(!appsSettings.contains(appName)) {
            onNewAppTracked(appName);
        }
    }
}

void ChartsView::loadCategoriesSettings()
{
    QFile file{ "categoriesSettings" };

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file 'categoriesSettings'";
        return;
    }

    QDataStream stream{ &file };
    stream.setVersion(QDataStream::Qt_6_2);

    stream >> categoriesSettings;
    file.close();
}

void ChartsView::setDateRange(const QDate& beginDate, const QDate& endDate)
{
    this->beginDate = beginDate;
    this->endDate = endDate;
    updateData();
}

void ChartsView::onGroupByComboBoxTextChanged(const QString& text)
{
    auto it{ groupByNames.find(text) };
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

void ChartsView::onSettingsButtonClicked()
{
    SettingsDialog settingsDialog{ appsSettings, categoriesSettings };
    if(settingsDialog.exec()) {
        // appsSettings and categoriesSettings are updated only if user confirmed changes made in settings dialog
        appsSettings = settingsDialog.getAppsSettings();
        categoriesSettings = settingsDialog.getCategoriesSettings();
        updateData();
    }
}

void ChartsView::onNewAppTracked(const QString& appName)
{
    ApplicationSettings appSettings;
    appSettings.displayName = appName;
    appSettings.categoryName = "";
    appSettings.chartColor = Utils::generateRandomColorForChart();
    appsSettings.insert(appName, std::move(appSettings));
}

void ChartsView::onBarSetHovered(bool status, int, QBarSet* barSet)
{
    // Make visible when hovered and insvisible otherwise
    if(status) {
        barSet->setLabelColor(Qt::black);
    }
    else {
        barSet->setLabelColor(Qt::transparent);
    }
}

void ChartsView::updateData()
{
    appData = timeTracker->getDataInRange(beginDate, endDate);

    if(chartDataType == ChartDataType::Categories) {
        TimeTracker::AppData newAppData;
        // Create new app data with key for every category
        for(auto it{ categoriesSettings.constBegin() }; it != categoriesSettings.constEnd(); ++it) {
            newAppData.insert(it.key(), QVector<TimeTracker::DateTimeRange>());
        }

        for(auto it{ appData.constBegin() }; it != appData.constEnd(); ++it) {
            // If app has a category assigned, add its date time ranges to the category's date time ranges
            const QString categoryName{ appsSettings[it.key()].categoryName };
            if(!categoryName.isEmpty()) {
                newAppData[categoryName].append(it.value());
            }
        }

        // Date ranges have to be sorted in increasing order to work properly
        for(auto it{ newAppData.begin() }; it != newAppData.end(); ++it) {
            std::sort(it.value().begin(), it.value().end(), [](const auto& range1, const auto& range2) {
                return range1.first < range2.first;
            });
        }

        appData = std::move(newAppData);
    }
    else if(chartDataType == ChartDataType::Activity) {
        // Merge all apps date time ranges into one
        TimeTracker::AppData newAppData;
        auto dateRangesIt = newAppData.insert("Activity", QVector<TimeTracker::DateTimeRange>());
        for(auto it{ appData.constBegin() }; it != appData.constEnd(); ++it) {
            for(TimeTracker::DateTimeRange dateRange : appData.value(it.key())) {
                dateRangesIt->push_back(std::move(dateRange));
            }
        }

        // Date ranges have to be sorted in increasing order to work properly
        std::sort(dateRangesIt->begin(), dateRangesIt->end(), [](const auto& range1, const auto& range2){
            return range1.first < range2.first;
        });

        appData = std::move(newAppData);
    }

    // Show only 'maxBarSets' count of apps
    if(appData.size() > maxBarSets) {
        // Count total sum of time for each app
        QVector<QPair<QString, int>> totalSecondsPerApp;
        for(auto it{ appData.constBegin() }; it != appData.constEnd(); ++it) {
            totalSecondsPerApp.push_back({it.key(), 0});
            for(const TimeTracker::DateTimeRange& dateRange : appData.value(it.key())) {
                totalSecondsPerApp.last().second += dateRange.first.secsTo(dateRange.second);
            }
        }

        // Sort in descending order
        std::sort(totalSecondsPerApp.begin(), totalSecondsPerApp.end(), [](const auto& p1, const auto& p2) {
           return p1.second > p2.second;
        });

        // Only include 'maxBarSets' count of apps with most time
        TimeTracker::AppData newAppData;
        for(int i{ 0 }; i < maxBarSets; ++i) {
            const QString appName{ totalSecondsPerApp[i].first };
            newAppData.insert(appName, appData.value(appName));
        }
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
        QDate currentDate{ beginDate };
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
            const QDate groupBeginDate{ currentDate };
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

    float maxTime{ 0.f };
    auto* barSeries = new QBarSeries;
    auto it{ appData.constBegin() };
    while(it != appData.constEnd()) {
        // Get the bar chart name and color
        const auto [name, color] = [it, this]() -> QPair<QString, QColor> {
            switch(chartDataType) {
            case ChartDataType::Applications: {
                // Try to find the name & color in the apps settings
                if(auto appsSettingsIt{ appsSettings.find(it.key()) }; appsSettingsIt != appsSettings.end()) {
                    return { appsSettingsIt->displayName, appsSettingsIt->chartColor };
                }
                break;
            }
            case ChartDataType::Categories: {
                // Try to find the color in the categories settings
                if(auto categoriesSettingsIt{ categoriesSettings.find(it.key()) }; categoriesSettingsIt != categoriesSettings.end()) {
                    return {it.key(), categoriesSettingsIt->chartColor };
                }
                break;
            }
            case ChartDataType::Activity: {
                return { it.key(), activityChartColor };
            }
            }
            return { it.key(), QColor() };
        }();

        auto* barSet = new QBarSet(name);
        barSet->setColor(color);
        barSet->setLabelColor(Qt::transparent); // labels should be initially invisible

        if(groupBy == GroupBy::None) {
            int seconds{ 0 };
            for(const TimeTracker::DateTimeRange& dateRange : appData.value(it.key())) {
                seconds += dateRange.first.secsTo(dateRange.second);
            }
            const float hours{ static_cast<float>(seconds) / 3600.f };
            maxTime = qMax(maxTime, hours);
            barSet->append(hours);
            //barSet->setLabel(Utils::getTimeAsString(seconds));
        }
        else {
            const QVector<TimeTracker::DateTimeRange> dateRanges{ appData.value(it.key()) };
            auto dateRangesIt = dateRanges.constBegin();
            // For each date group try to find app usage date ranges that are within the group's date range
            for(int i{ 0 }; i < dateGroups.size(); ++i) {
                int seconds{ 0 };
                while(dateRangesIt != dateRanges.constEnd()) {
                    const QDate dateRangeBeginDate{ dateRangesIt->first.date() };
                    const QDate dateRangeEndDate{ dateRangesIt->second.date() };
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

                const float hours{ static_cast<float>(seconds) / 3600.f };
                maxTime = qMax(maxTime, hours);
                barSet->append(hours);
                //barSet->setLabel(Utils::getTimeAsString(seconds));
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
    chart->addSeries(barSeries);
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    barSeries->attachAxis(xAxis);
    barSeries->attachAxis(yAxis);
    barSeries->setLabelsFormat("@value h");
    barSeries->setLabelsPrecision(1);

    barSeries->setLabelsVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    connect(barSeries, &QBarSeries::hovered, this, &ChartsView::onBarSetHovered);

    QChart* oldChart{ chartView->chart() };
    chartView->setChart(chart);
    oldChart->deleteLater();
}
