#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QVector>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include "gameevent.h"
#include "gamewindow.h"

class GameObject : public QObject {
    Q_OBJECT
private:
    friend class GameWindow;
private:
    static QVector<GameObject *>objlist;
    static long long startstamp;
    static QTimer * gamecontrol;
protected:
    static GameWindow * onwindow;
public:
    const static int fps = 200;
    const static int limit = 1000 / fps;
    static const double PI;
    static const double J_TO_H;//角度值*J_to_H=弧度制
protected:
    static bool onrun;
private:
    bool hasdel = false;
    GameObject * p_object = nullptr;
    GameWindow * p_window = nullptr;
public:
    GameObject * parentgame() {
        return p_object;
    }
    GameWindow * parentwindow() {
        return p_window;
    }
    const QRect OnRect();
public:
    explicit GameObject(GameObject *parent);
    explicit GameObject(GameWindow *parent = onwindow);
    virtual ~GameObject();
    void Delete();
protected:
    bool event(QEvent *event) override;
protected:
    virtual void EveryFrame() = 0;//每个游戏循环的行为
    virtual void Initial() {}; //加入游戏循环前的行为
protected:
    void RecordCost();
    void EndRecord();
public:
    static long long MsStamp();
private:
    static void GameLoop();
    static void StartFrom(GameWindow *window);
    static void EndGame();
public:
    template<typename T>
    static QVector<T *> FindAll() {
        QVector<T *>res;
        for(auto ptr : objlist) {
            auto temp = dynamic_cast<T *>(ptr);
            if(temp) {
                res.push_back(temp);
            }
        }
        return res;
    }
    static bool InLoop(GameObject *ptr) {
        return objlist.count(ptr) && !ptr->hasdel;
    }
};

#endif // GAMEOBJECT_H
