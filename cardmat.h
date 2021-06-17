/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDMAT_H
#define CARDMAT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QMultiMap>
#include <QVector>
#include <QTransform>
#include "card.h"
#include "resultdatablock.h"
#include "cardpositioncontainer.h"
#include "carddisplaylabel.h"
#include "cardspread.h"
#include "defines.h"

#include <QStyleOption>
#include <QPainter>

class CardMat : public QWidget
{
    Q_OBJECT

public:
    explicit CardMat(QWidget *parent = 0);

private:
    QGridLayout* mainLayout = new QGridLayout(this);

    QVector<CardDisplayDataCollection> cardDisplayCollections;
    QMultiMap<eSuit,SpellResult> harrowingSpellResults;
    int selectedSpread = -1;

    void prepareGrid(CardSpread* Mode);
    eMatchType getCardMatchType(CardDisplayDataContainer* C, Card* CurrentPick);
    SpellResult getHarrowingSpellResult(CardDisplayDataContainer* C, Card* CurrentPick);
    void setCardAlignmentByPercentage(CardDisplayDataContainer* C);
    bool setCardAlignmentByPosition(CardDisplayDataContainer* C);

protected:
    virtual void paintEvent(QPaintEvent* ) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

signals:
    void resetResultMat();
    void shuffleDeck();
    void getCard(Card *&OutCard);
    void getCurrentPick(Card *&Name);
    void sendResultDataBlock(CardResultDataBlock &Result);
    void getPercentage(float &Percentage);
    void swapCard(Card* OldCard, Card* NewCard);
    void resetContextMenu();
    void requestMenuOpen(CardDisplayLabel* source);
    void sendHarrowingDataBlock(QMultiMap<eSuit,SpellResult> &Result);
    void sendNeutralHarrowingResult();

public slots:
    void deal(CardSpread* Mode);
    void interpretMat(bool bRerollAlignment = true);
    void setCardAtLabel(CardDisplayLabel* source, Card* inCard);
    void toggleMisaligned(CardDisplayLabel* source);
    void toggleFacedown(CardDisplayLabel* source);
    void resetSpreadMat();
    void getMenuOpenRequest(CardDisplayLabel* source);

};

#endif // CARDMAT_H
