#ifndef LANGUAGEVISITOR_H
#define LANGUAGEVISITOR_H

#include <QTranslator>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class LanguageVisitor
{
public:
    virtual ~LanguageVisitor(){}
    void retranslate(MainWindow* mainWindow);


protected:
    QTranslator translator;
    virtual void fun();
};

class LanguageRussianVisitor: public LanguageVisitor
{
public:
    LanguageRussianVisitor();
protected:
    virtual void fun();
};

class LanguageEnglishVisitor: public LanguageVisitor
{
public:
    LanguageEnglishVisitor();
protected:
    virtual void fun();
};

#endif // LANGUAGEVISITOR_H
