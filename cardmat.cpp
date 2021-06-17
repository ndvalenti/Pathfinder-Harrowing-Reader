/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "cardmat.h"
#include <QMapIterator>

CardMat::CardMat(QWidget *parent)
    : QWidget(parent)
{
    setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignHCenter);
    setMinimumSize(1000,1000);
}

void CardMat::prepareGrid(CardSpread* Mode)
{
    CardDisplayLabel* tempDisplayLabel;
    resetSpreadMat();
    selectedSpread = Mode->id;
    setMinimumSize(Mode->width, Mode->height);

    for (const auto &collection: Mode->cardCollections) {
        QVector<CardDisplayDataContainer*> tempCardDataCollection;
        for (const auto &card: collection.cards) {
            tempDisplayLabel = new CardDisplayLabel(this);
            connect(tempDisplayLabel, &CardDisplayLabel::requestContextMenu, this, &CardMat::getMenuOpenRequest);
            tempDisplayLabel->setFixedSize(Mode->cardWidth, Mode->cardHeight);

            CardDisplayDataContainer* tempDisplayData = new CardDisplayDataContainer(tempDisplayLabel);
            tempDisplayData->cardMetaData = card;
            tempDisplayData->bFacedown = tempDisplayData->cardMetaData.bFacedownDefault;

            mainLayout->addWidget(tempDisplayLabel, card.r, card.c, card.rSpan, card.cSpan);
            tempCardDataCollection.append(tempDisplayData);
        }
        CardDisplayDataCollection newGroupContainer(collection.name, tempCardDataCollection);
        cardDisplayCollections.append(newGroupContainer);
    }
}

eMatchType CardMat::getCardMatchType(CardDisplayDataContainer* C, Card* CurrentPick)
{
    int alignments = 0;
    int misalignments = 0;

    if (C->card == CurrentPick) return eMatchType::RoleCard;
    if (C->cardMetaData.alignment != -1 && C->cardMetaData.order == -1) return eMatchType::NoMatch;

    if (C->card->alignment != 1) {
        if (C->card->alignment == C->cardMetaData.r) {
            alignments++;
        } else if (C->cardMetaData.r != 1) {
            misalignments++;
        }
    }
    if (C->card->order != 1) {
        if (C->card->order == C->cardMetaData.c) {
            alignments++;
        } else if (C->cardMetaData.c != 1) {
            misalignments++;
        }
    }

    if (alignments == 2) return eMatchType::TrueMatch;
    if (alignments == 1) return eMatchType::PartialMatch;
    if (misalignments == 2) return eMatchType::OppositeMatch;
    return eMatchType::NoMatch;
}

SpellResult CardMat::getHarrowingSpellResult(CardDisplayDataContainer* C, Card* CurrentPick)
{
    int alignments = 0;
    int misalignments = 0;
    SpellResult alignResult;

    if (CurrentPick->alignment == 1 && CurrentPick->order == 1)
    {
        alignResult.harrowingBonus = 0;
        alignResult.bGreaterHarrowingBonus = false;
        alignResult.bValid = false;
        return alignResult;
    }

    if (C->card->alignment == CurrentPick->alignment) {
        alignResult.bGreaterHarrowingBonus = true;
        alignResult.bValid = true;
    }

    if (C->card->alignment != 1) {
        if (C->card->alignment == CurrentPick->alignment) {
            alignments++;
        } else if (CurrentPick->alignment != 1) {
            misalignments++;
        }
    }

    if (C->card->order != 1) {
        if (C->card->order == CurrentPick->order) {
            alignments++;
        } else if (CurrentPick->order != 1) {
            misalignments++;
        }
    }

    if (alignments == 2) {
        alignResult.harrowingBonus = 2;
        alignResult.bValid = true;
    } else if (alignments == 1) {
        alignResult.harrowingBonus = 1;
        alignResult.bValid = true;
    } else if (misalignments == 2) {
        alignResult.harrowingBonus = -1;
        alignResult.bValid = true;
    }

    return alignResult;
}

void CardMat::setCardAlignmentByPercentage(CardDisplayDataContainer* C)
{
    if (!qFuzzyIsNull(C->cardMetaData.misalignChance)) {
        float randRoll;
        std::clamp(C->cardMetaData.misalignChance, 0.0f, 1.0f);
        emit getPercentage(randRoll);
        C->bMisaligned = randRoll <= C->cardMetaData.misalignChance;
    }
}

