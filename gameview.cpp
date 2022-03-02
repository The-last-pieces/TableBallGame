#include "gameview.h"
#include <QPainter>
#include <QDir>
#include <QMessageBox>
GameView::GameView(GameObject * gameparent, GameWindow * parent): GameObject(gameparent), view(new QLabel(parent)) {
    view->setFixedSize({1, 1});
    connect(view, &QObject::destroyed, this, &GameView::DelLabel);
    //析构后必须置为nullptr
}

GameView::~GameView() {
    if(view) {
        view->deleteLater();
        view = nullptr;
    }
}

void GameView::Initial() {
    view->show();
}

void GameView::EveryFrame() {
    //更新视图
    this->Paint();
    view->update();
}

void GameView::DelLabel() {
    view = nullptr;
}

void GameView::SetPosition(double x, double y) {
    pos = {x, y};
}
void GameView::SetSize(const QSize& nsize) {
    fixsize = nsize;
}
void GameView::Rotate(double angle) {
    r_angle = angle;
}

void GameView::TrimPos() {
    if(view) {
        int px = pos.x() - view->width() / 2;
        int py = pos.y() - view->height() / 2;
        view->move(px, py);
    }
}

void GameView::SwitchImage(const QImage &img) {
    //首先调整图片尺寸
    if(!view || img.isNull()) {
        return;
    }
    QPixmap fact = QPixmap::fromImage(fixsize != img.size() ? img.scaled(fixsize) : img);
    //添加旋转变换
    QMatrix trans;
    trans.rotate(r_angle);
    fact = fact.transformed(trans, Qt::TransformationMode::SmoothTransformation);
    //设置view为图片的实际大小
    view->setFixedSize(fact.size());
    //切换视图
    view->setPixmap(fact);
    //移动到当前坐标位置
    TrimPos();
    view->show();
}

const QImage GameView::LoadImageByName(const QString& name) {
    QString path = QDir::currentPath() + R"(/imgs/)" + name;
    const QImage temp = QImage(path);
    if(temp.isNull()) {
        QMessageBox::information(nullptr, "error", "图片资源加载失败:" + path, QMessageBox::YesAll);
        QApplication::quit();
    }
    return temp;
}
