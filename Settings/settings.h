#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QPoint>

class Settings
{
    public:
        Settings();
        ~Settings();

        QString pathToSkin;
        QPoint windowPosition;
        int updateDelay;
        bool bellEachHour;
        bool onTop;
};

#endif // SETTINGS_H
