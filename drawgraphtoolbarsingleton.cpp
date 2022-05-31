#include "drawgraphtoolbarsingleton.h"
#include "toolbarelementsfactory.h"

extern template std::optional<QAction *> ToolbarElementsFactory<QAction>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);

DrawGraphToolBarSingleton* DrawGraphToolBarSingleton::getInstance(MainWindow* parent)
{
    static DrawGraphToolBarSingleton* theInstance =
            new DrawGraphToolBarSingleton(parent);
    return theInstance;
}

void DrawGraphToolBarSingleton::retranslate()
{
    ToolbarElementsFactory<QAction>::setText(this,"actDrawLine",tr("Линия"));
    ToolbarElementsFactory<QAction>::setText(this,"actDrawRectangle",tr("Прямоугольник"));
}

DrawGraphToolBarSingleton::DrawGraphToolBarSingleton(MainWindow *parent)
    : QToolBar(parent)
{
    auto act = ToolbarElementsFactory<QAction>::create(
                "actDrawLine", this,true,QPixmap(":/icons/graphics/line.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setDrawingLine()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                    "actDrawRectangle",this,true,QPixmap(":/icons/graphics/rectangle.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setDrawingRectangle()));
    addAction(*act);
}

void DrawGraphToolBarSingleton::clearActionsChecked(QAction* currentAct)
{
    auto act = findChild<QAction*>("actDrawLine");
    if(act && act != currentAct) act->setChecked(false);
    act = findChild<QAction*>("actDrawRectangle");
    if(act && act != currentAct) act->setChecked(false);
}

void DrawGraphToolBarSingleton::setDrawingElement(QString&& objName, GraphDocumentView::TypeGraphElement typeGraphElement)
{
    auto wnd = qobject_cast<MainWindow*>(parent());
    if(!wnd) return;
    auto optDocView = wnd->currentSubWindow();
    if(!optDocView) return;
    auto graphDocView = qobject_cast<GraphDocumentView*>(*optDocView);
    if(!graphDocView) return;

    auto act = findChild<QAction*>(objName);
    if(!act) return;

    clearActionsChecked(act);

    if(act->isChecked())
        graphDocView->setTypeGraphElement(typeGraphElement);
    else
        graphDocView->setTypeGraphElement(GraphDocumentView::TypeGraphElement::Empty);
}

void DrawGraphToolBarSingleton::setDrawingLine()
{
    setDrawingElement("actDrawLine",GraphDocumentView::TypeGraphElement::Line);
}

void DrawGraphToolBarSingleton::setDrawingRectangle()
{
    setDrawingElement("actDrawRectangle",GraphDocumentView::TypeGraphElement::Rectangle);
}

void DrawGraphToolBarSingleton::activatedDocumentView(DocumentView* docView)
{

}
