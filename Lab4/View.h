#pragma once
#include <QtWidgets/QWidget>
#include "Model.h"
#include "Controller.h"

class CanvasView : public QWidget {
    Q_OBJECT
public:
    CanvasView(Model* model, Controller* controller, QWidget* parent = nullptr);

    QSize minimumSizeHint() const override { return {400,400}; } // минимальный размер
    QSize sizeHint() const override { return {800,600}; } // размер окна

protected:
    void paintEvent(QPaintEvent*) override; // перерисовка
    void mousePressEvent(QMouseEvent* ev) override; // нажатие мыши начало создания/выбора
    void mouseMoveEvent(QMouseEvent* ev) override; // движение
    void mouseReleaseEvent(QMouseEvent* ev) override; // отпускание завершение
    void keyPressEvent(QKeyEvent* ev) override; // клавиатура
    void wheelEvent(QWheelEvent* ev) override; // колесо

private:
    Model* m_model; // модель с хранилищем фигур
    Controller* m_controller; // контроллер
};
