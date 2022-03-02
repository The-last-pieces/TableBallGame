#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "gameobject.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDir>

GameWindow * GameWindow::instance = nullptr;
QRect GameWindow::bound;

GameWindow::GameWindow(QWidget *parent, const QString& name)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow) {
    ui->setupUi(this);
    ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);
    setWindowTitle(name);
    GameObject::StartFrom(this);
    //设置背景
    QPixmap backimg = QPixmap(QDir::currentPath() + "/imgs/background.png");
    if(!backimg.isNull()) {
        backimg = backimg.scaled(1400, 700);
        this->setFixedSize(backimg.size());
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(backimg));
        this->setPalette(palette);
    }
}

void GameWindow::UpdateFps(double costms) {
    static long long lastcall = 0;
    if(GameObject::MsStamp() - lastcall > 1000) {
        lastcall = GameObject::MsStamp();
        setWindowTitle(appname + " fps:" + QString::number(1000 / costms));
    }
}

void GameWindow::mouseReleaseEvent(QMouseEvent* mevent) {
    Q_EMIT this->SendMouseReleasePoint(mevent->pos());
}

void GameWindow::keyPressEvent(QKeyEvent *keyevent) {
    Q_EMIT this->SendKeyDown(Qt::Key(keyevent->key()));
}
void GameWindow::keyReleaseEvent(QKeyEvent *keyevent) {
    Q_EMIT this->SendKeyUp(Qt::Key(keyevent->key()));
}
void GameWindow::mouseMoveEvent(QMouseEvent*mevent) {
    Q_EMIT this->SendMouseMovePoint(mevent->pos());
}

GameWindow::~GameWindow() {
    GameObject::EndGame();
    delete ui;
}

void GameWindow::SetBound(int x, int y, int width, int height) {
    bound = QRect(x, y, width, height);
}

GameWindow * GameWindow::InitGame(const QString &name, QWidget *parent) {
    instance = new GameWindow(parent, name);
    if(bound == QRect()) {
        SetBound(0, 0, instance->width(), instance->height());
    }
    return instance;
}

