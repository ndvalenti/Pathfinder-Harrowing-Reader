/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#ifndef DRAWFRAME_H
#define DRAWFRAME_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QMap>
#include <QFrame>
#include "suit.h"

class DrawFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DrawFrame(QWidget *parent = 0);

    void setInstructionText(QString In);
    void setButtonText(QString In);
    void setSelectorVisible(bool visible = true);
    int getComboBoxSelected();
    void setComboBoxSelected(int Selection);
    void addComboBoxItem(int Key, QString Name, QString Description);

private:
    QGridLayout* layout = new QGridLayout(this);
    QTextEdit* description = new QTextEdit(this);
    QComboBox* selector = new QComboBox(this);
    QPushButton* frameButton = new QPushButton(this);
    QLabel* selectorDesc = new QLabel(this);

    QMap<int, QString> descriptionPairs;

signals:
    void buttonPress();

public slots:
    void setDescriptionText(int Key);

};

#endif // DRAWFRAME_H
