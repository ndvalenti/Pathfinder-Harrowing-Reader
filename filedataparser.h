/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef FILEDATAPARSER_H
#define FILEDATAPARSER_H
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "card.h"
#include "suit.h"
#include "cardspread.h"

#include <QMessageBox>

class FileDataParser: public QObject
{
    Q_OBJECT
public:
    explicit FileDataParser(QObject *parent = 0);

    void parseSpreadFile(QString Source);
    void parseSuitFile(QString Source);
    void parseCardFile(QString Source);

signals:
    void addSpread(CardSpread* NewSpread);
    void addSuit(int Id, Suit NewSuit, QString Harrowing, QString NHarrowing, QString GreaterHarrowing);
    void addCard(Card NewCard, QString MenuName);
};

#endif // FILEDATAPARSER_H
