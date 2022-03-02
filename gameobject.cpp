#include "gameobject.h"
#include <QDesktopWidget>
#include <QPaintEvent>

QVector<GameObject *>GameObject::objlist;
long long GameObject::startstamp = GameObject::MsStamp();
QTimer * GameObject::gamecontrol = nullptr;
GameWindow * GameObject::onwindow = nullptr;
const double GameObject::PI = 4 * atan(1);
const double GameObject::J_TO_H = PI / 180;
bool GameObject::onrun = false;

GameObject::GameObject(GameObject *parent) : QObject(parent) {
    p_object = parent;
    QApplication::postEvent(this, new GameEvent(Event::join));
    //延迟加入游戏循环
}

GameObject::GameObject(GameWindow *parent): QObject(parent) {
    p_window = parent;
    //无子对象的游戏对象以GameWindow作为父对象
    QApplication::postEvent(this, new GameEvent(Event::join));
}

GameObject::~GameObject() {
    objlist.removeAll(this);
    //qDebug() << "del" << this;
    //从游戏循环中移除
}

bool GameObject::event(QEvent *event) {
    if(event->type() == ETYPE(flush)) {
        RecordCost();
        EveryFrame();
        EndRecord();
        return true;
    } else if(event->type() == ETYPE(join)) {
        this->Initial();
        objlist.append(this);
        return true;
    } else {
        return QObject::event(event);
    }
}

void GameObject::RecordCost() {
    if(this == objlist.front()) {
        startstamp = MsStamp();
        onrun = true;
    }
}

void GameObject::EndRecord() {
    if(this == objlist.back()) {
        onrun = false;
    }
}

const QRect GameObject::OnRect() {
    if(p_window) {
        return p_window->GetBound();
    }
    if(onwindow) {
        return onwindow->GetBound();
    }
    if(p_object) {
        return p_object->OnRect();
    } else {
        return QApplication::desktop()->rect();
    }
}

void GameObject::Delete() {
    if(!hasdel) {
        hasdel = true;
        deleteLater();
    }
}

long long GameObject::MsStamp() {
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void GameObject::GameLoop() {
    //qDebug() << "size" << objlist.size();
    if(onrun) {
        //return;
    }
    if(onwindow) {
        onwindow->UpdateFps(MsStamp() - startstamp);
    }
    for(auto ptr : objlist) {
        QApplication::postEvent(ptr, new GameEvent(ETYPE(flush)));
    }
}

void GameObject::StartFrom(GameWindow *window) {
    if(window) {
        if(!gamecontrol) {
            onwindow = window;
            gamecontrol = new QTimer();
            gamecontrol->setTimerType(Qt::TimerType::PreciseTimer);
            connect(gamecontrol, &QTimer::timeout, &GameObject::GameLoop);
            gamecontrol->start(limit);
        }
    } else {
        throw "游戏不能运行在null窗口上";
    }
}

void GameObject::EndGame() {
    onwindow = nullptr;
    delete gamecontrol;
    gamecontrol = nullptr;
    for(auto ptr : objlist) {
        if(!ptr->parent()) {
            ptr->Delete();
        }
    }
    objlist.clear();
}
