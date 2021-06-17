/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef RESULTDATAMAT_H
#define RESULTDATAMAT_H

#include <QWidget>
#include <QVector>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QFrame>
#include "resultdatablock.h"
#include "cardframe.h"
#include "defines.h"

class ResultDataMat : public QFrame
{
    Q_OBJECT

public:
    explicit ResultDataMat(QWidget *parent = 0);

private:
    QVBoxLayout* mat = new QVBoxLayout(this);
    QTabWidget* tab = new QTabWidget(this);
    QVector<QWidget*> wRecord;
    QVector<QLayout*> lRecord;
    QMap<eSuit, HarrowData> suitData;

public slots:
    void addCardDataBlockResult(CardResultDataBlock &Result);
    void addHarrowNeutralRoleResult();
    void addHarrowDataResults(QMultiMap<eSuit,SpellResult> &Result);
    void addHarrowData(eSuit Suit, QString Name, QString Harrowing, QString NegativeHarrowing, QString GreaterHarrowing);
    void reset();

};

#endif // RESULTDATAMAT_H
