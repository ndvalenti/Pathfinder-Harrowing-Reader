/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDDATA_H
#define CARDDATA_H
#include <QString>
#include "defines.h"


class CardData
{
public:
    CardData(QString Name, QString Description, QString MisalignedText,
             bool BMisaligned, eMatchType Type);

    QString name;
    QString description;
    QString misaligned;
    eMatchType type;
    bool bMisaligned;
};

#endif // CARDDATA_H
