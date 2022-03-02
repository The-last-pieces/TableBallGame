#include "triggerpoint.h"
#include "triggercircle.h"
#include "triggeraxisrect.h"

bool TriggerPoint::OverRange() {
    return bondobj->OnRect().contains(QPoint(x, y));
}

bool TriggerPoint::IsCollide(GameTrigger *obj) {
    if(dynamic_cast<TriggerPoint *>(obj)) {
        return CalDistance(this, obj) < 3;//小于3像素默认碰撞
    }
    if(dynamic_cast<TriggerAxisRect *>(obj)) {
        auto ptr = dynamic_cast<TriggerAxisRect *>(obj);
        return ptr->IsCollide(this);
    }
    if(dynamic_cast<TriggerCircle *>(obj)) {
        auto ptr = dynamic_cast<TriggerCircle *>(obj);
        return ptr->IsCollide(this);
    }
    return false;
}
