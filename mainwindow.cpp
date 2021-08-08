#include "mainwindow.h"

const QString PROGRAM_DIR = QDir::homePath() + "/.newyear/skins";

MainWindow::MainWindow()
{
    settings = new Settings();
    setAttribute(Qt::WA_TranslucentBackground);
    updateWindowFlags(settings->onTop);
    move(settings->windowPosition);

    treeLabel = new QLabel();
    treeLabel->setAttribute(Qt::WA_TranslucentBackground);
    treeLabel->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setCentralWidget(treeLabel);

    updateThread = new UpdateLeftTimeThread();
    updateThread->setUpdateDelay(settings->updateDelay);
    updateThread->setBellEnable(settings->bellEachHour);
    connect(updateThread, SIGNAL(drawTree(QPixmap*)), this, SLOT(display(QPixmap*)));
    applySkin(settings->pathToSkin);

    treeLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    initContextMenu();
    connect(treeLabel, SIGNAL(customContextMenuRequested(QPoint)),
                   SLOT(customMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    updateThread->deleteLater();
    delete treeLabel;
    delete settings;
}

void MainWindow::initContextMenu()
{
    mainMenu = new QMenu(this);
    findSkins();

    QMenu *delayMenu = new QMenu(tr("Delay"), mainMenu);

    QActionGroup *group = new QActionGroup(delayMenu);
    group->setExclusive(true);

    QAction *action = new QAction(tr("0.5 sec"), this);
    action->setObjectName("Delay1000");
    action->setActionGroup(group);
    action->setCheckable(true);
    action->setChecked(settings->updateDelay == 500);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    delayMenu->addAction(action);

    action = new QAction(tr("1 sec"), this);
    action->setObjectName("Delay1000");
    action->setActionGroup(group);
    action->setCheckable(true);
    action->setChecked(settings->updateDelay == 1000);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    delayMenu->addAction(action);

    action = new QAction(tr("1.5 sec"), this);
    action->setObjectName("Delay1500");
    action->setActionGroup(group);
    action->setCheckable(true);
    action->setChecked(settings->updateDelay == 1500);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    delayMenu->addAction(action);

    action = new QAction(tr("2 sec"), this);
    action->setObjectName("Delay2000");
    action->setActionGroup(group);
    action->setCheckable(true);
    action->setChecked(settings->updateDelay == 2000);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    delayMenu->addAction(action);

    mainMenu->addMenu(delayMenu);

    action = new QAction(tr("Bell"), this);
    action->setObjectName("Bell");
    action->setCheckable(true);
    action->setChecked(settings->bellEachHour == true);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    mainMenu->addAction(action);

    action = new QAction(tr("On top"), this);
    action->setObjectName("OnTop");
    action->setCheckable(true);
    action->setChecked(settings->onTop == true);
    connect(action, &QAction::triggered, this, &MainWindow::menuClicked);
    mainMenu->addAction(action);

    action = new QAction(tr("Exit"), this);
    mainMenu->addAction(action);
    connect(action, &QAction::triggered, this, &QMainWindow::close);
}

void MainWindow::customMenuRequested(QPoint pos) {
    mainMenu->popup(treeLabel->mapToGlobal(pos));
}

void MainWindow::menuClicked(bool checked)
{
    if (checked) {
        if (sender()->objectName().compare("Delay500") == 0) {
            settings->updateDelay = 500;
            updateThread->setUpdateDelay(500);
        }
        if (sender()->objectName().compare("Delay1000") == 0) {
            settings->updateDelay = 1000;
            updateThread->setUpdateDelay(1000);
        }
        if (sender()->objectName().compare("Delay1500") == 0) {
            settings->updateDelay = 1500;
            updateThread->setUpdateDelay(1500);
        }
        if (sender()->objectName().compare("Delay2000") == 0) {
            settings->updateDelay = 2000;
            updateThread->setUpdateDelay(2000);
        }
    }
    if (sender()->objectName().compare("Bell") == 0) {
        settings->bellEachHour = checked;
        updateThread->setBellEnable(checked);
    }
    if (sender()->objectName().compare("OnTop") == 0) {
        settings->onTop = checked;
    }
}

void MainWindow::skinClicked(bool checked)
{
    if (checked) {
        applySkin(sender()->objectName());
    }
}

void MainWindow::display(QPixmap *treePixmap)
{
    treeLabel->setPixmap(*treePixmap);
    treeLabel->setFixedWidth(treePixmap->width());
    treeLabel->setFixedHeight(treePixmap->height());
    setFixedWidth(treePixmap->width());
    setFixedHeight(treePixmap->height());
    setMask(treePixmap->mask());
}

void MainWindow::findSkins()
{
    QDir dir(PROGRAM_DIR);
    if (dir.exists(PROGRAM_DIR) == false) {
        dir.mkpath(PROGRAM_DIR);
    }
    QDirIterator it(PROGRAM_DIR,
                        QStringList() << "*",
                        QDir::NoDotAndDotDot | QDir::Dirs);
    QMenu *skinsMenu = nullptr;
    QActionGroup *group = nullptr;
    QAction *action = nullptr;
    if (it.hasNext()) {
        skinsMenu = new QMenu(tr("Skins"), mainMenu);
        group = new QActionGroup(skinsMenu);
        group->setExclusive(true);
    }
    while (it.hasNext()) {
        QString path = it.next();
        QDir skin(path);
        action = new QAction(skin.dirName(), this);
        action->setObjectName(path);
        action->setActionGroup(group);
        action->setCheckable(true);
        action->setChecked(false);
        connect(action, &QAction::triggered, this, &MainWindow::skinClicked);
        skinsMenu->addAction(action);
    }
    if (skinsMenu != nullptr) {
        mainMenu->addMenu(skinsMenu);
    }
}

void MainWindow::applySkin(QString pathToSkin)
{
    updateThread->applySkin(pathToSkin);
    if (!updateThread->isRunning()) {
        updateThread->start();
    }
    settings->pathToSkin = pathToSkin;
}

void MainWindow::updateWindowFlags(bool onTop)
{
    if (onTop) {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    }
}
