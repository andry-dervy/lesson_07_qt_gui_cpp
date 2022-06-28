#include "drawgraphtoolbarsingleton.h"
#include "toolbarelementsfactory.h"
#include "QDebug"

extern template std::optional<QAction *> ToolbarElementsFactory<QAction>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);
extern template void ToolbarElementsFactory<QAction>::setText(QObject* parent,const QString&& nameObject,const QString&& text);
extern template std::optional<QComboBox *> ToolbarElementsFactory<QComboBox>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);
extern template void ToolbarElementsFactory<QComboBox>::setText(QObject* parent,const QString&& nameObject,const QString&& text);

DrawGraphToolBarSingleton* DrawGraphToolBarSingleton::getInstance(MainWindow* parent)
{
    static DrawGraphToolBarSingleton* theInstance =
            new DrawGraphToolBarSingleton(parent);
    return theInstance;
}

void DrawGraphToolBarSingleton::retranslate()
{
    ToolbarElementsFactory<QAction>::setText(this,"actDrawElipse",tr("Элипс"));
    ToolbarElementsFactory<QAction>::setText(this,"actDrawRectangle",tr("Прямоугольник"));
    ToolbarElementsFactory<QAction>::setText(this,"actDrawStar",tr("Звезда"));
    ToolbarElementsFactory<QComboBox>::setText(this,"cbWidthPen",tr("Толщина пера"));
    ToolbarElementsFactory<QAction>::setText(this,"actColorGraphItemBorder",tr("Цвет границы элемента"));
    ToolbarElementsFactory<QAction>::setText(this,"actColorGraphItem",tr("Цвет элемента"));
}

DrawGraphToolBarSingleton::DrawGraphToolBarSingleton(MainWindow *parent)
    : QToolBar(parent)
{
    auto act = ToolbarElementsFactory<QAction>::create(
                "actDrawElipse", this,true,QPixmap(":/icons/graphics/elipse.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setDrawingElipse()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                    "actDrawRectangle",this,true,QPixmap(":/icons/graphics/rectangle.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setDrawingRectangle()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                    "actDrawStar",this,true,QPixmap(":/icons/graphics/star.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setDrawingStar()));
    addAction(*act);

    auto combo = ToolbarElementsFactory<QComboBox>::create(
                    "cbWidthPen",this,false,QPixmap());
    Q_ASSERT(combo != nullptr);

    QStringList listItems;
    listItems << "1" << "2" << "3";
    (*combo)->addItems(listItems);
    connect(*combo, QOverload<int>::of(&QComboBox::activated),
        this,&DrawGraphToolBarSingleton::setWidthPen);
    addWidget(*combo);

    act = ToolbarElementsFactory<QAction>::create(
                    "actColorGraphItemBorder",this,false,QPixmap(":/icons/graphics/colorgraphitemborder.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setColorBorder()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                    "actColorGraphItem",this,false,QPixmap(":/icons/graphics/colorgraphitem.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(setColor()));
    addAction(*act);
}

std::optional<GraphDocumentView*> DrawGraphToolBarSingleton::getCurrentGraphDocView() const
{
    auto wnd = qobject_cast<MainWindow*>(parent());
    if(!wnd) return std::nullopt;
    auto optDocView = wnd->currentSubWindow();
    if(!optDocView) return std::nullopt;
    auto graphDocView = qobject_cast<GraphDocumentView*>(*optDocView);
    if(!graphDocView) return std::nullopt;

    return std::make_optional<GraphDocumentView*>(graphDocView);
}

void DrawGraphToolBarSingleton::setDrawingElement(QString&& objName, GraphDocumentView::TypeGraphElement typeGraphElement)
{
    auto graphDocView = getCurrentGraphDocView();
    if(!graphDocView) return;

    if(ToolbarElementsFactory<QAction>::isChecked(this,std::move(objName)))
    {
        (*graphDocView)->setTypeGraphElement(typeGraphElement);
        setActionsChecked(typeGraphElement);
        return;
    }

    (*graphDocView)->setTypeGraphElement(GraphDocumentView::TypeGraphElement::Empty);
    setActionsChecked(GraphDocumentView::TypeGraphElement::Empty);
}

void DrawGraphToolBarSingleton::setActionsChecked(GraphDocumentView::TypeGraphElement typeGraphElement)
{
    qDebug() << "setActionsChecked(" << typeGraphElement <<")";

    ToolbarElementsFactory<QAction>::setChecked(this,"actDrawElipse",false);
    ToolbarElementsFactory<QAction>::setChecked(this,"actDrawRectangle",false);
    ToolbarElementsFactory<QAction>::setChecked(this,"actDrawStar",false);

    switch (typeGraphElement) {
    case GraphDocumentView::TypeGraphElement::Ellipse:
        ToolbarElementsFactory<QAction>::setChecked(this,"actDrawElipse",true);
        break;
    case GraphDocumentView::TypeGraphElement::Rectangle:
        ToolbarElementsFactory<QAction>::setChecked(this,"actDrawRectangle",true);
        break;
    case GraphDocumentView::TypeGraphElement::Star:
        ToolbarElementsFactory<QAction>::setChecked(this,"actDrawStar",true);
        break;
    case GraphDocumentView::TypeGraphElement::Empty:
        break;
    }
}

void DrawGraphToolBarSingleton::setDrawingElipse()
{
    setDrawingElement("actDrawElipse",GraphDocumentView::TypeGraphElement::Ellipse);
}

void DrawGraphToolBarSingleton::setDrawingRectangle()
{
    setDrawingElement("actDrawRectangle",GraphDocumentView::TypeGraphElement::Rectangle);
}

void DrawGraphToolBarSingleton::setDrawingStar()
{
    setDrawingElement("actDrawStar",GraphDocumentView::TypeGraphElement::Star);
}

void DrawGraphToolBarSingleton::setColor()
{
    auto graphDocView = getCurrentGraphDocView();
    if(!graphDocView) return;

    (*graphDocView)->setColorBrush(QColorDialog::getColor());
}

void DrawGraphToolBarSingleton::setColorBorder()
{
    auto graphDocView = getCurrentGraphDocView();
    if(!graphDocView) return;

    (*graphDocView)->setPenColor(QColorDialog::getColor());
}

void DrawGraphToolBarSingleton::setWidthPen(int index)
{
    auto graphDocView = getCurrentGraphDocView();
    if(!graphDocView) return;

    switch(index)
    {
    case 0:
        (*graphDocView)->setPenWidth(2);
        break;
    case 1:
        (*graphDocView)->setPenWidth(5);
        break;
    case 2:
        (*graphDocView)->setPenWidth(10);
        break;
    }
}

void DrawGraphToolBarSingleton::activatedDocumentView(DocumentView* docView)
{
    if(!docView) return;
    auto graphDocView = qobject_cast<GraphDocumentView*>(docView);
    if(!graphDocView) return;

    //setActionsChecked(textDocView->te().alignment());
}
