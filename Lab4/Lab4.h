#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include "ui_Lab4.h"
#include "Common.h"
#include "Model.h"
#include "Controller.h"
#include "View.h"

// главное окно MVC
class Lab4 : public QMainWindow
{
    Q_OBJECT
public:
    Lab4(QWidget *parent = nullptr);
    ~Lab4();
private:
    Ui::Lab4Class ui;

    Model m_model; // модель
    Controller* m_controller{ nullptr }; // контроллер
    CanvasView* m_view{ nullptr }; // представ

    QActionGroup* m_shapeActions{ nullptr }; // группа для выбора одного типа фигуры
    QAction* actCircle{ nullptr }; // Круг
    QAction* actRect{ nullptr }; // Прямоугольник
    QAction* actEllipse{ nullptr }; // Эллипс
    QAction* actTriangle{ nullptr }; // Треугольник
    QAction* actLine{ nullptr }; // Линия

    QAction* actColor{ nullptr }; // выбрать цвет
    QAction* actDelete{ nullptr }; // удалить выделенные
    QAction* actGrow{ nullptr }; // увеличить размер
    QAction* actShrink{ nullptr }; // уменьшить размер

    void setupToolbar();// тулбар
    void setupMenus(); // создание меню
private slots:
    void onShapeSelected(QAction* act); // обработка выбора типа фигуры
    void onChooseColor(); // выбор цвета
    void onDeleteSelected(); // удаление выделенных фигур
    void onGrow(); // увеличение размеров выделенных фигур
    void onShrink(); // уменьшение размеров выделенных фигур
};

