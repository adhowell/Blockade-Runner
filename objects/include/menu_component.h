#include <QtWidgets>

#pragma once

class ComponentTile : public QGraphicsItem
{
public:
    ComponentTile(qreal x, qreal y, Component::ComponentType comp) : mX(x), mY(y), mType(comp)
    {
        switch (comp)
        {
            case Component::ComponentType::HeatSink:
                mLabel = "HEAT SINK";
                break;
            case Component::ComponentType::Reactor:
                mLabel = "REACTOR";
                break;
            case Component::ComponentType::RotateThruster:
                mLabel = "THRUSTER";
                break;
            case Component::ComponentType::CruiseThruster:
                mLabel = "ENGINE";
                break;
            default:
                break;
        }
    }

    constexpr static qreal sHeight = 20;
    constexpr static qreal sWidth = 60;

    void setFocus(bool isFocus)
    {
        if (isFocus)
            mPen.setColor(QColor(0, 255, 0, 150));
        else
            mPen.setColor(QColor(0, 255, 0, 25));
        prepareGeometryChange();
    }

    Component::ComponentType getType() { return mType; }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(mPen);
        painter->drawRect(QRectF(mX, mY, sWidth, sHeight));

        painter->setFont(QFont("OCR A Extended", 7));
        auto fm = QFontMetricsF(painter->fontMetrics());
        painter->drawText(QPointF(mX + (sWidth*0.5) - fm.width(mLabel)*0.5, mY + (fm.height() + sHeight)*0.42),
                          mLabel);
    }

    QRectF boundingRect() const override { return { mX, mY, sWidth, sHeight }; }

    qreal mX;
    qreal mY;
    QString mLabel;
    Component::ComponentType mType;
    QPen mPen {QColor(0, 255, 0, 25) };
};