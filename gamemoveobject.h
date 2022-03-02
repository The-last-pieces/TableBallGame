#ifndef GAMEMOVEOBJECT_H
#define GAMEMOVEOBJECT_H

#include "gamestaticimage.h"
#include "gameviewobject.h"

class GameMoveObject : public GameViewObject {
public:
    void TrimAngle();
protected:
    void SmoothMove(double vel, double angle, bool relatively = true); //平滑移动
    void SmoothRotate(double angle_v);//根据角速度
    virtual void EveryFrame() override;
    virtual void UpdateTrigger() override;
public:
    using GameViewObject::GameViewObject;
    virtual ~GameMoveObject() {}
    void SetXY(double x, double y);
    void SetXY(const QPointF& pos) {
        SetXY(pos.x(), pos.y());
    }
    const QPointF GetXY();
};

#endif // GAMEMOVEOBJECT_H
