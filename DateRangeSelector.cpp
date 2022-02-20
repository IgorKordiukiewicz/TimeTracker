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
    auto* thisMonthPresetButton = new QPushButton("This Month");
    auto* thisYearPresetButton = new QPushButton("This Year");
    auto* allTimePresetButton = new QPushButton("All Time");

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
    datePresetsLayout->addWidget(thisMonthPresetButton);
    datePresetsLayout->addWidget(thisYearPresetButton);
    datePresetsLayout->addWidget(allTimePresetButton);

    // Initialize main layout
    auto* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(dateEditsLayout);
    mainLayout->addLayout(datePresetsLayout);
    setLayout(mainLayout);
}
