/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "resultdatablock.h"

CardResultDataBlock::CardResultDataBlock(QString Section)
    : section(Section)
{ }

void CardResultDataBlock::addCardData(Card *Card, bool BMisaligned, eMatchType Match)
{
    CardData data(Card->name, Card->description, Card->misaligned, BMisaligned, Match);
    cardArray.append(data);
}
