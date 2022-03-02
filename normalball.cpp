#include "normalball.h"
#include "triggercircle.h"

QVector<QPoint> NormalBall::holes;

const auto mul = [](const QPointF & A, const QPointF & B)->double {
    return A.x() * B.x() + A.y() * B.y();
};
const auto angle = [](const QPointF & obj)->double {
    return atan2(obj.y(), obj.x());
};

void NormalBall::UpdateTrigger() {
    GameMoveObject::UpdateTrigger();
    dynamic_cast<TriggerCircle*>(trigger)->SetRadius(radius);
}

void NormalBall::UpdateView() {
    GameMoveObject::UpdateView();
}

GameTrigger* NormalBall::InitTrigger() {
    return new TriggerCircle(this);
}

GameView * NormalBall::InitView() {
    auto img = new GameStaticImage(this);
    //"ball_{{id}}.png"
    img->SetImage(QString::asprintf("ball_%d", ballid));
    img->SetSize(size);
    img->SetAttribute(Qt::WA_TransparentForMouseEvents, true);
    return img;
}

void NormalBall::EveryFrame() {
    GameMoveObject::EveryFrame();
    //施加速度
    SmoothMove(line_v, vel_angle);//沿着当前方向移动
    SmoothRotate(angle_v);//以当前角速度旋转
    //施加摩擦力
    line_v -= std::min(line_v, line_f / fps);
    angle_v -= std::min(angle_v, angle_f / fps);
    if(isstatic()) {
        //stop();
    }
    JudgeHole();
}

void NormalBall::stop() {
    auto balls = FindAll<NormalBall>();
    for(auto& ball : balls) {
        if(!ball->isstatic()) {
            return;
        }
    }
    //全部静止了,则发送轮换击球事件
}

void NormalBall::OverRange(const QSize &) {
    Reflect();
}

void NormalBall::Collide(GameViewObject* obj) {
    NormalBall * ptr = dynamic_cast<NormalBall*>(obj);
    if(ptr) {
        ///判断是否趋近
        //获取相对速度的矢量
        QPointF v1 = QPointF(this->line_v * cos(this->vel_angle), this->line_v * sin(this->vel_angle));
        QPointF v2 = QPointF(ptr->line_v * cos(ptr->vel_angle), ptr->line_v * sin(ptr->vel_angle));
        QPointF dv = v2 - v1;
        //获取相对距离的矢量
        QPointF dp = QPointF(ptr->x, ptr->y) - QPointF(this->x, this->y);
        //判断内积是否为正
        if(mul(dv, dp) > 0) {
            //趋向离开,不需要碰撞
            return;
        }
        ///处理线速度变换
        double mod = sqrt(mul(dp, dp));
        if(!mod) {
            return;
        }
        QPointF axis_x = dp / mod;
        QPointF axis_y = QPointF(-axis_x.y(), axis_x.x());
        double vx1 = mul(v1, axis_x), vy1 = mul(v1, axis_y);
        double vx2 = mul(v2, axis_x), vy2 = mul(v2, axis_y);
        std::swap(vx1, vx2);
        v1 = {vx1, vy1};
        v2 = {vx2, vy2};
        this->line_v = sqrt(mul(v1, v1));
        ptr->line_v = sqrt(mul(v2, v2));
        this->vel_angle = angle(axis_x) + angle(v1);
        ptr->vel_angle = angle(axis_x) + angle(v2);
        ///处理角速度变化
        std::swap(this->angle_v, ptr->angle_v);
        ///防止重复碰撞
        obj->ignore();
    }
}

void NormalBall::Reflect() {
    if(HasDirect(Direct::left) || HasDirect(Direct::right)) {
        vel_angle = TransAngle(vel_angle, PI / 2);
    }
    if(HasDirect(Direct::up) || HasDirect(Direct::down)) {
        vel_angle = TransAngle(vel_angle, 0);
    }
}


void NormalBall::JudgeHole() {
    for(auto pos : holes) {
        QPointF vct = {x - pos.x(), y - pos.y()};
        //qDebug() << vct;
        if(mul(vct, vct) < pow(40, 2)) {
            Q_EMIT IntoHole();
            return;
        }
    }
}
