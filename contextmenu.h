/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H
#include <QMenu>
#include <QMap>
#include <QString>
#include <QActionGroup>
#include "card.h"
#include "carddisplaylabel.h"

class ContextMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ContextMenu(QWidget* parent = 0);

    void addSuitSubmenu(eSuit Suit, QString Name);
    void insertItemBySuit(eSuit Suit, QString Name, int Index);

protected:
    QActionGroup* cardActions = new QActionGroup(this);
    QAction* screenshotAction = new QAction(this);

    QMap<eSuit, QMenu*> subMenus;
    CardDisplayLabel* contextSource;

signals:
    void screenshot(QWidget* Target);
    void requestCardAtIndex(int Index, Card *&OutCard);
    void replaceCard(CardDisplayLabel* Source, Card* OutCard);

public slots:
    virtual void showMenu(CardDisplayLabel* Source);
    virtual void actionTriggered(QAction* Action);
};

class ContextMenuHeavy : public ContextMenu
{
    Q_OBJECT
public:
    explicit ContextMenuHeavy(QWidget* parent = 0);

    void addAdditionalActions();

private:
    // TODO consider making this a modular construction...not now though
    QAction* misalignAction = new QAction(this);
    QAction* flipAction = new QAction(this);

signals:
    void toggleMisaligned(CardDisplayLabel* Source);
    void toggleFacedown(CardDisplayLabel* Source);

public slots:
    void showMenu(CardDisplayLabel* Source) override;
    void actionTriggered(QAction* Action) override;
    void setCardEnabled(int Index, eSuit Suit, bool BEnabled = true);
    void enableAllCards();
};

#endif // CONTEXTMENU_H
