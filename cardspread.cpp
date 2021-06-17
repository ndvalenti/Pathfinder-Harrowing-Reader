/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "cardspread.h"

CardSpread::CardSpread() {}

CardSpread::CardSpread(int Id, QString Name, QString Description, int Width, int Height,
                       int CardWidth, int CardHeight)
    : id(Id), name(Name), description(Description), width(Width), height(Height),
      cardWidth(CardWidth), cardHeight(CardHeight)
{}
