#include "include/config_view.h"

#include "../objects/include/menu_grid_box.h"

ConfigView::ConfigView(QGraphicsScene* scene) : QGraphicsView()
{
    setScene(scene);
    setBackgroundBrush(QBrush(QColor(0, 0, 0)));
    ensureVisible(QRectF(0, 0, 0, 0));
}

void ConfigView::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        // CCW
        switch (mDirection)
        {
            case TwoDeg::Up: mDirection = TwoDeg::Left; break;
            case TwoDeg::Left: mDirection = TwoDeg::Down; break;
            case TwoDeg::Down: mDirection = TwoDeg::Right; break;
            case TwoDeg::Right: mDirection = TwoDeg::Up; break;
        }
    }
    else
    {
        // CW
        switch (mDirection)
        {
            case TwoDeg::Up: mDirection = TwoDeg::Right; break;
            case TwoDeg::Left: mDirection = TwoDeg::Up; break;
            case TwoDeg::Down: mDirection = TwoDeg::Left; break;
            case TwoDeg::Right: mDirection = TwoDeg::Down; break;
        }
    }
    updateGridBoxes();
}

void ConfigView::mousePressEvent(QMouseEvent* event)
{
    switch (event->button())
    {
        case Qt::MouseButton::LeftButton:
            attemptFocusTile(event->pos());
            attemptAddPart(event->pos());
            break;
        case Qt::MouseButton::RightButton:
            attemptRemovePart(event->pos());
            //Q_EMIT handleClose();
            break;
        default:;
    }
}

void ConfigView::updateGridBoxes()
{
    for (auto item: items())
    {
        auto box = dynamic_cast<GridBox*>(item);
        if (!box)
            continue;
        box->setDirection(mDirection);
        box->setDrawDirection(mItemRequiresDirection);
        box->update();
    }
}

void ConfigView::attemptFocusTile(QPoint pos)
{
    auto focusTile = dynamic_cast<ComponentTile*>(itemAt(pos));
    if (focusTile)
    {
        for (auto item: items())
        {
            auto tile = dynamic_cast<ComponentTile*>(item);
            if (!tile)
                continue;
            if (tile == focusTile)
                tile->setFocus(true);
            else
                tile->setFocus(false);
        }
        mFocusComponent = focusTile->getType();
        mItemRequiresDirection = false;
        if (mFocusComponent == Component::ComponentType::CruiseThruster)
            mItemRequiresDirection = true;
        updateGridBoxes();
        return;
    }
}

void ConfigView::attemptAddPart(QPoint pos)
{
    for (auto item : items(pos))
    {
        auto box = dynamic_cast<GridBox*>(item);
        if (box)
        {
            Q_EMIT addShipPart(mFocusComponent, box->getCoords(), mDirection);
            return;
        }
    }
}

void ConfigView::attemptRemovePart(QPoint pos)
{
    for (auto item : items(pos))
    {
        auto box = dynamic_cast<GridBox*>(item);
        if (box)
        {
            Q_EMIT removeShipPart(box->getCoords());
            return;
        }
    }
}

ConfigScene::ConfigScene(QWidget* parent) : QGraphicsScene(parent)
{
    setSceneRect(QRectF(-50, -50, 100, 100));
    mView = new ConfigView(this);
    connect(mView, &ConfigView::handleClose, this, &ConfigScene::handleClose);
    mView->scale(4, 4);
    mView->setRenderHint(QPainter::Antialiasing);

    // Draw grid-lines
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            addItem(new GridBox(x, y));

    // Draw component tiles
    addItem(new ComponentTile(-30, -80, Component::ComponentType::Reactor));
    addItem(new ComponentTile(-30, -55, Component::ComponentType::HeatSink));
    addItem(new ComponentTile(-95, -55, Component::ComponentType::RotateThruster));
    addItem(new ComponentTile(-95, -80, Component::ComponentType::CruiseThruster));
}

ConfigView* ConfigScene::getView() const
{
    return mView;
}

void ConfigScene::handleClose()
{
    Q_EMIT close();
}

void ConfigScene::drawConfigComponent(std::shared_ptr<Component> component)
{
    mComponents << addPolygon(component->getPoly(), QPen(QColor(0, 255, 0)));
}

void ConfigScene::drawConfigEngine(std::shared_ptr<Engine> engine)
{
    mComponents << addPolygon(engine->getPoly(), QPen(QColor(0, 255, 0)));
}

void ConfigScene::deleteAllComponents()
{
    for (auto item : mComponents)
    {
        removeItem(item);
        delete item;
    }
    mComponents.clear();
}

