#pragma once
#include "Model.h"
#include "Common.h"

class CanvasView;

// контроллер
class Controller {
public:
    Controller(Model* model, CanvasView* view); // указатели на модель и представление

    void setShapeType(ShapeType t) { m_currentType = t; } // меняем активный тип создаваемой фигуры

    void beginMouse(const QPoint& pos, Qt::KeyboardModifiers mods); // старт операции (создание или выбор)
    void moveMouse(const QPoint& pos); // перемещение мыши
    void endMouse(const QPoint& pos); // завершение операции

    void keyPress(int key, const QRect& workArea); // обработка клавиатуры

    void applyColor(const QColor& c); // применить цвет ко всем выделенным
    void deleteSelected(); // удалить выделенные фигуры
    void growSelected(const QRect& workArea); // увеличить выделенные
    void shrinkSelected(const QRect& workArea); // уменьшить выделенные

private:
    Model* m_model; // ссылка на model
    CanvasView* m_view; // ссылка на view
    ShapeType m_currentType{ ShapeType::Circle }; // текущий выбранный тип для создания
    LineShape* m_tempLine{ nullptr }; // линия
};
