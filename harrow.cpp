/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "harrow.h"
#include "ui_harrow.h"


Harrow::Harrow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Harrow)
{
    setCentralWidget(mainWidget);

    clipboard = QApplication::clipboard();

    mainWidget->setLayout(horizontalLayout);
    horizontalLayout->addWidget(controlMat);
    horizontalLayout->addWidget(cardDealMat);
    horizontalLayout->addWidget(resultDataMat);

    QFile styleFile(":/other/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    setStyleSheet(style);

    setSignalsSlots();
    initialize();
}

Harrow::~Harrow()
{
    delete ui;
}

void Harrow::initialize()
{
    rng = QRandomGenerator::securelySeeded();
    dataReader->parseSpreadFile(":/data/spreads.json");
    dataReader->parseSuitFile(":/data/suits.json");
    dataReader->parseCardFile(":/data/cards.json");
    cardDealContextMenu->addAdditionalActions();

    shuffleDeck();
    showMaximized();
}

void Harrow::setSignalsSlots()
{
    connect(dataReader, &FileDataParser::addSpread, this, &Harrow::spreadAdded);
    connect(dataReader, &FileDataParser::addSuit, this, &Harrow::suitAdded);
    connect(dataReader, &FileDataParser::addCard, this, &Harrow::cardAdded);
    connect(controlMat, &ControlMat::pickSingle, this, &Harrow::getRandomCardBySuit);
    connect(controlMat, &ControlMat::getSpread, this, &Harrow::getSpreadAt);
    connect(cardDealMat, &CardMat::getCard, this, &Harrow::dealTopCard);
    connect(cardDealMat, &CardMat::shuffleDeck, this, &Harrow::shuffleDeck);
    connect(cardDealMat, &CardMat::getPercentage, this, &Harrow::rollPercentage);
    connect(cardDealMat, &CardMat::swapCard, this, &Harrow::swapCardsInCurrentDeck);
    connect(chooseContextMenu, &ContextMenu::screenshot, this, &Harrow::clipboardScreenshotByWidget);
    connect(chooseContextMenu, &ContextMenu::requestCardAtIndex, this, &Harrow::getCardAtIndex);
    connect(cardDealContextMenu, &ContextMenuHeavy::requestCardAtIndex, this, &Harrow::getCardAtIndex);
    connect(cardDealContextMenu, &ContextMenuHeavy::screenshot, this, &Harrow::clipboardScreenshotByWidget);

    connect(this, &Harrow::addSuitToComboBox, controlMat, &ControlMat::addSuitToComboBox);
    connect(this, &Harrow::addSpreadToControlBox, controlMat, &ControlMat::addSpreadToComboBox);
    connect(cardDealMat, &CardMat::getCurrentPick, controlMat, &ControlMat::getCurrentPick);
    connect(chooseContextMenu, &ContextMenu::replaceCard, controlMat, &ControlMat::setRoleCard);

    connect(controlMat, &ControlMat::dealSpread, cardDealMat, &CardMat::deal);
    connect(controlMat, &ControlMat::resetMat, cardDealMat, &CardMat::resetSpreadMat);
    connect(controlMat, &ControlMat::reinterpret, cardDealMat, &CardMat::interpretMat);
    connect(cardDealContextMenu, &ContextMenuHeavy::toggleMisaligned, cardDealMat, &CardMat::toggleMisaligned);
    connect(cardDealContextMenu, &ContextMenuHeavy::toggleFacedown, cardDealMat, &CardMat::toggleFacedown);
    connect(cardDealContextMenu, &ContextMenuHeavy::replaceCard, cardDealMat, &CardMat::setCardAtLabel);

    connect(this, &Harrow::suitDataAdded, resultDataMat, &ResultDataMat::addHarrowData);
    connect(cardDealMat, &CardMat::sendResultDataBlock, resultDataMat, &ResultDataMat::addCardDataBlockResult);
    connect(cardDealMat, &CardMat::sendHarrowingDataBlock, resultDataMat, &ResultDataMat::addHarrowDataResults);
    connect(cardDealMat, &CardMat::resetResultMat, resultDataMat, &ResultDataMat::reset);
    connect(cardDealMat, &CardMat::sendNeutralHarrowingResult, resultDataMat, &ResultDataMat::addHarrowNeutralRoleResult);

    connect(this, &Harrow::setCardContextEnabled, cardDealContextMenu, &ContextMenuHeavy::setCardEnabled);
    connect(controlMat, &ControlMat::requestMenuOpen, chooseContextMenu, &ContextMenu::showMenu);
    connect(cardDealMat, &CardMat::resetContextMenu, cardDealContextMenu, &ContextMenuHeavy::enableAllCards);
    connect(cardDealMat, &CardMat::requestMenuOpen, cardDealContextMenu, &ContextMenuHeavy::showMenu);

}

/* Begin Slots */
void Harrow::spreadAdded(CardSpread* NewSpread)
{
    if (NewSpread) {
        spreads.insert(NewSpread->id, NewSpread);
        emit addSpreadToControlBox(NewSpread->id, NewSpread->name, NewSpread->description);
    }
}

void Harrow::suitAdded(int Id, Suit NewSuit, QString Harrowing, QString NegativeHarrowing, QString GreaterHarrowing)
{
    chooseContextMenu->addSuitSubmenu(static_cast<eSuit>(Id), NewSuit.name);
    cardDealContextMenu->addSuitSubmenu(static_cast<eSuit>(Id), NewSuit.name);
    emit addSuitToComboBox(Id, NewSuit.name + " (" + NewSuit.ability.mid(0,3) + ")", NewSuit.domain);
    emit suitDataAdded(static_cast<eSuit>(Id), NewSuit.name, Harrowing, NegativeHarrowing, GreaterHarrowing);
}

void Harrow::cardAdded(Card NewCard, QString MenuName)
{
    cards.append(NewCard);
    chooseContextMenu->insertItemBySuit(NewCard.suit, MenuName, cards.count() - 1);
    cardDealContextMenu->insertItemBySuit(NewCard.suit, MenuName, cards.count() - 1);
}

void Harrow::rollPercentage(float &RollResult)
{
    RollResult = rng.bounded(1.0f);
}

void Harrow::dealTopCard(Card *&OutCard)
{
    int cardId = currentDeckOrder.last();
    OutCard = &cards[cardId];
    currentDeckOrder.pop_back();
    emit setCardContextEnabled(cardId, OutCard->suit, false);
}

void Harrow::getRandomCardBySuit(Card *&OutCard, eSuit Suit)
{
    QVector<int> SuitedCards;
    for (int i = 0; i < cards.count(); i++)
    {
        if (cards[i].suit == Suit)
        {
            SuitedCards.append(i);
        }
    }
    int rand = rng.bounded(SuitedCards.count() - 1);
    OutCard = &cards[SuitedCards[rand]];
}

void Harrow::getCardAtIndex(int index, Card *&outCard)
{
    outCard = &cards[index];
}

void Harrow::swapCardsInCurrentDeck(Card* OldCard, Card* NewCard)
{
    if (OldCard && NewCard) {
        int oldId = cards.indexOf(*OldCard);
        int newId = cards.indexOf(*NewCard);
        int deckIndex = currentDeckOrder.indexOf(newId);
        currentDeckOrder.replace(deckIndex, oldId);
        emit setCardContextEnabled(oldId, OldCard->suit, true);
        emit setCardContextEnabled(newId, NewCard->suit, false);
    }
}

void Harrow::setCardByLabel(CardDisplayLabel* Source, Card* OutCard)
{
    if (Source && OutCard) {
        Source->dataContainer->card = OutCard;
    }
}

void Harrow::shuffleDeck()
{
    currentDeckOrder.resize(cards.size());
    for (int i = 0; i < currentDeckOrder.size(); i++)
    {
        currentDeckOrder[i] = i;
    }
    std::shuffle(currentDeckOrder.begin(), currentDeckOrder.end(), rng);
}

void Harrow::clipboardScreenshotByWidget(QWidget* Widget)
{
    if (Widget) {
        QImage bitmap(Widget->size(), QImage::Format_ARGB32);
        bitmap.fill(Qt::transparent);

        QPainter painter(&bitmap);
        Widget->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
        clipboard->setImage(bitmap);
    }
}

void Harrow::getSpreadAt(int Index, CardSpread *&Spread)
{
    Spread = spreads.value(Index);
}
