#include "gamemoveobject.h"
//#include "circletrigger.h"

void GameMoveObject::SmoothMove(double vel, double angle, bool relatively) {
    this->x += vel * cos(angle + face_angle * relatively) / fps;
    this->y += vel * sin(angle + face_angle * relatively) / fps;
}

void GameMoveObject::SmoothRotate(double angle_v) {
    this->face_angle += angle_v / fps;
}

void GameMoveObject::EveryFrame() {
    GameViewObject::EveryFrame();
    TrimAngle();//保证转角在0~2*PI之间
}

void GameMoveObject::SetXY(double x, double y) {
    this->x = x, this->y = y;
}

const QPointF GameMoveObject::GetXY() {
    return {x, y};
}

void GameMoveObject::UpdateTrigger() {
    GameViewObject::UpdateTrigger();
}

void GameMoveObject::TrimAngle() {
    static const double towPi = 2 * PI;
    while(face_angle > PI) {
        face_angle -= towPi;
    }
    while(face_angle < -PI) {
        face_angle += towPi;
    }
}
