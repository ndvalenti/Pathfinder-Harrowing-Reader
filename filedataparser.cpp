/*
 * Copyright (c) 2021 Nicholas Valenti. All rights reserved.
 * This project is supplied as-is with no warranty or guarantees.
 *
 * Licensed under GPLv3.
 */

#include "filedataparser.h"

FileDataParser::FileDataParser(QObject *parent)
    :QObject(parent)
{

}

void FileDataParser::parseSpreadFile(QString Source)
{
    QFile file(Source);
    file.open(QIODevice::ReadOnly);
    QJsonDocument jsDoc = QJsonDocument::fromJson(QString::fromUtf8(file.readAll()).simplified().toUtf8());

    QJsonArray jsonSpreads = jsDoc.array();
    for (const auto &itSpread: jsonSpreads)
    {
        QJsonObject jsSpreadObj = itSpread.toObject();
        int reference = jsSpreadObj["id"].toInt();

        CardSpread* newSpread = new CardSpread(reference, jsSpreadObj["name"].toString(), jsSpreadObj["description"].toString(),
                             jsSpreadObj["width"].toInt(), jsSpreadObj["height"].toInt(), jsSpreadObj["cardWidth"].toInt(),
                             jsSpreadObj["cardHeight"].toInt());

        QJsonArray cardGroups = jsSpreadObj["cardGroups"].toArray();
        for (const auto &group: cardGroups) {
            QJsonObject jsGroupObj = group.toObject();
            CardCollection collection(jsGroupObj["name"].toString());
            collection.id = jsGroupObj["id"].toInt();
            QJsonArray jsCardArr = jsGroupObj["cards"].toArray();

            for (const auto &itObject: jsCardArr) {
                QJsonObject jsCardObj = itObject.toObject();
                CardMetaData meta(jsCardObj["id"].toInt(), jsCardObj["row"].toInt(), jsCardObj["column"].toInt(),
                                  jsCardObj["rowspan"].toInt(), jsCardObj["colspan"].toInt());

                if (jsCardObj["facedown"] != QJsonValue::Null) {
                    meta.bFacedownDefault = jsCardObj["facedown"].toBool();
                }
                if (jsCardObj["chosen"] != QJsonValue::Null) {
                    meta.bChosen = jsCardObj["chosen"].toBool();
                }

                QJsonObject jsMisObj = jsCardObj["misalignment"].toObject();
                if (jsMisObj["randomChance"] != QJsonValue::Null) {
                    meta.misalignChance = jsMisObj["randomChance"].toDouble();
                }
                if (jsMisObj["positional"] != QJsonValue::Null) {
                    QJsonObject pos = jsMisObj["positional"].toObject();
                    meta.alignment = pos["alignment"].toInt();
                    meta.order = pos["order"].toInt();
                }
                collection.cards.append(meta);
            }
            newSpread->cardCollections.append(collection);
        }
        std::sort(newSpread->cardCollections.begin(), newSpread->cardCollections.end(),
                  [](CardCollection a, CardCollection b) { return a < b; });

        emit addSpread(newSpread);
    }
    file.close();
}

void FileDataParser::parseSuitFile(QString Source)
{
    QFile file(Source);
    file.open(QIODevice::ReadOnly);

    QJsonDocument jsDoc = QJsonDocument::fromJson(QString::fromUtf8(file.readAll()).simplified().toUtf8());
    QJsonArray jsSuitArr = jsDoc.array();

    for (const auto &it: jsSuitArr)
    {
        QJsonObject jsObj = it.toObject();
        Suit tempsuit(jsObj["name"].toString(), jsObj["domain"].toString(), jsObj["ability"].toString());
        int reference = jsObj["id"].toInt();

        emit addSuit(reference, tempsuit, jsObj["harrowing"].toString(),
                       jsObj["negativeHarrowing"].toString(), jsObj["greaterHarrowing"].toString());
    }
    file.close();
}

void FileDataParser::parseCardFile(QString Source)
{
    QFile file(Source);
    file.open(QIODevice::ReadOnly);

    QJsonDocument jsDoc = QJsonDocument::fromJson(QString::fromUtf8(file.readAll()).simplified().toUtf8());
    QJsonArray jsCardArr = jsDoc.array();

    for (const auto &it: jsCardArr)
    {
        QJsonObject jsObj = it.toObject();
        Card tempcard(jsObj["name"].toString(), jsObj["description"].toString(), jsObj["misaligned"].toString(), jsObj["bonus"].toString(),
                jsObj["alignment"].toInt(), jsObj["order"].toInt(), jsObj["suit"].toInt(), jsObj["path"].toString());
        if (tempcard.suit == eSuit::None)
        {
            tempcard.setBackImage(tempcard.image);
        }
        else
        {
            QString menuName = "The " + tempcard.name;

            if (tempcard.order == 0) menuName += " L";
            else if (tempcard.order == 1) menuName += " N";
            else if (tempcard.order == 2) menuName += " C";

            if (tempcard.alignment == 0) menuName += "G";
            else if (tempcard.alignment == 1 && tempcard.order != 1) menuName += "N";
            else if (tempcard.alignment == 2) menuName += "E";

            emit addCard(tempcard, menuName);
        }
    }
    file.close();
}
