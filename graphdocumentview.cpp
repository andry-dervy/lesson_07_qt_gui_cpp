#include "graphdocumentview.h"
#include <QPixmap>
#include <QIcon>
#include <QTextStream>
#include <QEvent>
#include "QMdiSubWindow"
#include <QResizeEvent>
#include <QRectF>
#include <QDebug>
#include <QColor>

ControlPoint::ControlPoint(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x,y,w,h,parent)
{}

QRectF ControlPoint::boundingRect() const
{
    return QRectF(-40, -40, 40, 40);
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        qDebug() << "ControlPoint::mouseMoveEvent";
    }

    QGraphicsItem::mouseMoveEvent(event);
}

Line::Line(qreal ax1, qreal ay1, qreal ax2, qreal ay2, QGraphicsItem *parent)
    : QGraphicsItemGroup (parent)
    , x1(ax1)
    , y1(ay1)
    , x2(ax2)
    , y2(ay2)
{
    itemLine = new QGraphicsLineItem(x1,y1,x2,y2,this);
    addToGroup(itemLine);
    p_begin = new ControlPoint(x1,y1,4,4,this);
    p_begin->setFlags(ItemIsMovable|ItemIsSelectable);
    p_begin->setSelected(true);
    addToGroup(p_begin);
    p_end = new ControlPoint(x2,y2,4,4,itemLine);
    addToGroup(p_end);
}

void Line::setPen(QPen pen)
{
    Q_ASSERT(itemLine != nullptr);
    Q_ASSERT(p_begin != nullptr);
    Q_ASSERT(p_end != nullptr);

    itemLine->setPen(pen);
    p_begin->setPen(pen);
    p_end->setPen(pen);
}

Rect::Rect(qreal ax1, qreal ay1, qreal ax2, qreal ay2, QGraphicsItem *parent)
    : QGraphicsItemGroup (parent)
    , x1(ax1)
    , y1(ay1)
    , x2(ax2)
    , y2(ay2)
{
    rect = new QGraphicsRectItem(ax1,ay1,ax2 - ax1,ay2 - ay1);
    rect->setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
    addToGroup(rect);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
}

void Rect::setPen(QPen pen)
{
    Q_ASSERT(rect != nullptr);

    rect->setPen(pen);
}

QRectF Rect::boundingRect() const
{
    return QRectF(x1-5, y1-5, x2+5, y2+5);
}

void Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Rect::mouseMoveEvent: " << event->pos();
}

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mousePressEvent: " << event->pos();
}

void Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mouseReleaseEvent: " << event->pos();
}

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView (parent)
    ,currentItem(nullptr)
{}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    switch (event->buttons()) {
        case Qt::LeftButton: {
            if(m_isKeyPressed) {
                if(!currentItem) return;
                QPointF pos = mapToScene(event->pos());
                currentItem->moveBy(pos.x() - lastpos.x(),pos.y() - lastpos.y());
                lastpos = pos;
                qDebug() << "QEvent::MouseMove: " << pos;
            }
        }
        break;
        default:
        break;
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    auto docView = qobject_cast<GraphDocumentView*>(parent());
    Q_ASSERT(docView != nullptr);

    switch (event->button())
    {
        case Qt::LeftButton:
        {
            QPointF pos = mapToScene(event->pos());
            qDebug() << "QEvent::MouseButtonPress - " << "Qt::LeftButton: " << pos;
            switch(docView->getTypeGraphElement())
            {
                case GraphDocumentView::TypeGraphElement::Empty:
                {
                    m_isKeyPressed = true;
                    lastpos = mapToScene(event->pos());
                    auto item = itemAt(event->pos());
                    if(!item) return;
                    currentItem = item;
                    break;
                }
                case GraphDocumentView::TypeGraphElement::Line:
                case GraphDocumentView::TypeGraphElement::Rectangle:
                {
                    auto newitem = docView->getNewItem(pos.x()-10, pos.y()-10, pos.x()+10, pos.y()+10);
                    if(newitem) scene()->addItem(newitem);
                    break;
                }
            }
            break;
        }
        default:
        break;
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    auto docView = qobject_cast<GraphDocumentView*>(parent());
    Q_ASSERT(docView != nullptr);

    switch (event->button()) {
    case Qt::LeftButton:
    {
        m_isKeyPressed = false;
        currentItem = nullptr;

        QPointF pos = mapToScene(event->pos());
        qDebug() << "QEvent::MouseButtonRelease - " << "Qt::LeftButton: " << pos;
    }
    break;
    default:
    break;
    }

}

GraphDocumentView::GraphDocumentView(QWidget* parent)
    : DocumentView(parent)
    , typeGraphElement(TypeGraphElement::Empty)
    , isKeyPressed(false)
{
    auto layout = new QGridLayout(this);
    setLayout(layout);

    graphView = new GraphicsView(this);
    layout->addWidget(graphView,0,0,1,1);
//    auto graphViewEF = new GraphicsViewEventFilter(this);
//    graphView->installEventFilter(graphViewEF);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRect(0,0,10,10));
    graphView->setScene(scene);
    graphView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    //scene->installEventFilter(this);

    graphView->show();

    setWindowIcon(QPixmap(":/icons/new.png"));
    setWindowTitle(tr("Без названия"));

    currentPen.setColor(Qt::red);
    currentPen.setWidth(2);
}

bool GraphDocumentView::openDocument(QFile& file, ReadableWritable rw)
{
    Q_UNUSED(file);
    Q_UNUSED(rw);
    return false;
}

bool GraphDocumentView::saveDocument(QFile& file)
{
    Q_UNUSED(file);
    return false;
}

void GraphDocumentView::setReadableWritable(ReadableWritable aRW)
{
    Q_UNUSED(aRW);
}

void GraphDocumentView::print(QPrinter* printer) const
{
    Q_UNUSED(printer);
}

QGraphicsItemGroup* GraphDocumentView::getNewItem(qreal ax1, qreal ay1, qreal ax2, qreal ay2)
{
    switch(typeGraphElement)
    {
    case TypeGraphElement::Empty:
        qDebug() << "getNewItem: Empty";
        return nullptr;
    case TypeGraphElement::Line:
    {
        qDebug() << "getNewItem: Line";
        auto item = new Line(ax1,ay1,ax2,ay2);
        item->setPen(currentPen);
        return item;
    }
    case TypeGraphElement::Rectangle:
    {
        qDebug() << "getNewItem: Rectangle";
        auto item = new Rect(ax1,ay1,ax2,ay2);
        item->setPen(currentPen);
        return item;
    }
    }
    return nullptr;
}

bool GraphDocumentView::eventFilter(QObject* obj, QEvent* event)
{
    switch(event->type())
    {
        case QEvent::Close:
        {
            QMdiSubWindow* subWindow = dynamic_cast<QMdiSubWindow*>(obj);
            if(subWindow == nullptr) break;
            emit closeSubWnd(TypeDocumentView::GraphDocumentView);
            break;
        }
        default:
            break;
    }

    return QObject::eventFilter(obj, event);
}

DocumentView* CreatorGraphDocumentView::newDocumentView(QWidget* parent)
{
    return new GraphDocumentView(parent);
}

DocumentView* CreatorGraphDocumentView::openDocumentView(QString& fileName, QWidget* parent, ReadableWritable rw)
{
    Q_UNUSED(fileName);
    Q_UNUSED(parent);
    Q_UNUSED(rw);
    return nullptr;
}

bool CreatorGraphDocumentView::saveDocumentView(QString& fileName, DocumentView& docView)
{
    Q_UNUSED(fileName);
    Q_UNUSED(docView);
    return false;
}

void CreatorGraphDocumentView::fun() {}


