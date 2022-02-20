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
    auto* fromLabel = new QLabel("From:");
    fromLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fromDateEdit = new QDateEdit(QDate::currentDate());

    auto* toLabel = new QLabel("To:");
    toLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    toDateEdit = new QDateEdit(QDate::currentDate());

    auto* todayPresetButton = new QPushButton("Today");
    auto* yesterdayPresetButton = new QPushButton("Yesterday");
    auto* thisWeekPresetButton = new QPushButton("This Week");
    auto* thisMonthPresetButton = new QPushButton("This Month");
    auto* thisYearPresetButton = new QPushButton("This Year");

    // Initialize date edits layout
    auto* dateEditsLayout = new QHBoxLayout;
    dateEditsLayout->addWidget(fromLabel);
    dateEditsLayout->addWidget(fromDateEdit);
    dateEditsLayout->addWidget(toLabel);
    dateEditsLayout->addWidget(toDateEdit);

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

    connect(todayPresetButton, SIGNAL(clicked()), this, SLOT(onTodayPresetClicked()));
    connect(yesterdayPresetButton, SIGNAL(clicked()), this, SLOT(onYesterdayPresetClicked()));
    connect(thisWeekPresetButton, SIGNAL(clicked()), this, SLOT(onThisWeekPresetClicked()));
    connect(thisMonthPresetButton, SIGNAL(clicked()), this, SLOT(onThisMonthPresetClicked()));
    connect(thisYearPresetButton, SIGNAL(clicked()), this, SLOT(onThisYearPresetClicked()));
}

void DateRangeSelector::onTodayPresetClicked()
{
    fromDateEdit->setDate(QDate::currentDate());
    toDateEdit->setDate(QDate::currentDate());
}

void DateRangeSelector::onYesterdayPresetClicked()
{
    const QDate yesterdayDate = QDate::currentDate().addDays(-1);
    fromDateEdit->setDate(yesterdayDate);
    toDateEdit->setDate(yesterdayDate);
}

void DateRangeSelector::onThisWeekPresetClicked()
{
    const QDate weekStartDate = [](){
        QDate date = QDate::currentDate();
        date = date.addDays(-date.dayOfWeek() + 1);
        return date;
    }();
    fromDateEdit->setDate(weekStartDate);
    toDateEdit->setDate(QDate::currentDate());
}

void DateRangeSelector::onThisMonthPresetClicked()
{
    const QDate monthStartDate = [](){
        QDate date = QDate::currentDate();
        date.setDate(date.year(), date.month(), 1);
        return date;
    }();
    fromDateEdit->setDate(monthStartDate);
    toDateEdit->setDate(QDate::currentDate());
}

void DateRangeSelector::onThisYearPresetClicked()
{
    const QDate yearStartDate = [](){
        QDate date = QDate::currentDate();
        date.setDate(date.year(), 1, 1);
        return date;
    }();
    fromDateEdit->setDate(yearStartDate);
    toDateEdit->setDate(QDate::currentDate());
}
