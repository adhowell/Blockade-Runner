#include <QtWidgets>

#pragma once


class TextBox : public QGraphicsItem
{
public:
    TextBox(qreal x, qreal y, QString category, QString defaultText) : mX(x), mY(y), mCategory(category), mDefaultText(defaultText) {}
    ~TextBox() override = default;

    constexpr static qreal sHeight = 9.5;
    constexpr static qreal sWidth = 90;

    void updateText(QString text)
    {
        mText = text;
        update();
    }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        QPen pen;
        pen.setWidthF(0.5);
        mText.isEmpty() ? pen.setColor(QColor(255, 0, 0, 150)) : pen.setColor(QColor(0, 255, 0, 150));
        painter->setPen(pen);
        painter->drawRect(QRectF(mX, mY, sWidth, sHeight));

        painter->setFont(QFont("OCR A Extended", 3));
        auto fm = QFontMetricsF(painter->fontMetrics());
        painter->drawText(QPointF(mX + 2, mY + (fm.height() + sHeight)*0.42), mCategory + (mText.isEmpty() ? mDefaultText : mText));
    }

    QRectF boundingRect() const override { return { mX, mY, sWidth, sHeight }; }

    qreal mX;
    qreal mY;
    QString mText;
    QString mCategory;
    QString mDefaultText;
};