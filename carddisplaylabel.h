/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef CARDDISPLAYLABEL_H
#define CARDDISPLAYLABEL_H

#include <QLabel>

class CardDisplayDataContainer;

class CardDisplayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CardDisplayLabel(QWidget* parent = 0);
    CardDisplayDataContainer* dataContainer = nullptr;

signals:
    void requestContextMenu(CardDisplayLabel* Source);

public slots:
    void openContextMenu(const QPoint &) { emit requestContextMenu(this); }

};

#endif // CARDDISPLAYLABEL_H
