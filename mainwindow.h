#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QBitmap>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QImage>

#include "updatelefttimethread.h"
#include "Settings/settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    private slots:
        void customMenuRequested(QPoint pos);
        void menuClicked(bool checked);
        void skinClicked(bool checked);
        void display(QPixmap *treePixmap);

    private:
        Settings *settings = nullptr;
        QPoint winPosition;
        QLabel *treeLabel = nullptr;
        QMenu *mainMenu = nullptr;
        UpdateLeftTimeThread *updateThread = nullptr;

        void findSkins();
        void applySkin(QString pathToSkin);
        void updateWindowFlags(bool onTop);
        void initContextMenu();

    protected:
        virtual void mousePressEvent(QMouseEvent* pe)
        {
            winPosition = pe->pos();
        };
        virtual void mouseMoveEvent(QMouseEvent* pe)
        {
            move(pe->globalPos() - winPosition);
            settings->windowPosition = pe->globalPos() - winPosition;
        };

};

#endif // MAINWINDOW_H
