/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <QString>

enum class eMatchType {
    RoleCard,
    TrueMatch,
    OppositeMatch,
    PartialMatch,
    NoMatch
};

enum class eSuit {
    Hammers = 0,
    Keys = 1,
    Shields = 2,
    Books = 3,
    Stars = 4,
    Crowns = 5,
    None = -1
};

struct SpellResult
{
    bool bValid = false;
    int harrowingBonus = 0;
    bool bGreaterHarrowingBonus = false;
};

struct HarrowData
{
    QString name;
    QString harrowing;
    QString negativeHarrowing;
    QString greaterHarrowing;
};

struct HarrowResult
{
    eSuit suit;
    int magnitude;
    QString description;
};


#endif // DEFINES_H
