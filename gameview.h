#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QLabel>
#include <QPainter>
#include "gameobject.h"

class GameView : public GameObject {
protected:
    QLabel * view;
    double r_angle = 0;//视图的偏移角度
    QSize fixsize = {1, 1}; //视图的固定尺寸
    QPointF pos;//视图的中心坐标(相对游戏中)
private:
    void TrimPos();
    void DelLabel();
protected:
    void Initial() override;
    const QImage LoadImageByName(const QString& name);//从特定路径导入图片
    void SwitchImage(const QImage& img);//根据当前的坐标,转角,尺寸,自适应地切换当前Pixmap
    //这个类只需要提供图像处理的接口
    void EveryFrame() override;
    //子类不需要重写EveryFrame,而是Paint行为
    virtual void Paint() = 0;//提供一个可绘制的QImage指针
public:
    GameView(GameObject * gameparent = nullptr, GameWindow * parent = onwindow);
    virtual ~GameView();
    void SetPosition(double x, double y);//设置对象位置,参数为游戏内坐标,1:10
    void Rotate(double angle);//设置r_angle,顺时针旋转的角度值(角度制)
    void SetSize(const QSize& size);//设置显示的尺寸
    const QLabel& ConstRef() {
        return *view;
    }
    void SetAttribute(Qt::WidgetAttribute type, bool on = false) {
        this->view->setAttribute(type, on);
    }
    static const QImage MakeBlankImage(const QSize& size) {
        auto temp = QImage(size, QImage::Format::Format_ARGB32);
        temp.fill(Qt::transparent);
        return temp;
    }
};

#endif // GAMEVIEW_H
