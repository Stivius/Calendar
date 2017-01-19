#ifndef TRANSLATIONMODEL_H
#define TRANSLATIONMODEL_H

#include <QTranslator>

enum Language {
    Russian,
    Ukrainian,
    English
};

class TranslationModel : public QObject
{
    Q_OBJECT
signals:
    void languageChanged(Language language);

public:
    explicit TranslationModel(QObject *parent = 0);
    void setLanguage(Language language);

private:
    QTranslator _translator;

};

#endif // TRANSLATIONMODEL_H
