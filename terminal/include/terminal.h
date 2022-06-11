#include "include/input.h"
#include "include/history.h"
#include "../physics/include/directions.h"
#include <QtWidgets>

#pragma once

class Terminal : public QFrame
{
    Q_OBJECT
public:
    explicit Terminal(QWidget* parent = nullptr);

Q_SIGNALS:
    void sendParsedInput(QString text);
    void setThrustDirection(TwoDeg direction, bool isActive);

public Q_SLOTS:
    void parseInput(QString rawText);

private:
    void parseCommands(QString rawText);
    void parseThrustCommand(QString rawText);
    int getLineIndex();

    History* mHistory;
    Input* mInput;

    int mIndex = 0;

    // Command lookups
    QMap<QString, TwoDeg> mLookupDirection;
};
