#ifndef TRIGGERCIRCLE_H
#define TRIGGERCIRCLE_H

#include "gametrigger.h"

class TriggerCircle : public GameTrigger {
    double radius = 0;
public:
    using GameTrigger::GameTrigger;
    void SetRadius(double nr) {
        radius = nr;
    }
protected:
    bool OverRange() override;
public:
    bool IsCollide(GameTrigger *obj) override;
};

#endif // TRIGGERCIRCLE_H
