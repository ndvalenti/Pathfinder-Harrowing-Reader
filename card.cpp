/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "card.h"

QImage Card::backImage;

Card::Card(QString Name, QString Description, QString Misaligned, QString Bonus,
     int Alignment, int Order, int Suit, QString Image)
    : name(Name), description(Description), misaligned(Misaligned), bonus(Bonus),
      alignment(Alignment), order(Order), imagePath(Image)
{
    suit = static_cast<eSuit>(Suit);
    image.load(Image);
}
