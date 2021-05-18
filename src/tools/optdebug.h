#ifndef OPTDEBUG_H
#define OPTDEBUG_H

#include <QDebug>

template<typename T>
QDebug &operator << (QDebug &d, const std::optional<T>& opt) {
    if (opt.has_value()) {
        d << opt.value();
    } else {
        d << "none";
    }
    return d;
}


#endif // OPTDEBUG_H
