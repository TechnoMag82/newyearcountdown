#include "mycalendar.h"

MyCalendar::MyCalendar()
{
}

time_t MyCalendar::dateToTimeT(int month, int day, int year) {
    std::tm tmp = tm();
    tmp.tm_mday = day;
    tmp.tm_mon = month - 1;
    tmp.tm_year = year;
    tmp.tm_hour = 0;
    tmp.tm_min = 0;
    tmp.tm_sec = 0;
    return mktime(&tmp);
}

QString MyCalendar::leftToNY()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    time_t newYear = dateToTimeT(01, 01, ltm->tm_year + 1);
    double sec = difftime(newYear, now);
    uint leftDays = static_cast<uint>(sec / secondsInDay);
    uint diffSecs = sec - (leftDays * secondsInDay);
    uint leftHours = diffSecs / secondsInHour;
    uint remainingSeconds = diffSecs - (leftHours * secondsInHour);
    uint leftMinutes = remainingSeconds / secondsInMinute;
    uint leftSeconds = remainingSeconds - (leftMinutes * secondsInMinute);
    isTimeToBell = leftMinutes == 0 && leftSeconds == 0;
    return timeStr.arg(leftDays)
            .arg(leftHours)
            .arg(leftMinutes, 2, 10, QChar('0'))
            .arg(leftSeconds, 2, 10, QChar('0'));
}
