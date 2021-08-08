#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QString>

#include <ctime>

class MyCalendar
{
    public:
        MyCalendar();
        QString leftToNY();
        bool isTimeToBell = false;

    private:
        const int secondsInHour = 60 * 60;
        const int secondsInMinute = 60;
        const int secondsInDay = secondsInHour * 24;
        const QString timeStr = "%1 %2:%3:%4";

        time_t dateToTimeT(int month, int day, int year);
};

#endif // MYCALENDAR_H
