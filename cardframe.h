/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDFRAME_H
#define CARDFRAME_H
#include <QFrame>
#include <QTextEdit>

class CardFrame : public QFrame
{
    Q_OBJECT

public:
    explicit CardFrame(QWidget *parent = 0);

    void setText(QString Text);
    bool hasContent() { return (content->toPlainText() != ""); }
    void setHeight(int Height);

private:
    QTextEdit* content = new QTextEdit(this);
};

#endif // CARDFRAME_H
