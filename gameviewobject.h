#ifndef GAMEVIEWOBJECT_H
#define GAMEVIEWOBJECT_H

#include "gameobject.h"
#include "gameview.h"

class GameTrigger;
enum class Direct;
class GameViewObject : public GameObject {
    friend class GameTrigger;
protected:
    double x = 0, y = 0;
    double face_angle = 0;
    GameTrigger * trigger = nullptr;//绑定碰撞器
    GameView * view = nullptr;//绑定一个静态视图类
    int collidedirect = 0;//前4位记录碰撞方向
protected:
    virtual void UpdateTrigger(); //更新碰撞器参数
    virtual void UpdateView();//更新视图参数
    virtual void EveryFrame() override;
protected:
    void Initial() override;
    virtual GameView * InitView() {
        return nullptr;
    };
    virtual GameTrigger *  InitTrigger() {
        return nullptr;
    };
public:
    using GameObject::GameObject;
    virtual ~GameViewObject() {}
    //这两个函数管理子对象
    void SetView(GameView * nview);
    void SetTrigger(GameTrigger * ntrigger);

    //碰撞模块
private:
    bool isignore = false;
public:
    void ignore() {
        isignore = true;
    }
protected:
    virtual void OverRange(const QSize &) {}//越界时调用的函数,默认无
    void KCollide(GameViewObject *obj) {
        if(isignore) {
            isignore = false;
        } else {
            Collide(obj);
        }
    }
    virtual void Collide(GameViewObject * ) {} //碰撞时调用的函数,默认无

public:
    void AddDirect(Direct dir);
    bool HasDirect(Direct dir);
    int NowDirect();
protected:
    double TransAngle(double now, double axis) {
        return  2 * axis - now;
    }
};

#endif // GAMEVIEWOBJECT_H
