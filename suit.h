/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef SUIT_H
#define SUIT_H

#include <QString>
#include "defines.h"

class Suit
{
public:
    Suit();
    Suit(QString Name, QString Domain, QString Ability);

    QString name;
    QString domain;
    QString ability;
};

#endif // SUIT_H
