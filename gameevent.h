#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <QApplication>
#include <QEvent>

enum class Event {
    flush,
    join
};

class GameEvent : public QEvent {
public:
    GameEvent(Event id);
    using QEvent::QEvent;
public:
    static QEvent::Type Transfrom(Event ev);
};

#define ETYPE(e) GameEvent::Transfrom(Event::e)

#endif // GAMEEVENT_H
