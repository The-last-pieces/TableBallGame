#include "normalball.h"
#include <QApplication>

const int width = 1400, height = 700;

void drawback() {
    QImage img(width, height, QImage::Format_ARGB32);
    //边框为棕色
    img.fill(QColor(116, 54, 68));
    QPainter painter(&img);
    //台面为蓝色
    painter.setBrush(QColor(12, 142, 172));
    int first_step = 40;
    painter.drawRect(first_step, first_step, width - 2 * first_step, height - 2 * first_step);
    //小边框为深蓝色
    //painter.setBrush(QColor(20, 105, 135));
    //int second_step = 10;
    //painter.drawRect(second_step, second_step, width - 2 * first_step, height - 2 * first_step);
    //球袋为黑色
    painter.setBrush(QColor(0, 0, 0));
    int radius = first_step / 4 * 3;
    QVector<int>xpos = {first_step / 2, width / 2 - radius - first_step, width - first_step * 2};
    for(int x : xpos) {
        NormalBall::AddHole({x + radius, first_step / 2 + radius});
        painter.drawEllipse( x, first_step / 2,  2 * radius, 2 * radius);
        NormalBall::AddHole({ x + radius, height - 2 * first_step + radius});
        painter.drawEllipse( x, height - 2 * first_step, 2 * radius, 2 * radius);
    }
    //保存
    GameWindow::SetBound(first_step, first_step, width - 2 * first_step, height - 2 * first_step);
    img.save("imgs/background.png");
}

void InitMotherBall(GameWindow* window) {
    int force = 2000;
    auto ptr = new NormalBall(window);
    ptr->SetXY(width / 3, height / 2);
    QObject::connect(window, &GameWindow::SendMouseReleasePoint, [ = ](QPoint pos)->void {
        if(!NormalBall::AllStop() || !GameObject::InLoop(ptr))return;
        QPointF self = ptr->GetXY();
        ptr->ImmediateForce(force, GameObject::PI +  atan2(self.y() - pos.y(), self.x() - pos.x()));
    });
    class Predict : public GameView {
    private:
        bool needupdate = false;
        QPointF start;
        double angle = 0;
        void Paint()override {
            //从起点开始绘制虚线,在终点绘制虚球
            if(!NormalBall::AllStop()) {
                view->hide();
                return;
            }
            if(!needupdate) {
                return;
            }
            needupdate = false;
            auto end = getEnd();
            QRectF rect = getRect(start, end);
            QSize size = QSize(rect.width() + 2 * NormalBall::radius, rect.height() + 2 * NormalBall::radius);
            //qDebug() << start << end;
            this->SetSize(size);
            QPoint lefttop = QPoint(rect.left() - NormalBall::radius, rect.top() - NormalBall::radius);
            QPointF center = rect.center();
            this->SetPosition(center.x(), center.y());
            QImage img(size, QImage::Format_ARGB32);
            img.fill(Qt::transparent);
            //qDebug() << rect;
            if(img.isNull()) {
                return;
            }
            QPainter painter(&img);
            //虚线
            painter.setPen(Qt::PenStyle::SolidLine);
            painter.setPen(Qt::white);
            painter.drawLine(start - lefttop, end - lefttop);
            //虚球
            painter.drawEllipse(end - lefttop, NormalBall::radius, NormalBall::radius);
            this->SwitchImage(img);
        }
        const QRectF getRect(QPointF start, QPointF end) {
            QPointF center = (start + end) / 2;
            QSizeF size = QSizeF(
                              abs(start.x() - end.x()),
                              abs(start.y() - end.y())
                          );
            return QRectF(
                       center - QPointF(size.width() / 2, size.height() / 2),
                       size + QSizeF{2, 2}
                   );
        }
        const QPointF getEnd() {
            auto balls = FindAll<NormalBall>();
            QPointF end = start;
            while(inRect(end)) {
                end += QPointF(cos(angle), sin(angle));
                for(const auto& ball : balls) {
                    auto pos = ball->GetXY();
                    if(pos == start) {
                        continue;
                    }
                    double dis = pow(pos.x() - end.x(), 2) + pow(pos.y() - end.y(), 2);
                    if(sqrt(dis) < 2 * NormalBall::radius) {
                        return end;
                    }
                }
            }
            return end;
        }
        bool inRect(QPointF end) {
            QRectF range = OnRect();
            int x = end.x(), y = end.y(), radius = NormalBall::radius;
            if(x - radius < range.left()) {
                return false;
            }
            if(x + radius > range.right()) {
                return false;
            }
            if(y - radius < range.top() ) {
                return false;
            }
            if(y + radius > range.bottom()) {
                return false;
            }
            return true;
        }
    protected:
        void Initial()override {
            GameView::Initial();
            view->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        }
    public:
        using GameView::GameView;
        void setInfo(QPointF pos, double dir) {
            start = pos;
            angle = dir;
            needupdate = true;
        }
    };
    auto line = new Predict();
    QObject::connect(window, &GameWindow::SendMouseMovePoint, [line, ptr](QPoint pos)->void{
        if(!GameObject::InLoop(ptr))return;
        QPointF self = ptr->GetXY();
        line->setInfo(self, GameObject::PI + atan2(self.y() - pos.y(), self.x() - pos.x()));
    });
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ///绘制背景
    drawback();
    ///创建窗口
    GameWindow * window = GameWindow::InitGame("桌球");
    window->showNormal();
    ///初始化球体位置
    //母球
    InitMotherBall(window);
    //子球
    int r = NormalBall::radius;
    int basex = width * 2 / 3, basey =  height / 2;
    int id = 1;
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j <= i; ++j) {
            double x = basex + sqrt(3) * r * i;
            double y = basey - i * r + 2 * j * r;
            auto ptr = new NormalBall(window);
            ptr->SetInfo(id);
            ptr->SetXY(x, y);
            ++id;
            QObject::connect(ptr, &NormalBall::IntoHole, [ptr]()->void{ptr->Delete();});
        }
    }
    return a.exec();
}
