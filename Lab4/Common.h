#pragma once
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtGui/QColor>

enum class ShapeType
{
    Circle,
    Rectangle,
    Ellipse,
    Triangle,
    Line
};

static const int kDefaultSize = 60; // начальный размер
static const int kMinSize = 10; // мин размер

inline QColor DefaultFillColor(bool selected)
{
    return selected ? QColor(240, 120, 120, 160) : QColor(120, 180, 240, 160);
}