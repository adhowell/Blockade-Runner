#include "include/history.h"

History::History() : QPlainTextEdit()
{
    setReadOnly(true);
    setStyleSheet("color: #00ff00; background-color: black; font-size: 20px; font-family: \"OCR A Extended\"");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void History::addCommand(const QString &text)
{
    appendPlainText(text);
}