#pragma once
#include <QtGui/QPainter>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <vector>
#include <cmath>
#include "Common.h"

class Shape
{
protected:
    QPoint m_pos; // позиция фигуры
    int m_w; // ширина в пикселях
    int m_h;  // высота в пикселях
    QColor m_color; // текущий цвет
    bool m_selected{ false }; // флаг выделения
public:
    Shape(QPoint p, int w, int h, QColor c) : m_pos(p), m_w(w), m_h(h), m_color(c) {}
    virtual ~Shape() = default;

    virtual void draw(QPainter& p) = 0; // фигура сама себя рисует
    virtual bool contains(const QPoint& pt) const = 0; // попал ли курсор внутрь фигуры

    virtual QRect bounds() const { return QRect(m_pos.x() - m_w/2, m_pos.y() - m_h/2, m_w, m_h); } // вычисляем прямоугольник по центру

    void setSelected(bool s) { m_selected = s; } // стави выделение
    bool isSelected() const { return m_selected; } // читаем выделение

    virtual void moveBy(int dx, int dy, const QRect& workArea)
    {
        m_pos += QPoint(dx, dy); // смещаем точку центра
        clampTo(workArea); // не даем выйти за рамКи программы
    }

    virtual void resizeBy(int dw, int dh, const QRect& workArea)
    {
        m_w = std::max(kMinSize, m_w + dw); // ограничиваем мин ширину
        m_h = std::max(kMinSize, m_h + dh); // ограничиваем мин высоту
        clampTo(workArea); // изменяю позицию если вылезли
    }

    void setColor(const QColor& c) { m_color = c; } // смена цвета

protected:
    virtual void clampTo(const QRect& work)
    {
        //  если выходит, смещаем внутрь
        m_w = std::min(m_w, work.width()); // ширина не больше
        m_h = std::min(m_h, work.height()); // высота не больше
        QRect b = bounds(); // границы
        if (b.left() < work.left()) m_pos.setX(work.left() + m_w/2); // если вылезли слева
        if (b.right() > work.right()) m_pos.setX(work.right() - m_w/2); // справа
        if (b.top() < work.top()) m_pos.setY(work.top() + m_h/2); // сверху
        if (b.bottom() > work.bottom()) m_pos.setY(work.bottom() - m_h/2); // снизу
    }
};

class Circle : public Shape
{
public:
    Circle(QPoint p, int size, QColor c) : Shape(p, size, size, c) {} // круг
    void draw(QPainter& p) override
    {
        p.setBrush(m_color); // цвет
        p.setPen(m_selected ? QPen(Qt::red, 2) : QPen(Qt::black, 1)); // если выделен крас
        p.drawEllipse(m_pos, m_w/2, m_h/2); // рисуем окружность по центру
    }
    bool contains(const QPoint& pt) const override
    {
        int dx = pt.x() - m_pos.x(); // смещение по X
        int dy = pt.y() - m_pos.y(); // смещение по Y
        int r = m_w / 2; // радиус круга
        return dx*dx + dy*dy <= r*r; // проверка
    }
};

class RectShape : public Shape
{
public:
    RectShape(QPoint p, int w, int h, QColor c) : Shape(p, w, h, c) {} // прямоугольник
    void draw(QPainter& p) override
    {
        p.setBrush(m_color); // цвет заливки
        p.setPen(m_selected ? QPen(Qt::red, 2) : QPen(Qt::black, 1)); // выделение
        p.drawRect(bounds()); // рисуем по вычисленным границам
    }
    bool contains(const QPoint& pt) const override
    {
        return bounds().contains(pt); // проверка
    }
};

class EllipseShape : public Shape
{
public:
    EllipseShape(QPoint p, int w, int h, QColor c) : Shape(p, w, h, c) {} // эллипс
    void draw(QPainter& p) override
    {
        p.setBrush(m_color); // заливка
        p.setPen(m_selected ? QPen(Qt::red, 2) : QPen(Qt::black, 1)); // контур
        p.drawEllipse(bounds()); // рисование
    }
    bool contains(const QPoint& pt) const override
    {
        double rx = m_w / 2.0; // полуось X
        double ry = m_h / 2.0; // полуось Y
        double dx = (pt.x() - m_pos.x()) / rx; // смещение
        double dy = (pt.y() - m_pos.y()) / ry; // смещение
        return dx*dx + dy*dy <= 1.0; // точка внутри эллипса
    }
};

