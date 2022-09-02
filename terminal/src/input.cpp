#include "include/input.h"


Input::Input() : QPlainTextEdit()
{
    setFixedHeight(25);  // Don't like this
    setStyleSheet("border: 0px; color: #00ff00; background-color: black; font-size: 15px; font-family: \"OCR A Extended\"");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    insertPlainText(">> ");
}

void Input::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key::Key_Enter:
        case Qt::Key::Key_Return:
            Q_EMIT sendRawInput(toPlainText().mid(3));
            mCommandHistory << toPlainText().mid(3);
            mCommandIndex = mCommandHistory.size();
            clear();
            insertPlainText(">> ");
            return;
        case Qt::Key::Key_Backspace:
            if (toPlainText().size() == 3) return;
        case Qt::Key::Key_Left:
            if (textCursor().columnNumber() == 3) return;
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
    if (mCommandHistory.empty()) return;
    if (mCommandIndex == 0 && isPrevious) return;
    if (mCommandIndex >= mCommandHistory.size()-1 && !isPrevious)
    {
        clear();
        insertPlainText(">> ");
        mCommandIndex = mCommandHistory.size();
        return;
    }
    clear();
    mCommandIndex += isPrevious ? -1 : 1;
    insertPlainText(">> " + mCommandHistory[mCommandIndex]);
}