bool CardMat::setCardAlignmentByPosition(CardDisplayDataContainer* C)
{
    if (C->cardMetaData.alignment != -1 && C->cardMetaData.order != -1) {
        if (C->type == eMatchType::OppositeMatch) {
            C->bMisaligned = true;
        } else if (C->card->alignment != 1 && C->cardMetaData.r != 1 && C->card->alignment != C->cardMetaData.r) {
            C->bMisaligned = true;
        } else if (C->card->order != 1 && C->cardMetaData.c != 1 && C->card->order != C->cardMetaData.c) {
            C->bMisaligned = true;
        } else {
            C->bMisaligned = false;
        }
        return true;
    }
    return false;
}

/* Begin Slots */

void CardMat::deal(CardSpread* Mode)
{
    if (Mode) {
        prepareGrid(Mode);
        emit resetContextMenu();

        for (const auto &collectionContainers : cardDisplayCollections) {
            for (const auto &container: collectionContainers.displayContainer) {
                if (container->cardMetaData.bChosen) {
                    emit getCurrentPick(container->card);
                } else {
                    emit getCard(container->card);
                }
            }
        }
        interpretMat(true);
    }
}

void CardMat::interpretMat(bool bRerollAlignment)
{
    bool bAlignBased = false;
    emit resetResultMat();
    harrowingSpellResults.clear();
    Card* currentPick;
    emit getCurrentPick(currentPick);

    if (cardDisplayCollections.count() > 0) {
        CardResultDataBlock chosen("Chosen");
        chosen.addCardData(currentPick, false, eMatchType::RoleCard);
        emit sendResultDataBlock(chosen);
    }

    for (const auto &i : cardDisplayCollections) {
        CardResultDataBlock currentBlock(i.name);
        for (const auto &it: i.displayContainer) {
            SpellResult spellResult = getHarrowingSpellResult(it, currentPick);
            if (spellResult.bValid) {
                harrowingSpellResults.insert(it->card->suit, spellResult);
            }
            if (!bAlignBased) {
                bAlignBased = setCardAlignmentByPosition(it);
            } else {
                setCardAlignmentByPosition(it);
            }

            if (bRerollAlignment) {
                setCardAlignmentByPercentage(it);
            }
            it->type = getCardMatchType(it, currentPick);

            if (it->bFacedown) {
                it->label->setPixmap(QPixmap::fromImage(it->card->backImage));
            } else {
                it->label->setPixmap(QPixmap::fromImage(it->card->image));

                if (it->bMisaligned) {
                    QTransform transform;
                    transform.rotate(180);
                    it->label->setPixmap(it->label->pixmap().transformed(transform));
                }
            }
            currentBlock.addCardData(it->card, it->bMisaligned, it->type);
        }
        emit sendResultDataBlock(currentBlock);
    }

    if (bAlignBased) {
        if (currentPick->alignment == 1 && currentPick->order == 1) {
            emit sendNeutralHarrowingResult();
        } else {
            emit sendHarrowingDataBlock(harrowingSpellResults);
        }
    }

}

void CardMat::setCardAtLabel(CardDisplayLabel* Source, Card* InCard)
{
    if (InCard && Source) {
        emit swapCard(Source->dataContainer->card, InCard);
        Source->dataContainer->card = InCard;
        interpretMat(false);
    }
}

void CardMat::toggleMisaligned(CardDisplayLabel* Source)
{
    if (Source) {
        Source->dataContainer->bMisaligned = !Source->dataContainer->bMisaligned;
        interpretMat(false);
    }
}

void CardMat::toggleFacedown(CardDisplayLabel* Source)
{
    if (Source) {
        Source->dataContainer->bFacedown = !Source->dataContainer->bFacedown;
        interpretMat(false);
    }
}

void CardMat::resetSpreadMat()
{
    for (auto &it: cardDisplayCollections) {
        for (auto &c: it.displayContainer) delete c->label;

        qDeleteAll(it.displayContainer);
        it.displayContainer.clear();
    }
    cardDisplayCollections.clear();

    emit shuffleDeck();
}

void CardMat::getMenuOpenRequest(CardDisplayLabel* Source)
{
    emit requestMenuOpen(Source);
}
