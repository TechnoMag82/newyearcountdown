#include "settings.h"

Settings::Settings()
{
    QSettings settings;
    pathToSkin = settings.value("skin").toString();
    windowPosition.setX(settings.value("xpos").toInt());
    windowPosition.setY(settings.value("ypos").toInt());
    updateDelay = settings.value("updateDelay", 500).toInt();
    bellEachHour = settings.value("bell", true).toBool();
    onTop = settings.value("onTop", true).toBool();
}

Settings::~Settings()
{
    QSettings settings;
    settings.setValue("skin", pathToSkin);
    settings.setValue("xpos", windowPosition.x());
    settings.setValue("ypos", windowPosition.y());
    settings.setValue("updateDelay", updateDelay);
    settings.setValue("bell", bellEachHour);
    settings.setValue("onTop", onTop);
}

