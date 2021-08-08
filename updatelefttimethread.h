#ifndef UPDATELEFTTIMETHREAD_H
#define UPDATELEFTTIMETHREAD_H

#include <QObject>
#include <QThread>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QPixmap>
#include <QPixmapCache>
#include <QDirIterator>
#include <QList>
#include <QSound>
#include <QFile>
#include <QTextStream>
#include <QLinearGradient>

#include "mycalendar.h"

class UpdateLeftTimeThread : public QThread
{
    Q_OBJECT
    public:
        UpdateLeftTimeThread();
        ~UpdateLeftTimeThread();
        void applySkin(QString pathToSkin);
        void setUpdateDelay(unsigned long delay);
        void setBellEnable(bool enabled);
        void run();

    signals:
        void drawTree(QPixmap*);

    public slots:
        void updateTime();

    private:
        QImage *image = nullptr;
        MyCalendar *calendar = nullptr;
        QPixmap *treePixmap = nullptr;
        QString timeString;
        QFont *timeFont = nullptr;
        QFontMetrics *timeFontMetrics = nullptr;
        QLinearGradient *fontGradient = nullptr;

        QList<QImage*> *imageList = nullptr;
        int imageIterator = 0;
        uint countdownSleep = 0;
        uint imageSleep = 0;
        bool isActive = false;
        bool isBellEnabled = false;
        unsigned long updateDelay = 500;
        void fillCache(QString pathToSkin);
        void loadSkinConfig(QString pathToSkin);
        void selectNextImage();
        void setupFont(QString fontName, int fontSize);
        QPen getPen();

        template <typename T>
        void releaseObject(T &pointer);
};

#endif // UPDATELEFTTIMETHREAD_H
