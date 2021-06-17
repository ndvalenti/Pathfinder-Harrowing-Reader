/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDSPREAD_H
#define CARDSPREAD_H
#include <QString>
#include <QVector>
#include "card.h"

struct CardMetaData {
    int id;
    int r;
    int c;
    int rSpan;
    int cSpan;

    int alignment = -1;
    int order = -1;
    float misalignChance = 0.0f;

    bool bFacedownDefault = false;
    bool bChosen = false;

    CardMetaData() {}
    CardMetaData(int Id, int R, int C, int RSpan, int CSpan,
                 float MisalignChance = 0.0f, bool BFacedownDefault = false)
        :id(Id), r(R), c(C), rSpan(RSpan), cSpan(CSpan)
    {
        misalignChance = MisalignChance;
        bFacedownDefault = BFacedownDefault;
    }
};

struct CardCollection {
    int id;
    QString name;
    QVector<CardMetaData> cards;

    CardCollection(QString Name) : name(Name) {}

    bool operator<(const CardCollection &Collection) const {
        return (this->id < Collection.id);
    }
    bool operator>(const CardCollection &Collection) const {
        return (this->id > Collection.id);
    }
};

class CardSpread
{
public:
    CardSpread();
    CardSpread(int Id, QString Name, QString Description,
               int Width, int Height, int CardWidth, int CardHeight);

    int id;
    QString name;
    QString description;

    int width;
    int height;

    int cardWidth;
    int cardHeight;

    QVector<CardCollection> cardCollections;
};

#endif // CARDSPREAD_H
