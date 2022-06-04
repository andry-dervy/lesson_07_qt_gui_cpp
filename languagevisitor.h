#ifndef LANGUAGEVISITOR_H
#define LANGUAGEVISITOR_H

#include <QTranslator>
#include "mainwindow.h"
#include "ui_mainwindow.h"

enum class LANGUAGE
{
    Russian, English
};

class LanguageVisitor
{
public:
    virtual ~LanguageVisitor() {};
    void retranslate(MainWindow* mainWindow);
    bool isLanguage(LANGUAGE l) const {return lang == l? true : false;}
protected:
    QTranslator translator;
    LANGUAGE lang;
    virtual void fun();
};

class LanguageRussianVisitor: public LanguageVisitor
{
public:
    LanguageRussianVisitor();
    ~LanguageRussianVisitor() override {}
protected:
    void fun() override;
};

class LanguageEnglishVisitor: public LanguageVisitor
{
public:
    LanguageEnglishVisitor();
    ~LanguageEnglishVisitor() override {}
protected:
    void fun() override;
};

#endif // LANGUAGEVISITOR_H
