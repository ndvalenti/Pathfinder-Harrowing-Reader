/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "controlmat.h"

ControlMat::ControlMat(QWidget *parent)
    : QWidget(parent)
{
    setLayout(mainLayout);
    setFixedWidth(300);
    setMinimumHeight(900);

    currentPick = nullptr;

    mainLayout->setAlignment(Qt::AlignCenter);
    cardLabel->setScaledContents(true);
    cardLabel->setFixedSize(182,300);
    connect(cardLabel, &CardDisplayLabel::requestContextMenu, this, &ControlMat::getMenuOpenRequest);

    chooseControlPanel->setInstructionText("Press choose by selecting the applicable attribute and then pressing \"Choose\" to pick a card from the relevant suit.");
    chooseControlPanel->setButtonText("Choose");
    chooseControlPanel->setSelectorVisible();

    dealControlPanel->setInstructionText("Once the principle card is chosen press \"Deal\" to deal and interpret the Harrow spread.");
    dealControlPanel->setButtonText("Deal");
    dealControlPanel->setSelectorVisible();

    connect(chooseControlPanel, &DrawFrame::buttonPress, this, &ControlMat::drawRole);
    connect(dealControlPanel, &DrawFrame::buttonPress, this, &ControlMat::signalDealSpread);

    mainLayout->addWidget(cardLabel);
    mainLayout->addWidget(chooseControlPanel);
    mainLayout->addWidget(dealControlPanel);

}

void ControlMat::updateCurrentPick(Card* Card)
{
    currentPick = Card;
    setDrawImage(currentPick->image);
}

void ControlMat::setDrawImage(QImage Image)
{
    cardLabel->setPixmap(QPixmap::fromImage(Image));
}

void ControlMat::setDrawImageFromPath(QString Path)
{
    QImage Image(Path);
    setDrawImage(Image);
}

void ControlMat::addSuitToComboBox(int Key, QString Name, QString Description)
{
    chooseControlPanel->addComboBoxItem(Key, Name, Description);
}

void ControlMat::addSpreadToComboBox(int Key, QString Name, QString Description)
{
    dealControlPanel->addComboBoxItem(Key, Name, Description);
}

void ControlMat::drawRole()
{
    eSuit suit = static_cast<eSuit>(chooseControlPanel->getComboBoxSelected());
    Card* card;
    emit pickSingle(card, suit);
    emit resetMat();
    setRoleCard(cardLabel, card);
}

void ControlMat::setRoleCard(CardDisplayLabel*, Card* InCard)
{
    if (InCard) {
        chooseControlPanel->setComboBoxSelected(static_cast<int>(InCard->suit));
        updateCurrentPick(InCard);
        emit reinterpret(false);
    }
}

void ControlMat::signalDealSpread()
{
    if (currentPick == nullptr) {
        drawRole();
    }

    int index = dealControlPanel->getComboBoxSelected();
    CardSpread* cardSpread;

    emit getSpread(index, cardSpread);
    emit dealSpread(cardSpread);
}

void ControlMat::getCurrentPick(Card *&Name)
{
    Name = currentPick;
}

void ControlMat::getMenuOpenRequest(CardDisplayLabel* Source)
{
    emit requestMenuOpen(Source);
}
