#pragma once

#include <QObject>

namespace PagesModelEnums {
    Q_NAMESPACE

    enum Status : int {
        Uncategorized = 0,
        Bad,
        Decent,
        Good
    };

    Q_ENUM_NS(Status)
}
