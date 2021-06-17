/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "cardframe.h"

CardFrame::CardFrame(QWidget *parent)
    :QFrame(parent)
{
    setFixedHeight(200);
    setFixedWidth(400);
    content->setReadOnly(true);
    content->setFixedSize(350, 175);
    content->viewport()->setCursor(Qt::ArrowCursor);
}

void CardFrame::setText(QString Text)
{
    content->setText(Text);
}

void CardFrame::setHeight(int Height)
{
    setFixedHeight(Height);
    content->setFixedHeight(Height - 25);
}
