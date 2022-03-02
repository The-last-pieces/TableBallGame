#ifndef NORMALBALL_H
#define NORMALBALL_H

#include "gamemoveobject.h"

class NormalBall : public GameMoveObject {
    Q_OBJECT
public:
    using GameMoveObject::GameMoveObject;
protected:
    void Reflect();
protected:
    //virtual void CollideEvent() = 0;
    void EveryFrame() override;
protected:
    void UpdateTrigger() override;//圆形碰撞器
    void UpdateView()override;

    GameView * InitView()override;
    GameTrigger * InitTrigger()override;

    //重载球体的碰撞事件和出界事件
    void Collide(GameViewObject* obj)override;
    void OverRange(const QSize &)override;
protected:
    //virtual void InHole() = 0;//进洞事件
    void stop();//停止事件
    bool isstatic() {
        return line_v == 0 && angle_v == 0;
    }
public:
    static const int radius = 25;
private:
    //尺寸
    const QSize size = {radius * 2, radius * 2};
    //摩擦力
    const double angle_f = 10;
    const double line_f = 180;
    //速度
    double angle_v = 0;
    double line_v = 0;
    double vel_angle = 0;
private:
    int ballid = 0;//球的编号
    int belong = 0;//所属
public:
    void SetInfo(int id) {
        ballid = id;
        if(id >= 1 && id <= 7) {
            belong = 1;
        } else if(id >= 9 && id <= 15) {
            belong = 2;
        }
    }
    void ImmediateForce(double vel, double angle) {
        vel_angle = angle;
        line_v = vel;
    }
protected:
    static QVector<QPoint> holes;
public:
    static void AddHole(const QPoint& newhole) {
        holes.push_back(newhole);
    }
    static bool AllStop() {
        auto list = FindAll<NormalBall>();
        for(auto ptr : list) {
            if(!ptr->isstatic()) {
                return false;
            }
        }
        return true;
    }
private:
    void JudgeHole();
signals:
    void IntoHole();
};

#endif // NORMALBALL_H
