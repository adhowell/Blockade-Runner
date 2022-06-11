#include <QtWidgets>
#include <QPlainTextEdit>

#pragma once

class Input : public QPlainTextEdit
{
    Q_OBJECT
public:
    Input();

    void keyPressEvent(QKeyEvent *e) override;

private:
    int mLimit = 20;

Q_SIGNALS:
    void sendRawInput(QString text);


};