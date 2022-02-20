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
    void dateChanged(QDate fromDate, QDate toDate);

private slots:
    void onFromDateChanged(QDate newDate);
    void onToDateChanged(QDate newDate);
    void onTodayPresetClicked();
    void onYesterdayPresetClicked();
    void onThisWeekPresetClicked();
    void onThisMonthPresetClicked();
    void onThisYearPresetClicked();

private:
    QDateEdit* fromDateEdit;
    QDateEdit* toDateEdit;
    bool dateChangedUsingPreset{false};
};

#endif // DATERANGESELECTOR_H
