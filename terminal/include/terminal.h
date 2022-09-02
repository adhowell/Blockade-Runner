#include "include/input.h"
#include "include/history.h"
#include "include/directions.h"

#include <QtWidgets>

#pragma once


class Terminal : public QFrame
{
    Q_OBJECT
public:
    explicit Terminal(QWidget* parent = nullptr);

    void setInputFocus() { mInput->setFocus(); }

Q_SIGNALS:
    void setThrustDirection(TwoDeg direction, bool isActive);
    void rotate(int degrees);

public Q_SLOTS:
    void parseInput(const QString& rawText);
    void displayLog(const QString& text);
    void displayInfo(const QString& text);
    void displayWarning(const QString& text);
    void displayError(const QString& text);

private:
    enum Command
    {
        None,
        Thrust,
        Rotate,
        Alias
    };

    void parseCommand(const QString& command, const QString& input);
    void parseThrustCommand(const QString& direction, bool isActive);
    void parseAliasCommand(const QString& input);
    void parseRotateCommand(const QString& input);

    History* mHistory;
    Input* mInput;

    QMap<QString, QString> mAliases;

    // Command lookups
    QMap<QString, TwoDeg> mLookupDirection;
    QMap<QString, Command> mLookupCommands;
};
