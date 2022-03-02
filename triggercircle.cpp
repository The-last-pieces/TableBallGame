#include "triggercircle.h"
#include "triggerpoint.h"
#include "triggeraxisrect.h"
bool TriggerCircle::OverRange() {
    auto range = bondobj->OnRect();
    if(x - radius < range.left()) {
        bondobj->AddDirect(Direct::left);
    }
    if(x + radius > range.right()) {
        bondobj->AddDirect(Direct::right);
    }
    if(y - radius < range.top() ) {
        bondobj->AddDirect(Direct::up);
    }
    if(y + radius > range.bottom()) {
        bondobj->AddDirect(Direct::down);
    }
    return bondobj->NowDirect();
}

bool TriggerCircle::IsCollide(GameTrigger *obj) {
    if(dynamic_cast<TriggerPoint *>(obj)) {
        auto ptr = dynamic_cast<TriggerPoint *>(obj);
        return CalDistance(this, ptr) < radius;
    }
    if(dynamic_cast<TriggerAxisRect *>(obj)) {
        auto ptr = dynamic_cast<TriggerAxisRect *>(obj);
        return ptr->intersects(x, y, radius);
    }
    if(dynamic_cast<TriggerCircle *>(obj)) {
        auto ptr = dynamic_cast<TriggerCircle *>(obj);
        return CalDistance(this, ptr) < radius + ptr->radius;
    }
    return false;
}
