#ifndef DATERANGESELECTOR_H
#define DATERANGESELECTOR_H

#include <QWidget>
#include <QDate>

class QDateEdit;

class DateRangeSelector : public QWidget
{
    Q_OBJECT

public:
    DateRangeSelector(QWidget* parent = nullptr);

signals:
    void dateChanged(QDate beginDate, QDate endDate);

private slots:
    void onBeginDateChanged(QDate newDate);
    void onEndDateChanged(QDate newDate);
    void onTodayPresetClicked();
    void onYesterdayPresetClicked();
    void onThisWeekPresetClicked();
    void onThisMonthPresetClicked();
    void onThisYearPresetClicked();

private:
    QDateEdit* beginDateEdit;
    QDateEdit* endDateEdit;
    bool dateChangedUsingPreset{false};
};

#endif // DATERANGESELECTOR_H
