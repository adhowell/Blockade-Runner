#include <QtWidgets>
#include <QPlainTextEdit>

#pragma once

class History : public QPlainTextEdit
{
Q_OBJECT
public:
    History();

public Q_SLOTS:
    void addCommand(const QString &text);

};