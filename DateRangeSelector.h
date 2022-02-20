#ifndef DATERANGESELECTOR_H
#define DATERANGESELECTOR_H

#include <QWidget>

class QDateEdit;

class DateRangeSelector : public QWidget
{
    Q_OBJECT

public:
    DateRangeSelector(QWidget* parent = nullptr);

private:
    QDateEdit* fromDateEdit;
    QDateEdit* toDateEdit;
};

#endif // DATERANGESELECTOR_H
