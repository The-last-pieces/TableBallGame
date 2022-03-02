#ifndef GAMETEXTVIEW_H
#define GAMETEXTVIEW_H

#include "gameview.h"

class GameTextView : public GameView {
    QString txt;
public:
    using GameView::GameView;
    void SetContext(const QString& str) {
        txt = str;
    }
protected:
    void Paint() override;
private:
    static const QImage MakeStringImage(const QString& msg) {
        static QFont font = QFont("Microsoft YaHei", 10);
        QSize txtsize = getTextSize(font, msg);
        QImage img = MakeBlankImage(txtsize);
        if(img.isNull()) {
            return img;
        }
        //绘图预处理
        QPainter painter(&img);
        //修改画笔颜色
        painter.setPen(Qt::GlobalColor::black);
        //设置字体
        painter.setFont(font);
        //绘制文字
        painter.drawText(img.rect(), Qt::AlignCenter, msg);
        return img;
    }
    static const QSize getTextSize(const QFont& font, const QString& msg) {
        QFontMetrics ms(font);
        QRect rect = ms.boundingRect(msg);
        return QSize(rect.width(), rect.height());
    }
};

#endif // GAMETEXTVIEW_H
