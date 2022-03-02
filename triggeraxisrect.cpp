#include "triggeraxisrect.h"
#include "triggercircle.h"
#include "triggerpoint.h"

bool TriggerAxisRect::OverRange() {
    auto fact = FactRect();
    return bondobj->OnRect().intersected(fact) != fact;
}

bool TriggerAxisRect::IsCollide(GameTrigger *obj) {
    if(dynamic_cast<TriggerPoint *>(obj)) {
        //点在矩形内部
        return FactRect().contains(obj->GetPos());
    }
    if(dynamic_cast<TriggerAxisRect *>(obj)) {
        //两个矩形有交集
        auto ptr = dynamic_cast<TriggerAxisRect *>(obj);
        return FactRect().intersects(ptr->FactRect());
    }
    if(dynamic_cast<TriggerCircle *>(obj)) {
        auto ptr = dynamic_cast<TriggerCircle *>(obj);
        return ptr->IsCollide(this);//调用圆形碰撞器的判断函数
    }
    return false;
}
