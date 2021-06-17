/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "carddisplaylabel.h"

CardDisplayLabel::CardDisplayLabel(QWidget* parent)
    : QLabel(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setScaledContents(true);
    setAlignment(Qt::AlignCenter);

    connect(this, &CardDisplayLabel::customContextMenuRequested, this, &CardDisplayLabel::openContextMenu);
}
