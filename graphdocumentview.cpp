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

BaseGraphicsItem::BaseGraphicsItem(QGraphicsScene* scene, QGraphicsItem* parent)
    :QGraphicsItem(parent)
{
    Q_ASSERT(scene != nullptr);
    scene->addItem(this);
}

ControlPoint::ControlPoint(QGraphicsScene* scene, QGraphicsItem *parent)
    : BaseGraphicsItem(scene, parent)
{}

QRectF ControlPoint::boundingRect() const
{
    return QRectF(-SZ, -SZ, SZ, SZ);
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        qDebug() << "ControlPoint::mouseMoveEvent";
    }

    QGraphicsItem::mouseMoveEvent(event);
}

Line::Line(qreal ax1, qreal ay1, qreal ax2, qreal ay2, QGraphicsScene* scene, QGraphicsItem *parent)
    : BaseGraphicsItem(scene, parent)
    , x1(ax1)
    , y1(ay1)
    , x2(ax2)
    , y2(ay2)
{
//    itemLine = new QGraphicsLineItem(x1,y1,x2,y2,this);
//    //addToGroup(itemLine);
//    p_begin = new ControlPoint(this);
//    p_begin->setFlags(ItemIsMovable|ItemIsSelectable);
//    p_begin->setSelected(true);
//    //addToGroup(p_begin);
//    p_end = new ControlPoint(itemLine);
//    //addToGroup(p_end);
}

void Line::setPen(QPen pen)
{
    Q_ASSERT(itemLine != nullptr);
    Q_ASSERT(p_begin != nullptr);
    Q_ASSERT(p_end != nullptr);

    itemLine->setPen(pen);
//    p_begin->setPen(pen);
    //    p_end->setPen(pen);
}

QRectF Line::boundingRect() const
{
    return QRectF();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

Rect::Rect(qreal ax1, qreal ay1, qreal ax2, qreal ay2, QGraphicsScene* scene, QGraphicsItem *parent)
    : BaseGraphicsItem(scene, parent)
    , x1(ax1)
    , y1(ay1)
    , x2(ax2)
    , y2(ay2)
    , hoverEntered(false)
{
    w = x2 - x1;
    h = y2 - y1;

    rect = new QGraphicsRectItem(this);
    rect->setRect(x1,y1,w,h);
    rect->setPen(QPen(Qt::blue));

    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

void Rect::setPen(QPen aPen)
{
    pen = aPen;
}

void Rect::resize(qreal x2, qreal y2)
{
    qreal weight = x2 - x1;
    qreal hight = y2 - y1;

    if(weight >= 0 && hight >= 0)
    {
        prepareGeometryChange();
        w = weight;
        h = hight;

        setPos(x1 + weight, y1 + hight);

        rect->setRect(-w,-h,w,h);

        update();
        qDebug() << "x1 " << x1 << " x2 " << x2 << " y1 " << y1 << " y2 " << y2;
        qDebug() << "Rect::resize: " << w << " " << h;
    }
}

QRectF Rect::boundingRect() const
{
    return QRectF(-w, -h, w, h);
}

void Rect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen);

    QRectF bound = boundingRect();
//    painter->drawRect(bound);
    //painter->draw;

    if(hoverEntered)
    {
//        painter->drawRect(bound);
//        QRectF rUpLeft = QRectF(bound.x(),bound.y(),SZ,SZ);
//        painter->fillRect(rUpLeft,Qt::black);
//        QRectF rUpRight = QRectF(bound.x()+w-SZ,bound.y(),SZ,SZ);
//        painter->fillRect(rUpRight,Qt::black);
//        QRectF rDownLeft = QRectF(bound.x(),bound.y()+h-SZ,SZ,SZ);
//        painter->fillRect(rDownLeft,Qt::black);
//        QRectF rDownRight = QRectF(bound.x()+w-SZ,bound.y()+h-SZ,SZ,SZ);
//        painter->fillRect(rDownRight,Qt::black);
    }
    else
    {
//        painter->drawRect(bound);
    }
}

void Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Rect::mouseMoveEvent: " << event->pos();
    //update();
    QGraphicsItem::mouseMoveEvent(event);
}

void Rect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mousePressEvent: " << event->pos();
    //update();



    QGraphicsItem::mousePressEvent(event);
}

void Rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Rect::mouseReleaseEvent: " << event->pos();
    //update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Rect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverEnterEvent: " << event->pos();
    hoverEntered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverLeaveEvent: " << event->pos();
    hoverEntered = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Rect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverMoveEvent: " << event->pos();

    QGraphicsItem::hoverMoveEvent(event);
}

GraphicsView::GraphicsView(QWidget *parent)
    :QGraphicsView (parent)
    ,currentItem(nullptr)
{}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    switch (event->buttons()) {
        case Qt::LeftButton:
        {
            if(!currentItem) break;

            QPointF pos = mapToScene(event->pos());
            currentItem->resize(pos.x(), pos.y());
            qDebug() << "QEvent::MouseMove: " << pos << event->pos();
        }
        break;
        default:
        break;
    }
    QGraphicsView::mouseMoveEvent(event);
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
            qDebug() << "event->pos() " << event->pos();
            QPointF posGl = mapToGlobal(event->pos());
            qDebug() << "posGl " << posGl;
            switch(docView->getTypeGraphElement())
            {
                case GraphDocumentView::TypeGraphElement::Empty:
                {
                    break;
                }
                case GraphDocumentView::TypeGraphElement::Line:
                case GraphDocumentView::TypeGraphElement::Rectangle:
                {
                    auto newitem = docView->getNewItem(pos.x(), pos.y(), pos.x(), pos.y());
                    newitem->setPos(pos);
                    if(newitem)
                    {
                        //scene()->addItem(newitem);
                        currentItem = newitem;
                        lastpos = pos;
                    }
                    break;
                }
            }
            break;
        }
        default:
        break;
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    auto docView = qobject_cast<GraphDocumentView*>(parent());
    Q_ASSERT(docView != nullptr);

    switch (event->button()) {
    case Qt::LeftButton:
    {
        currentItem = nullptr;
        QPointF pos = mapToScene(event->pos());
        qDebug() << "QEvent::MouseButtonRelease - " << "Qt::LeftButton: " << pos;
    }
    break;
    default:
    break;
    }
    QGraphicsView::mouseReleaseEvent(event);
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
    scene->setSceneRect(QRect(0,0,100,100));
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

BaseGraphicsItem* GraphDocumentView::getNewItem(qreal ax1, qreal ay1, qreal ax2, qreal ay2)
{
    switch(typeGraphElement)
    {
    case TypeGraphElement::Empty:
        qDebug() << "getNewItem: Empty";
        return nullptr;
    case TypeGraphElement::Line:
    {
        qDebug() << "getNewItem: Line";
        auto item = new Line(ax1,ay1,ax2,ay2,scene);
        item->setPen(currentPen);
        return item;
    }
    case TypeGraphElement::Rectangle:
    {
        qDebug() << "getNewItem: Rectangle";
        auto item = new Rect(ax1,ay1,ax2,ay2,scene);
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



