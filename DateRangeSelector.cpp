#include "DateRangeSelector.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QCalendarWidget>

DateRangeSelector::DateRangeSelector(QWidget* parent)
    : QWidget(parent)
{
    const QDate currentDate { QDate::currentDate() };
    lastDate = currentDate;

    auto* beginDateLabel = new QLabel{ "From:" };
    beginDateLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    beginDateEdit = new QDateEdit{ currentDate };
    beginDateEdit->setMaximumDate(currentDate);

    auto* endDateLabel = new QLabel{ "To:" };
    endDateLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    endDateEdit = new QDateEdit{ currentDate };
    endDateEdit->setMaximumDate(currentDate);

    auto* todayPresetButton = new QPushButton{ "Today" };
    auto* yesterdayPresetButton = new QPushButton{ "Yesterday" };
    auto* thisWeekPresetButton = new QPushButton{ "This Week" };
    auto* thisMonthPresetButton = new QPushButton{ "This Month" };
    auto* thisYearPresetButton = new QPushButton{ "This Year" };

    // Initialize date edits layout
    auto* dateEditsLayout = new QHBoxLayout;
    dateEditsLayout->addWidget(beginDateLabel);
    dateEditsLayout->addWidget(beginDateEdit);
    dateEditsLayout->addWidget(endDateLabel);
    dateEditsLayout->addWidget(endDateEdit);

    // Initialie date presets layout
    auto* datePresetsLayout = new QHBoxLayout;
    datePresetsLayout->addWidget(todayPresetButton);
    datePresetsLayout->addWidget(yesterdayPresetButton);
    datePresetsLayout->addWidget(thisWeekPresetButton);
    datePresetsLayout->addWidget(thisMonthPresetButton);
    datePresetsLayout->addWidget(thisYearPresetButton);

    // Initialize main layout
    auto* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(dateEditsLayout);
    mainLayout->addLayout(datePresetsLayout);
    setLayout(mainLayout);

    connect(beginDateEdit, &QDateEdit::dateChanged, this, &DateRangeSelector::onBeginDateChanged);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &DateRangeSelector::onEndDateChanged);
    connect(todayPresetButton, &QPushButton::clicked, this, &DateRangeSelector::onTodayPresetClicked);
    connect(yesterdayPresetButton, &QPushButton::clicked, this, &DateRangeSelector::onYesterdayPresetClicked);
    connect(thisWeekPresetButton, &QPushButton::clicked, this, &DateRangeSelector::onThisWeekPresetClicked);
    connect(thisMonthPresetButton, &QPushButton::clicked, this, &DateRangeSelector::onThisMonthPresetClicked);
    connect(thisYearPresetButton, &QPushButton::clicked, this, &DateRangeSelector::onThisYearPresetClicked);
}

void DateRangeSelector::update()
{
    const QDate currentDate { QDate::currentDate() };
    if (currentDate > lastDate) {
        beginDateEdit->setMaximumDate(currentDate);
        endDateEdit->setMaximumDate(currentDate);
        lastDate = currentDate;
    }
}

QDate DateRangeSelector::getBeginDate() const
{
    return beginDateEdit->date();
}

QDate DateRangeSelector::getEndDate() const
{
    return endDateEdit->date();
}

void DateRangeSelector::onBeginDateChanged(QDate newDate)
{
    // 'from' date can't be larger than the 'to' date
    if (beginDateEdit->date() > endDateEdit->date() && !dateChangedUsingPreset) {
        beginDateEdit->setDate(endDateEdit->date());
        return;
    }

    if(!dateChangedUsingPreset) {
        emit dateChanged(beginDateEdit->date(), endDateEdit->date());
    }
}

void DateRangeSelector::onEndDateChanged(QDate newDate)
{
    // 'to' date can't be smaller than the 'from' date
    if (endDateEdit->date() < beginDateEdit->date() && !dateChangedUsingPreset) {
        endDateEdit->setDate(beginDateEdit->date());
        return;
    }

    if(!dateChangedUsingPreset) {
        emit dateChanged(beginDateEdit->date(), endDateEdit->date());
    }
}

void DateRangeSelector::onTodayPresetClicked()
{
    const QDate currentDate{ QDate::currentDate() };
    handlePresetClicked(currentDate, currentDate);
}

void DateRangeSelector::onYesterdayPresetClicked()
{
    const QDate yesterdayDate{ QDate::currentDate().addDays(-1) };
    handlePresetClicked(yesterdayDate, yesterdayDate);
}

void DateRangeSelector::onThisWeekPresetClicked()
{
    const QDate weekStartDate = [](){
        QDate date{ QDate::currentDate() };
        date = date.addDays(-date.dayOfWeek() + 1);
        return date;
    }();
    handlePresetClicked(weekStartDate, QDate::currentDate());
}

void DateRangeSelector::onThisMonthPresetClicked()
{
    const QDate monthStartDate = [](){
        QDate date{ QDate::currentDate() };
        date.setDate(date.year(), date.month(), 1);
        return date;
    }();
    handlePresetClicked(monthStartDate, QDate::currentDate());
}

void DateRangeSelector::onThisYearPresetClicked()
{
    const QDate yearStartDate = [](){
        QDate date{ QDate::currentDate() };
        date.setDate(date.year(), 1, 1);
        return date;
    }();
    handlePresetClicked(yearStartDate, QDate::currentDate());
}

void DateRangeSelector::handlePresetClicked(const QDate& beginDate, const QDate& endDate)
{
    dateChangedUsingPreset = true;
    beginDateEdit->setDate(beginDate);
    endDateEdit->setDate(endDate);
    dateChangedUsingPreset = false;
    emit dateChanged(beginDateEdit->date(), endDateEdit->date());
}
