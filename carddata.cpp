/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "carddata.h"


CardData::CardData(QString Name, QString Description, QString MisalignedText, bool BMisaligned, eMatchType Type)
    : misaligned(MisalignedText), type(Type), bMisaligned(BMisaligned)
{
    name = "The " + Name;
    description = name + " " + Description;
}
