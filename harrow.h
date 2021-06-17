/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef HARROW_H
#define HARROW_H

#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include <QSharedPointer>
#include <QMap>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QPainter>
#include <QClipboard>
#include "defines.h"
#include "filedataparser.h"
#include "controlmat.h"
#include "cardmat.h"
#include "resultdatamat.h"
#include "contextmenu.h"
#include "cardspread.h"
#include "card.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Harrow; }
QT_END_NAMESPACE

class Harrow : public QMainWindow
{
    Q_OBJECT

public:
    Harrow(QWidget* parent = nullptr);
    ~Harrow();

private:
    /* UI elements */
    Ui::Harrow* ui;
    QWidget* mainWidget = new QWidget(this);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(mainWidget);
    ControlMat* controlMat = new ControlMat(this);
    CardMat* cardDealMat = new CardMat(this);
    ResultDataMat* resultDataMat = new ResultDataMat(this);
    ContextMenu* chooseContextMenu = new ContextMenu(this);
    ContextMenuHeavy* cardDealContextMenu = new ContextMenuHeavy(this);
    FileDataParser* dataReader = new FileDataParser(this);

    /* state storage */
    QClipboard* clipboard;
    QVector<Card> cards;
    QMap<int,CardSpread*> spreads;
    QVector<int> currentDeckOrder;
    QRandomGenerator rng;

    void initialize();
    void setSignalsSlots();


signals:
    void suitDataAdded(eSuit Suit, QString Name, QString HarrowData, QString NegativeHarrowData, QString GreaterHarrowData);
    void addSpreadToControlBox(int Key, QString Name, QString Description);
    void addSuitToComboBox(int Key, QString Name, QString Description);
    void setCardContextEnabled(int Index, eSuit Suit, bool bEnabled);
    void enableAllCardsContext();


public slots:
    void spreadAdded(CardSpread* NewSpread);
    void suitAdded(int Id, Suit NewSuit, QString Harrowing, QString NegativeHarrowing, QString GreaterHarrowing);
    void cardAdded(Card NewCard, QString MenuName);
    void rollPercentage(float &RollResult);
    void dealTopCard(Card *&OutCard);
    void getRandomCardBySuit(Card *&OutCard, eSuit Suit);
    void getCardAtIndex(int Index, Card *&OutCard);
    void swapCardsInCurrentDeck(Card* NewCard, Card* OldCard);
    void setCardByLabel(CardDisplayLabel* Source, Card* OutCard);
    void shuffleDeck();
    void clipboardScreenshotByWidget(QWidget* Target);
    void getSpreadAt(int Index, CardSpread *&Spread);

};
#endif // HARROW_H
