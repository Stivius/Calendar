#include "translationmodel.h"

#include <QApplication>

//====================================================================================

TranslationModel::TranslationModel(QObject *parent) :
    QObject(parent)
{
}

//====================================================================================

void TranslationModel::setLanguage(Language language)
{
    switch(language)
    {
    case Russian:
        qApp->removeTranslator(&_translator);
        break;
    case Ukrainian:
        _translator.load(":/translations/Calendar_ua.qm");
        qApp->installTranslator(&_translator);
        break;
    case English:
        _translator.load(":/translations/Calendar_en.qm");
        qApp->installTranslator(&_translator);
        break;
    }
    emit languageChanged(language);
}

//====================================================================================
