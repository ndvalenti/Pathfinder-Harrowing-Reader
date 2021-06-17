/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "contextmenu.h"
#include "cardpositioncontainer.h"

ContextMenu::ContextMenu(QWidget* parent)
    : QMenu(parent)
{
    connect(this, &ContextMenu::triggered, this, &ContextMenu::actionTriggered);
    screenshotAction->setText("Take Screenshot");
    addAction(screenshotAction);
    addSeparator();
}

void ContextMenu::addSuitSubmenu(eSuit Suit, QString Name)
{
    subMenus.insert(Suit, addMenu(Name));
}

void ContextMenu::insertItemBySuit(eSuit Suit, QString Name, int Index)
{
    QAction* action = new QAction(this);
    action->setData(Index);
    action->setText(Name);
    subMenus.value(Suit)->addAction(action);
    cardActions->addAction(action);
}

void ContextMenu::showMenu(CardDisplayLabel* Source)
{
    if (Source) {
        contextSource = Source;
        exec(QCursor::pos());
    }
}

void ContextMenu::actionTriggered(QAction* Action)
{
    if (Action == screenshotAction) {
        // TODO selection process for screenshot target at init rather than relying on outside input
        emit screenshot(contextSource);
    } else if (Action->actionGroup() == cardActions) {
        Card* card;
        emit requestCardAtIndex(Action->data().toInt(), card);
        emit replaceCard(contextSource, card);
    }
}


ContextMenuHeavy::ContextMenuHeavy(QWidget* parent)
    : ContextMenu(parent)
{}

void ContextMenuHeavy::addAdditionalActions()
{
    this->addSeparator();

    misalignAction->setCheckable(true);
    misalignAction->setText("Misaligned");
    addAction(misalignAction);

    flipAction->setCheckable(true);
    flipAction->setText("Facedown");
    addAction(flipAction);
}

void ContextMenuHeavy::showMenu(CardDisplayLabel* Source)
{
    if (Source) {
        misalignAction->setChecked(Source->dataContainer->bMisaligned);
        flipAction->setChecked(Source->dataContainer->bFacedown);
        ContextMenu::showMenu(Source);
    }
}

void ContextMenuHeavy::actionTriggered(QAction* Action)
{
    if (Action == screenshotAction) {
        emit screenshot(contextSource->parentWidget());
    } else if (Action->actionGroup() == cardActions) {
        Card* card;
        emit requestCardAtIndex(Action->data().toInt(), card);
        emit replaceCard(contextSource, card);
    } else if (Action == misalignAction) {
        emit toggleMisaligned(contextSource);
    } else if (Action == flipAction) {
        emit toggleFacedown(contextSource);
    }
}

void ContextMenuHeavy::setCardEnabled(int Index, eSuit Suit, bool BEnabled)
{
    for (const auto &i: subMenus.value(Suit)->actions()) {
        if (i->data().toInt() == Index) {
            i->setEnabled(BEnabled);
            return;
        }
    }
}

void ContextMenuHeavy::enableAllCards()
{
    // QMapIterator not required as we don't care about detaching data here
    for (const auto &it: qAsConst(subMenus)) {
        for (const auto &i: it->actions()) {
            i->setEnabled(true);
        }
    }
}
