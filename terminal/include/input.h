#include <QtWidgets>
#include <QPlainTextEdit>

#pragma once


class Input : public QPlainTextEdit
{
    Q_OBJECT
public:
    Input();

    void keyPressEvent(QKeyEvent *e) override;

Q_SIGNALS:
    void sendRawInput(QString text);

private:
    void displayAdjacentCommand(bool isPrevious);

    int mLimit = 53;

    int mCommandIndex = 0;
    QVector<QString> mCommandHistory;
};