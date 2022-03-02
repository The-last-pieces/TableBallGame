#include "gametrigger.h"

GameTrigger::GameTrigger(GameViewObject * parent): GameObject(parent) {
    bondobj = parent;
}

void GameTrigger::EveryFrame() {
    auto triggers = FindAll<GameTrigger>();
    int size = triggers.size();
    if(size <= 1) {
        return;
    }
    for(auto obj : triggers) {
        //与自身互异,且这一帧没有和其碰撞过
        if(this != obj && !this->collidebefore.count(obj) && this->IsCollide(obj)) {
            //两个对象相互碰撞
            this->CollideWith(obj);
            obj->CollideWith(this);
            //下一帧不再检测
            obj->collidebefore.push_back(this);
        }
    }
    this->collidebefore.clear();
}

void GameTrigger::CollideWith(GameTrigger *obj) {
    if(bondobj) {
        this->bondobj->KCollide(obj->bondobj);
    }
}

double GameTrigger::CalDistance(GameTrigger *A, GameTrigger *B) {
    double offx = A->x - B->x, offy = A->y - B->y;
    return sqrt(offx * offx + offy * offy);
}
