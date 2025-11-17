#include "View.h"
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

CanvasView::CanvasView(Model* model, Controller* controller, QWidget* parent)
    : QWidget(parent), m_model(model), m_controller(controller) // ссылки на модель и контроллер
{
    setFocusPolicy(Qt::StrongFocus); // клава
    setAutoFillBackground(true); // фон закрашивается автомат
}

void CanvasView::paintEvent(QPaintEvent*)
{
    QPainter painter(this); // рисование
    painter.fillRect(rect(), Qt::white); // закрашиваем фон белым
    for (auto s : m_model->storage()) { // проходим все фигуры
        s->draw(painter); //нарисовать фигуру
    }
}

void CanvasView::mousePressEvent(QMouseEvent* ev)
{
    m_controller->beginMouse(ev->pos(), ev->modifiers()); // пнажатие
}

void CanvasView::mouseMoveEvent(QMouseEvent* ev)
{
    m_controller->moveMouse(ev->pos());
}

void CanvasView::mouseReleaseEvent(QMouseEvent* ev)
{
    m_controller->endMouse(ev->pos()); // завершаем
}

void CanvasView::keyPressEvent(QKeyEvent* ev)
{
    m_controller->keyPress(ev->key(), rect());
}

void CanvasView::wheelEvent(QWheelEvent* ev)
{
    int delta = ev->angleDelta().y()>0 ? 5 : -5; // направление крутки
    if (delta>0) m_controller->growSelected(rect()); else m_controller->shrinkSelected(rect()); // масштабируем выделенные
}