class TriangleShape : public Shape
{
public:
    TriangleShape(QPoint p, int w, int h, QColor c) : Shape(p, w, h, c) {} // треугольник
    void draw(QPainter& p) override
    {
        p.setBrush(m_color); // заливка
        p.setPen(m_selected ? QPen(Qt::red, 2) : QPen(Qt::black, 1)); // контур
        QPoint a(m_pos.x(), m_pos.y() - m_h/2); // верхняя вершина
        QPoint b(m_pos.x() - m_w/2, m_pos.y() + m_h/2); // нижняя левая
        QPoint c(m_pos.x() + m_w/2, m_pos.y() + m_h/2); // нижняя правая
        QPolygon poly; poly<<a<<b<<c; p.drawPolygon(poly);
    }
    bool contains(const QPoint& pt) const override
    {
        // определяем попадание внутрь треуг
        QPoint a(m_pos.x(), m_pos.y() - m_h/2);
        QPoint b(m_pos.x() - m_w/2, m_pos.y() + m_h/2);
        QPoint c(m_pos.x() + m_w/2, m_pos.y() + m_h/2);
        auto sign = [](const QPoint& p1, const QPoint& p2, const QPoint& p3){ return (p1.x() - p3.x())*(p2.y()-p3.y()) - (p2.x()-p3.x())*(p1.y()-p3.y()); }; // вычисление знака
        bool b1 = sign(pt, a, b) < 0; // сравниваем относительно ребер
        bool b2 = sign(pt, b, c) < 0;
        bool b3 = sign(pt, c, a) < 0;
        return (b1 == b2) && (b2 == b3); // по одну сторону всех ребер
    }
};

// линия хранит две точки
class LineShape : public Shape
{
    QPoint m_start; // начало
    QPoint m_end; // конец
public:
    LineShape(QPoint start, QPoint end, QColor c) : Shape((start+end)/2, std::abs(end.x()-start.x()), std::abs(end.y()-start.y()), c), m_start(start), m_end(end) {} // вычисляем центр и размеры

    void setEnd(const QPoint& end)
    {
        m_end = end; // обновляем конечную точку
        updateMetrics(); // пересчитываем центр
    }

    void draw(QPainter& p) override
    {
        p.setPen(m_selected ? QPen(Qt::red, 3) : QPen(m_color, 3));
        p.drawLine(m_start, m_end); // линия между точками
    }

    bool contains(const QPoint& pt) const override
    {
        // расстояние от точки до отрезка – если ближе чем 5 попадание
        QPoint ap = pt - m_start; // вектор от начала до точки
        QPoint ab = m_end - m_start; // вектор отрезка
        double ab2 = ab.x()*ab.x() + ab.y()*ab.y(); // квадрат длины отрезка
        if (ab2 == 0) return false;
        double t = (ap.x()*ab.x() + ap.y()*ab.y()) / ab2;
        t = std::max(0.0, std::min(1.0, t));
        QPoint closest(m_start.x() + int(ab.x()*t), m_start.y() + int(ab.y()*t)); // ближайшая точка на отрезке
        int dx = pt.x() - closest.x();
        int dy = pt.y() - closest.y();
        return (dx*dx + dy*dy) <= 25; // 5 радиус
    }

    QRect bounds() const override
    {
        // прямоугольник минимальный
        int left = std::min(m_start.x(), m_end.x());
        int top = std::min(m_start.y(), m_end.y());
        int right = std::max(m_start.x(), m_end.x());
        int bottom = std::max(m_start.y(), m_end.y());
        return QRect(left, top, right-left, bottom-top);
    }

    void moveBy(int dx, int dy, const QRect& workArea) override
    {
        m_start += QPoint(dx, dy); // смещаем обе точки
        m_end += QPoint(dx, dy);
        clampTo(workArea); // ограничиваем внутри
        updateMetrics(); // пересчитываем центр и размеры
    }

    void resizeBy(int dw, int dh, const QRect& workArea) override
    {
        // Масштабируем длину отрезка
        QPoint vec = m_end - m_start; // текущий вектор
        double len = std::sqrt(double(vec.x()*vec.x() + vec.y()*vec.y())); // длина
        if (len < 1) return;
        double delta = (dw + dh) / 2.0; // среднее изменение
        double newLen = std::max(5.0, len + delta); // ограничение минимальной длины
        double scale = newLen / len; // коэффициент
        QPoint mid = (m_start + m_end)/2; // середина
        QPoint half = QPoint(int(vec.x()*scale/2), int(vec.y()*scale/2)); // половина нового вектора
        m_start = mid - half; // новая стартовая
        m_end = mid + half; // новая конечная
        clampTo(workArea); // корректируем границы
        updateMetrics();
    }

protected:
    void clampTo(const QRect& work) override
    {
        // Ограничиваем каждую точку
        auto clampPoint = [&work](QPoint& p){
            p.setX(std::max(work.left(), std::min(work.right(), p.x()))); // clamp по X
            p.setY(std::max(work.top(), std::min(work.bottom(), p.y()))); // clamp по Y
        };
        clampPoint(m_start); // начало
        clampPoint(m_end); // конец
    }

private:
    void updateMetrics()
    {
        m_pos = (m_start + m_end)/2; // центр как среднее
        m_w = std::abs(m_end.x()-m_start.x()); // ширина
        m_h = std::abs(m_end.y()-m_start.y()); // высота
    }
};

class ShapeStorage
{
    std::vector<Shape*> m_data;
public:
    ~ShapeStorage() { clear(); } // очищаем память
    void clear() { for(auto s: m_data) delete s; m_data.clear(); } // удаляем фигуры
    void add(Shape* s) { m_data.push_back(s); } // добавление новой фигуры
    void removeSelected()
    {
        // удаляем те фигуры которые выделены
        for(auto it = m_data.begin(); it != m_data.end();) {
            if((*it)->isSelected()) { delete *it; it = m_data.erase(it); } else ++it; }
    }
    auto begin() { return m_data.begin(); }
    auto end() { return m_data.end(); }
};
