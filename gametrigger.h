#ifndef GAMETRIGGER_H
#define GAMETRIGGER_H

#include "gameviewobject.h"

enum class Direct {
    left = 1,
    right = 2,
    up = 4,
    down = 8
};

class GameTrigger : public GameObject {
    friend class GameViewObject;
protected:
    double x = 0, y = 0;
    QVector<GameTrigger*> collidebefore;//储存本帧中碰撞过的对象,防止重复碰撞
    GameViewObject * bondobj = nullptr;//绑定的视图对象
protected:
    void EveryFrame() override;//子类不可再实现
public:
    GameTrigger(GameViewObject * parent);
    virtual ~GameTrigger() {}
    const QPoint GetPos() {
        return QPoint(x, y);
    }
private:
    void CollideWith(GameTrigger * obj);
public:
    //判断碰撞器是否发生碰撞
    virtual bool IsCollide(GameTrigger * obj) = 0;
protected:
    //判断碰撞器是否越界
    virtual bool OverRange() = 0;
    static double CalDistance(GameTrigger * A, GameTrigger * B);
};

#endif // GAMETRIGGER_H
