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
    void sendParsedInput(const QString& text);
    void setThrustDirection(TwoDeg direction, bool isActive);

public Q_SLOTS:
    void parseInput(const QString& rawText);

private:
    enum Command
    {
        Thrust,
        Alias
    };

    void parseCommand(const QString& command, const QString& input);
    void parseThrustCommand(const QString& direction, bool isActive);
    void parseAliasCommand(const QString& input);

    History* mHistory;
    Input* mInput;

    QMap<QString, QString> mAliases;

    // Command lookups
    QMap<QString, TwoDeg> mLookupDirection;
    QMap<QString, Command> mLookupCommands;
};
