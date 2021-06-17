/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "resultdatamat.h"

//TODO: Define colors somewhere

ResultDataMat::ResultDataMat(QWidget *parent)
    : QFrame(parent)
{
    mat->setAlignment(Qt::AlignTop);
    setLayout(mat);
    setFixedWidth(500);
    setMinimumHeight(900);
    mat->addWidget(tab);
}

void ResultDataMat::addCardDataBlockResult(CardResultDataBlock &Result)
{
    QFrame* layoutContainer = new QFrame(this);
    wRecord.append(layoutContainer);
    QVBoxLayout* boxLayout = new QVBoxLayout(layoutContainer);
    lRecord.append(boxLayout);

    CardFrame* tempFrame;

    foreach (auto c, Result.cardArray) {
        tempFrame = new CardFrame(this);
        QString s = "";
        if (c.type == eMatchType::RoleCard){
            s += "<b><p style=\"font-size:14px;\">Role Card</p></b>";
        } else if (c.type == eMatchType::TrueMatch) {
            s += "<b><p style=\"font-size:14px;\">True Match</p></b>";
        } else if (c.type == eMatchType::PartialMatch) {
            s += "<b><p style=\"font-size:14px;\">Partial Match</p></b>";
        } else if (c.type == eMatchType::OppositeMatch) {
            s += "<b><p style=\"font-size:14px;\">Opposite Match</p></b>";
        }
        s += "<b>" + c.name + "</b><br>" + c.description;
        if (c.bMisaligned) {
            s += "<br><i>" + c.misaligned + "</i>";
        }
        tempFrame->setText(s);
        wRecord.append(tempFrame);
        boxLayout->addWidget(tempFrame);
    }

    boxLayout->setAlignment(Qt::AlignHCenter);
    boxLayout->setSizeConstraint(QLayout::SetMinimumSize);
    boxLayout->addStretch();

    layoutContainer->setLayout(boxLayout);
    layoutContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    tab->addTab(layoutContainer, Result.section);
}

void ResultDataMat::addHarrowNeutralRoleResult()
{
    QFrame* spellFrame = new QFrame(this);
    wRecord.append(spellFrame);
    QVBoxLayout* spellLayout = new QVBoxLayout(spellFrame);
    lRecord.append(spellLayout);

    CardFrame* frameTemp = new CardFrame();
    frameTemp->setText("<b>Neutral Role Card</b><br>Neutral has no natural opposition. Replace role card with one of alignment LG, LE, CG, or CE and use those results.");
    wRecord.append(frameTemp);
    spellLayout->addWidget(frameTemp);

    spellLayout->setAlignment(Qt::AlignHCenter);
    spellLayout->setSizeConstraint(QLayout::SetMinimumSize);
    spellLayout->addStretch();
    spellFrame->setLayout(spellLayout);
    spellFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    tab->addTab(spellFrame, "Harrowing");
}

void ResultDataMat::addHarrowDataResults(QMultiMap<eSuit,SpellResult> &Result)
{
    if (Result.count() > 0) {
        QFrame* harrowFrame = new QFrame(this);
        wRecord.append(harrowFrame);
        QVBoxLayout* harrowLayout = new QVBoxLayout(harrowFrame);
        lRecord.append(harrowLayout);

        QMultiMap<int, HarrowResult> standardResults;
        QMap<eSuit, HarrowResult> greaterResults;

        CardFrame* harrowTemp = new CardFrame();
        CardFrame* greaterTemp = new CardFrame();

        QMultiMapIterator<eSuit,SpellResult> i(Result);
        while (i.hasNext()) {
            i.next();
            QString tempText;
            if (i.value().harrowingBonus != 0) {
                HarrowResult tempResult;
                if (i.value().harrowingBonus > 0) {
                    tempText = "+" + QString::number(i.value().harrowingBonus) + " " + suitData.value(i.key()).harrowing;
                    tempResult.description = tempText + "<br>";
                    standardResults.insert(i.value().harrowingBonus, tempResult);
                } else {
                    tempText = QString::number(i.value().harrowingBonus) + " " + suitData.value(i.key()).negativeHarrowing;
                    tempResult.description = tempText + "<br>";
                    standardResults.insert(i.value().harrowingBonus, tempResult);
                }
            }
            if (i.value().bGreaterHarrowingBonus) {
                HarrowResult tempResult;
                tempText = suitData.value(i.key()).greaterHarrowing;
                tempResult.description = tempText + "<br>";
                greaterResults.insert(i.key(), tempResult);
            }
        }

        QString resultStr = "";
        if (standardResults.count() > 0) {
            resultStr = "<b>Harrowing</b><br>Bonuses may be used to modify a single roll, penalties apply for the duration<br><br>";
        }
        QMultiMapIterator<int, HarrowResult> j(standardResults);
        j.toBack();
        while (j.hasPrevious()) {
            j.previous();
            resultStr += j.value().description;
        }
        harrowTemp->setText(resultStr);

        resultStr = "";
        if (greaterResults.count() > 0) {
            resultStr = "<b>Harrowing, Greater</b><br>Choose one positive effect. During the spell's duration the effect can be used one time to gain the listed benefits for 1 minute.<br><br>";
        }
        QMapIterator<eSuit, HarrowResult> k(greaterResults);
        while (k.hasNext()) {
            k.next();
            resultStr += k.value().description;
        }
        greaterTemp->setText(resultStr);

        if (harrowTemp->hasContent()) {
            harrowTemp->setHeight(350);
            wRecord.append(harrowTemp);
            harrowLayout->addWidget(harrowTemp);
        } else delete harrowTemp;
        if (greaterTemp->hasContent()) {
            greaterTemp->setHeight(350);
            wRecord.append(greaterTemp);
            harrowLayout->addWidget(greaterTemp);
        } else delete greaterTemp;


        harrowLayout->setAlignment(Qt::AlignHCenter);
        harrowLayout->setSizeConstraint(QLayout::SetMinimumSize);
        harrowLayout->addStretch();
        harrowFrame->setLayout(harrowLayout);
        harrowFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        tab->addTab(harrowFrame, "Harrowing");
    }
}

void ResultDataMat::addHarrowData(eSuit Suit, QString Name, QString Harrowing, QString NegativeHarrowing, QString GreaterHarrowing)
{
    HarrowData Data;
    Data.name = Name;
    Data.harrowing = Harrowing;
    Data.negativeHarrowing = NegativeHarrowing;
    Data.greaterHarrowing = GreaterHarrowing;
    suitData.insert(Suit, Data);
}

void ResultDataMat::reset()
{
    for (int i = wRecord.count() - 1; i >= 0; i--) {
        wRecord[i]->close();
        delete wRecord[i];
    }
    wRecord.clear();
    tab->clear();
}
