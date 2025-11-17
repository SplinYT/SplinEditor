#include "Controller.h"
#include "View.h"

Controller::Controller(Model* model, CanvasView* view)
    : m_model(model), m_view(view) {}

void Controller::beginMouse(const QPoint& pos, Qt::KeyboardModifiers mods)
{
    bool ctrl = mods & Qt::ControlModifier; // Ctrl
    bool shift = mods & Qt::ShiftModifier; // Shift – множественный выбор
    if (m_currentType == ShapeType::Line) { // если создаем линию
        m_tempLine = new LineShape(pos, pos, DefaultFillColor(false)); // создаем временный объект
        m_view->update(); // перерисовать
        return;
    }
    // либо выбираем, либо создаем новую фигуру
    bool any = false; // флаг под курсором есть фигура
    for (auto s : m_model->storage()) if (s->contains(pos)) { any = true; break; } // проверяем попадание
    if (any) {
        m_model->selectAt(pos, shift, ctrl); // выбираем фигуры под курсором
    } else {
        Shape* s = nullptr; // будущая фигура
        QColor col = DefaultFillColor(false); // цвет
        switch (m_currentType) { // что создать
        case ShapeType::Circle: s = new Circle(pos, kDefaultSize, col); break; // круг
        case ShapeType::Rectangle: s = new RectShape(pos, kDefaultSize, kDefaultSize, col); break; // прямоугольник
        case ShapeType::Ellipse: s = new EllipseShape(pos, kDefaultSize*12/10, kDefaultSize*8/10, col); break; // эллипс
        case ShapeType::Triangle: s = new TriangleShape(pos, kDefaultSize, kDefaultSize, col); break; // треугольник
        case ShapeType::Line: break; // линия
        }
        if (s) {
            s->moveBy(0, 0, m_view->rect()); // сразу прижимаем фигуру внутрь области виджета
            m_model->clearSelection(); // снимаем прошлое выделение
            s->setSelected(true); // выделяем созданную
            m_model->addShape(s); // добавляем в хранилище
        }
    }
    m_view->update(); // перерисовка
}

void Controller::moveMouse(const QPoint& pos)
{
    if (m_tempLine) { // протягивание линии
        m_tempLine->setEnd(pos); // обновляем конечную
        m_view->update(); // перерисовываем
    }
}

void Controller::endMouse(const QPoint& pos)
{
    if (m_tempLine) { // завершаем создание линии
        m_tempLine->setEnd(pos); // фиксируем конечную позицию
        m_tempLine->moveBy(0, 0, m_view->rect()); // удерживаем линию в пределах области
        m_tempLine->setSelected(true); // выделяем созданную линию
        m_model->addShape(m_tempLine); // переносим во владение модели
        m_tempLine = nullptr; // очищаем указатель
        m_view->update(); // перерисовываем
    }
}

void Controller::keyPress(int key, const QRect& workArea)
{
    int step = 5; // изменение размера
    switch (key) {
    case Qt::Key_Delete: m_model->removeSelected(); break; // удаляем выделенные
    case Qt::Key_Left: m_model->moveSelected(-step,0, workArea); break; // двигаем влево
    case Qt::Key_Right: m_model->moveSelected(step,0, workArea); break; // вправо
    case Qt::Key_Up: m_model->moveSelected(0,-step, workArea); break; // вверх
    case Qt::Key_Down: m_model->moveSelected(0,step, workArea); break; // вниз
    case Qt::Key_Plus:
    case Qt::Key_Equal: m_model->resizeSelected(5, workArea); break; // увеличить
    case Qt::Key_Minus: m_model->resizeSelected(-5, workArea); break; // уменьшить
    default: return;
    }
    m_view->update();
}

void Controller::applyColor(const QColor& c)
{
    m_model->applyColorToSelected(c); // меняем цвет выделенных
    m_view->update();
}

void Controller::deleteSelected()
{
    m_model->removeSelected(); // удалить выделенные элементы
    m_view->update();
}

void Controller::growSelected(const QRect& workArea)
{
    m_model->resizeSelected(5, workArea); // увеличить
    m_view->update();
}

void Controller::shrinkSelected(const QRect& workArea)
{
    m_model->resizeSelected(-5, workArea); // уменьшить
    m_view->update(); // перерисовать
}
