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

    QDate getBeginDate() const;
    QDate getEndDate() const;

signals:
    void dateChanged(QDate beginDate, QDate endDate);

public slots:
    void update();

private slots:
    void onBeginDateChanged(QDate newDate);
    void onEndDateChanged(QDate newDate);
    void onTodayPresetClicked();
    void onYesterdayPresetClicked();
    void onThisWeekPresetClicked();
    void onThisMonthPresetClicked();
    void onThisYearPresetClicked();

private:
    void handlePresetClicked(const QDate& beginDate, const QDate& endDate);

private:
    QDateEdit* beginDateEdit;
    QDateEdit* endDateEdit;
    bool dateChangedUsingPreset{false};

    QDate lastDate;
};

#endif // DATERANGESELECTOR_H
