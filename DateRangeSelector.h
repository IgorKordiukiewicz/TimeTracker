#ifndef DATERANGESELECTOR_H
#define DATERANGESELECTOR_H

#include <QWidget>

class QDateEdit;

class DateRangeSelector : public QWidget
{
    Q_OBJECT

public:
    DateRangeSelector(QWidget* parent = nullptr);

private slots:
    void onTodayPresetClicked();
    void onYesterdayPresetClicked();
    void onThisWeekPresetClicked();
    void onThisMonthPresetClicked();
    void onThisYearPresetClicked();

private:
    QDateEdit* fromDateEdit;
    QDateEdit* toDateEdit;
};

#endif // DATERANGESELECTOR_H
