/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CONTROLMAT_H
#define CONTROLMAT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPixmap>
#include <QPushButton>
#include <QImage>
#include <QString>
#include <QFrame>
#include "cardpositioncontainer.h"
#include "drawframe.h"
#include "cardspread.h"

class ControlMat : public QWidget
{
    Q_OBJECT

public:
    explicit ControlMat(QWidget *parent = 0);

    CardDisplayLabel* cardLabel = new CardDisplayLabel(this);
    CardDisplayDataContainer* cardContainer = new CardDisplayDataContainer(cardLabel);

private:
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    DrawFrame* chooseControlPanel = new DrawFrame(this);
    DrawFrame* dealControlPanel = new DrawFrame(this);
    Card* currentPick;

    void updateCurrentPick(Card* Card);
    void setDrawImage(QImage Image);
    void setDrawImageFromPath(QString Path);

signals:
    void dealSpread(CardSpread* Mode);
    void getSpread(int Index, CardSpread *&Spread);
    void pickSingle(Card *&OutCard, eSuit Suit);
    void resetMat();
    void reinterpret(bool BRecalculateAlignment);
    void takeScreenshot(QWidget *Widget);
    void requestMenuOpen(CardDisplayLabel* Source);

public slots:
    void addSuitToComboBox(int Key, QString Name, QString Description);
    void addSpreadToComboBox(int Key, QString Name, QString Description);
    void drawRole();
    void setRoleCard(CardDisplayLabel*, Card* InCard);
    void signalDealSpread();
    void getCurrentPick(Card *&Name);
    void getMenuOpenRequest(CardDisplayLabel* Source);

};

#endif // CONTROLMAT_H
