#include "gamestaticimage.h"
#include <QPainter>
void GameStaticImage::SetImage(const QString &name) {
    img = LoadImageByName(name);
    SetSize(img.size());
}

void GameStaticImage::Paint() {
    SwitchImage(img);
}
