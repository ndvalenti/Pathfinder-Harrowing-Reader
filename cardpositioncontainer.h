/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDPOSITIONCONTAINER_H
#define CARDPOSITIONCONTAINER_H
#include "carddisplaylabel.h"
#include "cardspread.h"

class CardDisplayDataContainer
{
public:
    CardDisplayDataContainer(CardDisplayLabel* Label);

    Card* card;
    CardMetaData cardMetaData;
    eMatchType type;
    bool bMisaligned = false;
    bool bFacedown = false;
    CardDisplayLabel* label;
};

struct CardDisplayDataCollection
{
    QString name;
    QVector<CardDisplayDataContainer*> displayContainer;

    CardDisplayDataCollection(QString Name, QVector<CardDisplayDataContainer*> Container) {
        name = Name;
        displayContainer = Container;
    }
};

#endif // CARDPOSITIONCONTAINER_H
