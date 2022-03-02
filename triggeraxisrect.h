#ifndef TRIGGERAXISRECT_H
#define TRIGGERAXISRECT_H

#include "gametrigger.h"
//平行于坐标轴的矩形碰撞器
class TriggerAxisRect : public GameTrigger {
    QSize tsize;//碰撞器大小
public:
    using GameTrigger::GameTrigger;
    void SetSize(const QSize& nsize) {
        tsize = nsize;
    }
protected:
    bool OverRange() override;
public:
    bool IsCollide(GameTrigger *obj) override;
public:
    bool intersects(float cx, float cy, float radius) {
        auto rect = FactRect();
        float left = x - rect.width(),  top = y - rect.height();
        float right = x + rect.width(), bottom = y + rect.height();
        float closestX = (cx < left ? left : (cx > right ? right : cx));
        float closestY = (cy < top ? top : (cy > bottom ? bottom : cy));
        float dx = closestX - cx;
        float dy = closestY - cy;
        return ( dx * dx + dy * dy ) <= radius * radius;
    }
private:
    const QRect FactRect() {
        return QRect(QPoint(x - tsize.width() / 2, y - tsize.height() / 2), tsize);
    }
};

#endif // TRIGGERAXISRECT_H
