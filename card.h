/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QImage>
#include "defines.h"

class Card
{

public:
    Card(QString Name, QString Description, QString Misaligned, QString Bonus,
         int Alignment, int Order, int Suit, QString Image);

    static QImage backImage;

    QString name;
    QString description;
    QString misaligned;
    QString bonus;
    int alignment;
    int order;
    eSuit suit;
    QString imagePath;
    QImage image;

    void setBackImage(QImage Image) { backImage = Image; }

    bool operator==(const Card &card) const {
        return (this->name == card.name);
    }
};

#endif // CARD_H
