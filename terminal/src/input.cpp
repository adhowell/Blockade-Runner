#include "include/input.h"

Input::Input() : QPlainTextEdit()
{
    setStyleSheet("color: #00ff00; background-color: black; font-size: 25px; font-family: \"OCR A Extended\"");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void Input::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key::Key_Enter:
        case Qt::Key::Key_Return:
            Q_EMIT sendRawInput(toPlainText());
            mCommandHistory << toPlainText();
            mCommandIndex = mCommandHistory.size();
            clear();
            return;
        case Qt::Key::Key_Backspace:
        case Qt::Key::Key_Left:
        case Qt::Key::Key_Right:
            QPlainTextEdit::keyPressEvent(e);
            break;
        case Qt::Key::Key_Up:
            displayAdjacentCommand(true);
            break;
        case Qt::Key::Key_Down:
            displayAdjacentCommand(false);
            break;
        default:
            if (toPlainText().length() >= mLimit)
                return;
            insertPlainText(e->text().toUpper());
            break;
    }
}

void Input::displayAdjacentCommand(bool isPrevious)
{
    if (mCommandIndex == 0 && isPrevious) return;
    if (mCommandIndex == mCommandHistory.size()-1 && !isPrevious) return;
    clear();
    mCommandIndex += isPrevious ? -1 : 1;
    insertPlainText(mCommandHistory[mCommandIndex]);
}