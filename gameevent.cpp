#include "gameevent.h"

GameEvent::GameEvent(Event id): QEvent(Transfrom(id)) {
}

QEvent::Type GameEvent::Transfrom(Event ev) {
    static int typelist[QEvent::MaxUser - QEvent::User] = {0};
    int& val = typelist[int(ev)];
    if(val) {
        return QEvent::Type(val);
    } else {
        return QEvent::Type(val = registerEventType(QEvent::User + 1 + int(ev)));
    }
}
