#ifndef GAMESTATICIMAGE_H
#define GAMESTATICIMAGE_H

#include "gameview.h"

class GameStaticImage : public GameView {
private:
    QImage img;
protected:
    virtual void Paint()override;
public:
    using GameView::GameView;
    virtual ~GameStaticImage() {}
    void SetImage(const QString& name);
};

#endif // GAMESTATICIMAGE_H
