#pragma once

#include <QMutex>

#include "export.hpp"

class OTR_COMPAT_EXPORT mutex
{
    mutable QRecursiveMutex inner;

public:
    explicit mutex() {}

    mutex& operator=(const mutex& rhs) { return *this; }

    mutex(const mutex& datum) {}

    QRecursiveMutex* operator&() const noexcept { return &inner; }

    operator QRecursiveMutex*() const noexcept { return &inner; }

    QRecursiveMutex* operator->() const noexcept { return &inner; }
};
