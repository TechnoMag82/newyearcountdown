#include "updatelefttimethread.h"

UpdateLeftTimeThread::UpdateLeftTimeThread()
{
    calendar = new MyCalendar();
    imageList = new QList<QImage*>();
    moveToThread(this);
}

UpdateLeftTimeThread::~UpdateLeftTimeThread()
{
    delete calendar;
    delete imageList;
    delete image;
    delete timeFontMetrics;
}

void UpdateLeftTimeThread::run()
{
    while (true) {
        if (isActive) {
            QThread::msleep(500);
            countdownSleep += 500;
            if (imageList->size() > 0) {
                imageSleep += 500;
            }
            updateTime();
        }
    }
}

void UpdateLeftTimeThread::updateTime()
{
    if (!isActive)
        return;
    if (imageList->size() > 0 && imageSleep >= updateDelay) {
        imageSleep = 0;
        selectNextImage();
    }
    if (countdownSleep >= 1000) {
        timeString = calendar->leftToNY();
        countdownSleep = 0;
        if (isBellEnabled && calendar->isTimeToBell) {
            QSound::play(":/sound/chime.wav");
        }
    }
    treePixmap->fill(Qt::transparent);
    QPainter *painter = new QPainter(treePixmap);
    QRect rect = QRect(treePixmap->width() / 2 - image->width() / 2,
                       0,
                       image->width(),
                       image->height());
    painter->drawImage(rect, *image, image->rect());
    painter->setPen(getPen());
    painter->setFont(*timeFont);
    painter->drawText(treePixmap->width() / 2 - timeFontMetrics->width(timeString) / 2,
                      image->height() + timeFontMetrics->ascent(),
                      timeString);
    delete painter;
    emit drawTree(treePixmap);
}

void UpdateLeftTimeThread::applySkin(QString pathToSkin)
{
    isActive = false;
    timeString = calendar->leftToNY();
    releaseObject(image);
    imageList->clear();
    releaseObject(fontGradient);
    setupFont("Arial", 21);
    if (pathToSkin.isEmpty()) {
        image = new QImage(":/skin/tree1.png");
    } else {
        fillCache(pathToSkin);
        imageIterator = 0;
        if (imageList->size() > 0) {
            image = imageList->at(imageIterator);
            loadSkinConfig(pathToSkin);
        } else {
            image = new QImage(":/skin/tree1.png");
        }
    }
    releaseObject(treePixmap);
    int fontWidth = timeFontMetrics->width(timeString);
    if (fontWidth > image->width()) {
        treePixmap = new QPixmap(fontWidth,
                             image->height() + timeFontMetrics->ascent());
    } else {
        treePixmap = new QPixmap(image->width(),
                             image->height() + timeFontMetrics->ascent());
    }
    isActive = true;
    updateTime();
}

void UpdateLeftTimeThread::setUpdateDelay(unsigned long delay)
{
    this->updateDelay = delay;
}

void UpdateLeftTimeThread::setBellEnable(bool enabled)
{
    isBellEnabled = enabled;
}

void UpdateLeftTimeThread::fillCache(QString pathToSkin)
{
    qDeleteAll(*imageList);
    QDirIterator it(pathToSkin,
                        QStringList() << "*.png",
                        QDir::Files);
    while (it.hasNext()) {
        imageList->append(new QImage(it.next()));
    }
}

void UpdateLeftTimeThread::loadSkinConfig(QString pathToSkin)
{
    QFile skinConfig(pathToSkin + "/skin.config");
    if (skinConfig.open(QIODevice::ReadOnly | QIODevice::Text) == true) {
        QTextStream config(&skinConfig);
        config.setCodec("UTF-8");
        QString temp;
        QString font = "Arial";
        int fontSize = 21;
        while (config.atEnd() != true)
        {
            temp = config.readLine(0);
            if (temp.startsWith("font: ")) {
                 font = temp.remove(0, 6);
                 if (font.isEmpty()) {
                     font = "Arial";
                 }
            }
            if (temp.startsWith("font-size: ")) {
                 QString size = temp.remove(0, 11);
                 bool ok;
                 fontSize = size.toInt(&ok, 10);
                 if (!ok) {
                     fontSize = 21;
                 }
            }
            if (temp.startsWith("font-color: ")) {
                 QString color = temp.remove(0, 12);
                 QStringList colors = color.split(' ');
                 int colorCount = colors.size();
                 fontGradient = new QLinearGradient(0, 0, image->width(), 0);
                 qreal pos = 0;
                 for (int i = 0; i < colorCount; i++) {
                    fontGradient->setColorAt(pos, QColor(colors.at(i)));
                    pos += 0.5;
                 }
            }
        }
        skinConfig.close();
        setupFont(font, fontSize);
    }
}

void UpdateLeftTimeThread::selectNextImage()
{
    ++imageIterator;
    if (imageIterator >= imageList->size()) {
        imageIterator = 0;
    }
    image = imageList->at(imageIterator);
}

void UpdateLeftTimeThread::setupFont(QString fontName, int fontSize)
{
    releaseObject(timeFontMetrics);
    if (timeFont == nullptr) {
        timeFont = new QFont(fontName, fontSize, QFont::Normal);
    } else {
        timeFont->setFamily(fontName);
        timeFont->setPointSize(fontSize);
    }
    timeFontMetrics = new QFontMetrics(*timeFont);
}

QPen UpdateLeftTimeThread::getPen()
{
    if (fontGradient != nullptr) {
        return QPen(*fontGradient, 6);
    } else {
        return QPen(Qt::white, 6);
    }
}

template<typename T>
void UpdateLeftTimeThread::releaseObject(T &pointer)
{
    if (pointer != nullptr) {
        delete pointer;
        pointer = nullptr;
    }
}
