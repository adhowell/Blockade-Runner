#include "include/terminal.h"
#include <QRegExp>

Terminal::Terminal(QWidget* parent) : QFrame(parent)
{
    mHistory = new History();
    mInput = new Input();

    mLookupDirection["FORWARD"] = TwoDeg::Up;
    mLookupDirection["BACKWARD"] = TwoDeg::Down;
    mLookupDirection["LEFT"] = TwoDeg::Left;
    mLookupDirection["RIGHT"] = TwoDeg::Right;



    connect(mInput, &Input::sendRawInput, this, &Terminal::parseInput);
    connect(this, &Terminal::sendParsedInput, mHistory, &History::addCommand);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mHistory, 8);
    layout->addWidget(mInput, 1);
    setLayout(layout);
}

void Terminal::parseInput(QString rawText)
{
    QRegularExpression re("(\\w+) (\\w+)");
    QRegularExpressionMatch match = re.match(rawText);
    Q_EMIT sendParsedInput(QString("%1 <LOG> - %2").arg(getLineIndex()).arg(rawText));

    if (match.hasMatch())
        parseCommands(rawText);
    else
        Q_EMIT sendParsedInput(QString("%1 <ERROR> - INVALID ENTRY").arg(getLineIndex()));
}

void Terminal::parseCommands(QString rawText)
{
    QRegularExpression re("(\\w+) (\\w+)");
    QRegularExpressionMatch match = re.match(rawText);
    QRegularExpression thrustRe("(START|STOP) (\\w+)");
    QRegularExpressionMatch thrustMatch = thrustRe.match(rawText);

    if (thrustMatch.hasMatch())
    {
        if (!mLookupDirection.contains(thrustMatch.captured(2)))
        {
            Q_EMIT sendParsedInput(QString("%1 <ERROR> - INVALID DIRECTION: %2").arg(getLineIndex()).arg(thrustMatch.captured(2)));
            return;
        }
        Q_EMIT setThrustDirection(mLookupDirection[thrustMatch.captured(2)],
                                  thrustMatch.captured(1) == "START");
        return;
    }
    Q_EMIT sendParsedInput(QString("%1 <ERROR> - INVALID COMMAND: %2").arg(getLineIndex()).arg(match.captured(1)));
}

int Terminal::getLineIndex()
{
    return mIndex++;
}