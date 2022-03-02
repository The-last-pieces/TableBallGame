#ifndef TRIGGERPOINT_H
#define TRIGGERPOINT_H

#include "gametrigger.h"

class TriggerPoint : public GameTrigger {
public:
    using GameTrigger::GameTrigger;
protected:
    bool OverRange() override;
public:
    bool IsCollide(GameTrigger *obj) override;
};

#endif // TRIGGERPOINT_H
