/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "suit.h"

Suit::Suit() {}

Suit::Suit(QString Name, QString Domain, QString Ability)
    : name(Name), domain(Domain), ability(Ability)
{}
