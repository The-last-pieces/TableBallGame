#include "gameviewobject.h"
#include "gametrigger.h"

void GameViewObject::Initial() {
    SetView(InitView());
    SetTrigger(InitTrigger());
}

void GameViewObject::EveryFrame() {
    //同步数据到视图
    if(view) {
        UpdateView();
    }
    //同步数据到碰撞器
    if(trigger) {
        UpdateTrigger();
        //检测是否越界
        collidedirect = 0;
        if(trigger->OverRange()) {
            OverRange(OnRect().size());
        }
    }
}

void GameViewObject::SetView(GameView *nview) {
    if(view) {
        view->Delete();
    }
    view = nview;
}

void GameViewObject::SetTrigger(GameTrigger *ntrigger) {
    if(trigger) {
        trigger->Delete();
    }
    trigger = ntrigger;
}

void GameViewObject::UpdateView() {
    view->SetPosition(x, y);
    view->Rotate(face_angle / J_TO_H);
}

void GameViewObject::UpdateTrigger() {
    trigger->x = x;
    trigger->y = y;
}

void GameViewObject::AddDirect(Direct dir) {
    this->collidedirect |= (int)dir;
}

bool GameViewObject::HasDirect(Direct dir) {
    return this->collidedirect & (int) dir;
}

int GameViewObject::NowDirect() {
    return collidedirect;
}
