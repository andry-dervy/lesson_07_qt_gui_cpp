#ifndef DRAWGRAPHTOOLBARSINGLETON_H
#define DRAWGRAPHTOOLBARSINGLETON_H

#include <QToolBar>
#include "QPrintDialog"
#include "QFontDialog"
#include "QColorDialog"
#include <QAction>

#include "mainwindow.h"
#include "graphdocumentview.h"

class DrawGraphToolBarSingleton : public QToolBar
{
    Q_OBJECT
public:
    static DrawGraphToolBarSingleton* getInstance(MainWindow* parent);
    bool isAdded() const {return added;}
    void setAdded() {added = true;}
    void retranslate();

    void clearActionsChecked(QAction* currentAct);
private:
    bool added;
    Qt::Alignment alignment_;
private:
    DrawGraphToolBarSingleton(MainWindow* parent);
    ~DrawGraphToolBarSingleton() = default;
    DrawGraphToolBarSingleton(const DrawGraphToolBarSingleton& root) = delete;
    DrawGraphToolBarSingleton& operator=(const DrawGraphToolBarSingleton&) = delete;
    DrawGraphToolBarSingleton(DrawGraphToolBarSingleton&& root) = delete;
    DrawGraphToolBarSingleton& operator=(DrawGraphToolBarSingleton&&) = delete;

    void setDrawingElement(QString&& objName, GraphDocumentView::TypeGraphElement typeGraphElement);

public slots:
    void setDrawingLine();
    void setDrawingRectangle();
    void activatedDocumentView(DocumentView* docView);

signals:

};

#endif // DRAWGRAPHTOOLBARSINGLETON_H
