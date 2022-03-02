#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    GameWindow(QWidget *parent, const QString& name);
    ~GameWindow();
private:
    Ui::GameWindow *ui;
public:
    void UpdateFps(double costms);
signals:
    void SendKeyDown(Qt::Key);
    void SendKeyUp(Qt::Key);
    void SendMouseReleasePoint(QPoint);
    void SendMouseMovePoint(QPoint);
protected:
    void keyPressEvent(QKeyEvent*)override;
    void keyReleaseEvent(QKeyEvent*)override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void mouseMoveEvent(QMouseEvent*)override;
private:
    QString appname;
private:
    static GameWindow * instance;
    static QRect bound;
public:
    static GameWindow * InitGame(const QString& name = "2DGame_Frame", QWidget *parent = nullptr);
    static GameWindow * getInstance() {
        if(instance == nullptr) {
            throw "游戏主窗口不存在";
        }
        return instance;
    }
    static void SetBound(int x, int y, int width, int height);
    static const QRect GetBound() {
        return bound;
    }
};
#endif // GAMEWINDOW_H
