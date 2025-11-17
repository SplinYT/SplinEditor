#pragma once
#include "Shapes.h"
#include <QtCore/QRect>

// модель не знает про графический интерфейс
class Model {
public:
    ShapeStorage& storage() { return m_storage; }
    const ShapeStorage& storage() const { return m_storage; }

    void addShape(Shape* s) { if (s) m_storage.add(s); } // добавить новую фигуру
    void removeSelected() { m_storage.removeSelected(); } // удалить все выделенные

    void clearSelection() { // снять выделение со всех фигур
        for (auto s : m_storage) s->setSelected(false); // проходим весь контейнер
    }

    void selectAt(const QPoint& pos, bool multi, bool toggle) { // логика выбора по точке
        if (!multi) clearSelection(); // если не множественный выбор – сбрасываем
        for (auto s : m_storage) {
            if (s->contains(pos)) { // курсор попал внутрь фигуры
                if (toggle && s->isSelected()) s->setSelected(false); else s->setSelected(true); // переключаем или устанавливаем
                if (!multi) break; // одиночный режим
            }
        }
    }

    void applyColorToSelected(const QColor& c) { // применяем цвет ко всем выделенным
        if (!c.isValid()) return; // пропускаем некорректный цвет
        for (auto s : m_storage) if (s->isSelected()) s->setColor(c); // обновляем цвет
    }

    void moveSelected(int dx, int dy, const QRect& work) { // двигаем выделенные
        for (auto s : m_storage) if (s->isSelected()) s->moveBy(dx, dy, work); // вызываем метод фигуры
    }

    void resizeSelected(int d, const QRect& work) { // масштабируем выделенные
        for (auto s : m_storage) if (s->isSelected()) s->resizeBy(d, d, work); // изменение размера
    }

private:
    ShapeStorage m_storage; // контейнер фигур
};
