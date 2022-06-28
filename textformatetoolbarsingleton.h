#ifndef TEXTFORMATETOOLBARSINGLETON_H
#define TEXTFORMATETOOLBARSINGLETON_H

#include <QToolBar>
#include "QPrintDialog"
#include "QFontDialog"
#include "QColorDialog"

#include "QTextDocument"
#include "QTextDocumentFragment"
#include "mainwindow.h"
#include "textdocumentview.h"

class TextFormateToolBarSingleton: public QToolBar
{
    Q_OBJECT
public:
    static TextFormateToolBarSingleton* getInstance(MainWindow* parent);
    bool isAdded() const {return added;}
    void setAdded() {added = true;}
    void retranslate();
    void setActionsChecked(Qt::Alignment alignment);
    void setTextFormatAlignment(Qt::Alignment alignment);
    std::optional<std::pair<TextDocumentView*,QTextCursor>> getCursorCurrentSubWindow() const;
    void addTextDocumentView(DocumentView *docView);
private:
    bool added;
    Qt::Alignment alignment_;
    QSet<TextDocumentView*> docViews;

private:
    TextFormateToolBarSingleton(MainWindow* parent);
    ~TextFormateToolBarSingleton() = default;
    TextFormateToolBarSingleton(const TextFormateToolBarSingleton& root) = delete;
    TextFormateToolBarSingleton& operator=(const TextFormateToolBarSingleton&) = delete;
    TextFormateToolBarSingleton(TextFormateToolBarSingleton&& root) = delete;
    TextFormateToolBarSingleton& operator=(TextFormateToolBarSingleton&&) = delete;

public slots:
    void textFormateLeft();
    void textFormateCenter();
    void textFormateRight();
    void textFormateWidth();
    void textFont();
    void textColorFont();
    void textColorBackground();
    void textClearFormat();
    void textCopyFormat();
    void activatedDocumentView(DocumentView* docView);
    void currentAlignment(Qt::Alignment alignment);
    void eraseTextDocumentView(TextDocumentView *txtDocView);
};

#endif // TEXTFORMATETOOLBARSINGLETON_H
