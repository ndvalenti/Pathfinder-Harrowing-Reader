/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef RESULTDATABLOCK_H
#define RESULTDATABLOCK_H
#include <QString>
#include <QVector>
#include "card.h"
#include "carddata.h"
#include "defines.h"

class CardResultDataBlock
{
public:
    CardResultDataBlock(QString Section);

    QString section;
    QVector<CardData> cardArray;

    void addCardData(Card* Card, bool BMisaligned, eMatchType Match = eMatchType::NoMatch);
};

#endif // RESULTDATABLOCK_H
