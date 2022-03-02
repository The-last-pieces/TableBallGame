#include "gametextview.h"

void GameTextView::Paint() {
    QImage temp = MakeStringImage(txt);
    SetSize(temp.size());
    SwitchImage(temp);
}
