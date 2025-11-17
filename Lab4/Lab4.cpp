#include "Lab4.h"
#include <QtWidgets/QToolBar>
#include <QtGui/QActionGroup>
#include <QtWidgets/QColorDialog>

Lab4::Lab4(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // Создаем view и контроллер
    m_view = new CanvasView(&m_model, nullptr, this);
    m_controller = new Controller(&m_model, m_view);
    delete m_view;
    m_view = new CanvasView(&m_model, m_controller, this);

    setCentralWidget(m_view); // центральный виджет
    setupToolbar(); // тулбар
    setupMenus(); // меню
}

Lab4::~Lab4() {} // деструктор по умолчанию

void Lab4::setupToolbar()
{
    QToolBar* tb = new QToolBar("Фигуры", this); // создаем тулбар
    addToolBar(tb); // добавляем

    m_shapeActions = new QActionGroup(this); // группа выбора типа
    m_shapeActions->setExclusive(true); // только одно действие активно
    connect(m_shapeActions, &QActionGroup::triggered, this, &Lab4::onShapeSelected); // обработчик переключения фигуры

    actCircle = tb->addAction("Круг"); actCircle->setCheckable(true); actCircle->setData(int(ShapeType::Circle)); // круг
    actRect = tb->addAction("Прямоугольник"); actRect->setCheckable(true); actRect->setData(int(ShapeType::Rectangle)); // прямоугольник
    actEllipse = tb->addAction("Эллипс"); actEllipse->setCheckable(true); actEllipse->setData(int(ShapeType::Ellipse)); // эллипс
    actTriangle = tb->addAction("Треугольник"); actTriangle->setCheckable(true); actTriangle->setData(int(ShapeType::Triangle)); // треугольник
    actLine = tb->addAction("Отрезок"); actLine->setCheckable(true); actLine->setData(int(ShapeType::Line)); // линия

    m_shapeActions->addAction(actCircle); // добавляем действия в группу
    m_shapeActions->addAction(actRect);
    m_shapeActions->addAction(actEllipse);
    m_shapeActions->addAction(actTriangle);
    m_shapeActions->addAction(actLine);

    actCircle->setChecked(true); // по умолчанию выбран круг

    actColor = tb->addAction("Цвет..."); connect(actColor, &QAction::triggered, this, &Lab4::onChooseColor); // выбрать цвет
    actDelete = tb->addAction("Удалить"); connect(actDelete, &QAction::triggered, this, &Lab4::onDeleteSelected); // удалить
    actGrow = tb->addAction("Увеличить"); connect(actGrow, &QAction::triggered, this, &Lab4::onGrow); // увеличить
    actShrink = tb->addAction("Уменьшить"); connect(actShrink, &QAction::triggered, this, &Lab4::onShrink); // уменьшить
}

void Lab4::setupMenus()
{
    QMenu* menuShapes = menuBar()->addMenu("Фигуры"); // меню выбора фигур
    menuShapes->addActions(m_shapeActions->actions()); // добавляем все действия типов фигур
    QMenu* menuEdit = menuBar()->addMenu("Редактирование"); // меню редактирования
    menuEdit->addAction(actColor); // цвет
    menuEdit->addAction(actDelete); // удалить
    menuEdit->addAction(actGrow); // увеличить
    menuEdit->addAction(actShrink); // уменьшить
}

void Lab4::onShapeSelected(QAction* act)
{
    ShapeType t = ShapeType::Circle; // дефолт
    if (act) t = static_cast<ShapeType>(act->data().toInt()); // извлекаем тип
    m_controller->setShapeType(t); // новый тип
}

void Lab4::onChooseColor()
{
    QColor chosen = QColorDialog::getColor(Qt::yellow, this, "Цвет"); // показываем диалог
    if (chosen.isValid()) m_controller->applyColor(chosen);
}

void Lab4::onDeleteSelected()
{
    m_controller->deleteSelected(); // удаляем выделенные фигуры
}

void Lab4::onGrow()
{
    m_controller->growSelected(m_view->rect()); // увеличиваем фигуры
}

void Lab4::onShrink()
{
    m_controller->shrinkSelected(m_view->rect()); // уменьшаем фигуры
}

