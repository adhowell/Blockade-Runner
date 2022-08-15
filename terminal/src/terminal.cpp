#include "include/terminal.h"
#include <QRegExp>

Terminal::Terminal(QWidget* parent) : QFrame(parent)
{
    mHistory = new History();
    mInput = new Input();

    mLookupDirection["FORWARD"] = TwoDeg::Up;
    mLookupDirection["BACKWARD"] = TwoDeg::Down;
    //mLookupDirection["LEFT"] = TwoDeg::Left;
    //mLookupDirection["RIGHT"] = TwoDeg::Right;

    mLookupCommands["START"] = Command::Thrust;
    mLookupCommands["STOP"] = Command::Thrust;
    mLookupCommands["ALIAS"] = Command::Alias;
    mLookupCommands["ROTATE"] = Command::Rotate;

    connect(mInput, &Input::sendRawInput, this, &Terminal::parseInput);
    connect(this, &Terminal::sendParsedInput, mHistory, &History::addCommand);

    auto layout = new QVBoxLayout;
    layout->addWidget(mHistory, 8);
    layout->addWidget(mInput, 1);
    setLayout(layout);
    hide();
}

void Terminal::parseInput(const QString& rawText)
{
    QRegularExpression re("(\\w+) (.+)");
    QRegularExpressionMatch match = re.match(rawText);
    Q_EMIT sendParsedInput(QString("<LOG> - %1").arg(rawText));

    if (match.hasMatch())
    {
        QString aliasedCommand = match.captured(1);
        if (mAliases.contains(aliasedCommand)) aliasedCommand = mAliases[aliasedCommand];
        parseCommand(aliasedCommand, match.captured(2));
    }
    else
        Q_EMIT sendParsedInput(QString("<ERROR> - INVALID ENTRY"));
}

void Terminal::parseCommand(const QString& command, const QString& input)
{
    switch (mLookupCommands[command])
    {
        case Command::Thrust:
            parseThrustCommand(input, command == "START");
            break;
        case Command::Alias:
            parseAliasCommand(input);
            break;
        case Command::Rotate:
            parseRotateCommand(input);
            break;
        case Command::None:
            Q_EMIT sendParsedInput(QString("<ERROR> - INVALID COMMAND: %1").arg(command));
            return;
    }
}

void Terminal::parseThrustCommand(const QString& input, bool isActive)
{
    QStringList args = input.split(" ");
    if (args.size() > 1)
    {
        Q_EMIT sendParsedInput(QString("<ERROR> - COMMAND: %1 ACCEPTS ONE ARGUMENT").arg(isActive ? "START" : "STOP"));
        return;
    }
    QString direction = args[0];
    if (mAliases.contains(direction)) direction = mAliases[direction];

    if (mLookupDirection.contains(direction))
        setThrustDirection(mLookupDirection[direction], isActive);
    else
        Q_EMIT sendParsedInput(QString("<ERROR> - INVALID DIRECTION: %1").arg(direction));
}

void Terminal::parseAliasCommand(const QString& input)
{
    QStringList inputs = input.split(" ");
    if (inputs.size() != 2)
    {
        Q_EMIT sendParsedInput(QString("<ERROR> - COMMAND: ALIAS ACCEPTS TWO ARGUMENTS"));
        return;
    }
    mAliases[inputs[1]] = inputs[0];
    Q_EMIT sendParsedInput(QString("<INFO> - ALIAS %1 == %2").arg(inputs[1]).arg(inputs[0]));
}

void Terminal::parseRotateCommand(const QString &input)
{
    int degrees = input.toInt();
    if (degrees == 0)
    {
        Q_EMIT sendParsedInput(QString("<ERROR> - COMMAND: ROTATE ACCEPTS ONE NON-ZERO INTEGER"));
        return;
    }
    rotate(degrees);
}

void Terminal::displayText(const QString &text)
{
    mHistory->addCommand(text);
}