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
            clear();
            return;
        case Qt::Key::Key_Backspace:
            QPlainTextEdit::keyPressEvent(e);
            break;
        default:
            if (toPlainText().length() >= mLimit)
                return;
            insertPlainText(e->text().toUpper());
            break;
    }
}