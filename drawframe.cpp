/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "drawframe.h"

#include <QStyle>
DrawFrame::DrawFrame(QWidget *parent)
    : QFrame(parent)
{
    setLayout(layout);
    setFixedWidth(250);
    setFixedHeight(175);
    layout->setAlignment(Qt::AlignCenter);

    selector->setFixedWidth(50);
    selector->setVisible(false);

    frameButton->setFixedWidth(50);

    description->setReadOnly(true);
    description->setMaximumWidth(225);
    description->viewport()->setCursor(Qt::ArrowCursor);

    connect(frameButton, &QPushButton::released, this, &DrawFrame::buttonPress);
    connect(selector, &QComboBox::currentIndexChanged, this, &DrawFrame::setDescriptionText);

    layout->addWidget(description, 0, 0, 1, 2);
    layout->addWidget(selector, 2, 0);
    layout->addWidget(frameButton, 2, 1);
    layout->addWidget(selectorDesc, 1, 0, 1, 2);
}

void DrawFrame::setInstructionText(QString In)
{
    description->setText(In);
}

void DrawFrame::setButtonText(QString In)
{
    frameButton->setText(In);
}

void DrawFrame::setSelectorVisible(bool visible)
{
    selector->setVisible(visible);
}

void DrawFrame::addComboBoxItem(const int Key, const QString Name, const QString Description)
{
    selector->insertItem(Key, Name);
    descriptionPairs.insert(Key, Description);
    setDescriptionText(selector->currentIndex());
}

int DrawFrame::getComboBoxSelected()
{
    if (!selector->isVisible()) {
        return -1;
    }
    return selector->currentIndex();
}

void DrawFrame::setComboBoxSelected(int Selection)
{
    if (!selector->isVisible()) {
        return;
    }
    selector->setCurrentIndex(Selection);
}

void DrawFrame::setDescriptionText(int Key)
{
    selectorDesc->setText(descriptionPairs.value(Key));
}
