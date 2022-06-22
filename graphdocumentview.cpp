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

BaseGraphicsItem::BaseGraphicsItem(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{}

ControlPoint::ControlPoint(QGraphicsItem *parent)
    : BaseGraphicsItem(parent)
    , pen(Qt::cyan)
{}

void ControlPoint::removeFromScene()
{
    auto base = dynamic_cast<BaseGraphicsItem*>(parent());
    if(base) base->removeFromScene();
}

QRectF ControlPoint::boundingRect() const
{
    return QRectF(-SZ, -SZ, SZ, SZ);
}

void ControlPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen);
    painter->fillRect(boundingRect(),pen.color());
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QPointF posEvent = mapToScene(event->pos());
        emit move(posEvent.x() - posOld.x(),posEvent.y() - posOld.y());
        posOld = posEvent;
        QPointF posPoint = mapToScene(pos());
        qDebug() << "ControlPoint::mouseMoveEvent: " << posPoint.x() << posPoint.y() << posEvent.x() << posEvent.y();
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        posOld = mapToScene(event->pos());
        qDebug() << "ControlPoint::mousePressEvent: " << posOld.x() << posOld.y();
    }

    QGraphicsItem::mousePressEvent(event);
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

void Line::removeFromScene()
{

}

QRectF Line::boundingRect() const
{
    return QRectF();
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

//GraphicsRectItem::GraphicsRectItem(QGraphicsItem *parent)
//    : BaseGraphicsItem(parent)
//{

//}



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

//    rect = new QGraphicsRectItem(this);
//    rect->setRect(x1,y1,w,h);
//    rect->setPen(QPen(Qt::blue));

    cpUpLeft = new ControlPoint(this);//cpUpLeft
    connect(cpUpLeft,&ControlPoint::move,this,&Rect::moveUpLeftCorner);
    cpUpLeft->setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);

    cpDownLeft = new ControlPoint(this);//cpDownLeft
    cpUpRight = new ControlPoint(this);//cpUpRight
    cpDownRight = new ControlPoint(this);//cpDownRight

    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

void Rect::setPen(QPen aPen)
{
    pen = aPen;
}

void Rect::resize(qreal ax2, qreal ay2)
{
    qreal weight = ax2 - x1;
    qreal hight = ay2 - y1;

    if(weight >= 0 && hight >= 0)
    {
        x2 = ax2;
        y2 = ay2;

        prepareGeometryChange();
        w = weight;
        h = hight;

        setPos(x1 + weight, y1 + hight);

        //rect->setRect(-w,-h,w,h);
        cpUpLeft->setPos(-w+cpUpLeft->SZ,-h+cpUpLeft->SZ);
        cpUpLeft->setPosOld(QPointF(-w+cpUpLeft->SZ,-h+cpUpLeft->SZ));
        cpDownLeft->setPos(-w+cpDownLeft->SZ,0);
        cpDownLeft->setPosOld(QPointF(-w+cpDownLeft->SZ,0));
        cpUpRight->setPos(0,-h+cpUpRight->SZ);
        cpUpRight->setPosOld(QPointF(0,-h+cpUpRight->SZ));
        cpDownRight->setPos(0,0);
        cpDownRight->setPosOld(QPointF(0,0));

        update();
        qDebug() << "x1 " << x1 << " x2 " << x2 << " y1 " << y1 << " y2 " << y2;
        qDebug() << "Rect::resize: " << w << " " << h;
    }
}

void Rect::removeFromScene()
{

}

QRectF Rect::boundingRect() const
{
    return QRectF(-w, -h, w, h);
}

void Rect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(pen);
    painter->drawRect(boundingRect());
}

void Rect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    //qDebug() << "Rect::mouseMoveEvent: " << event->pos();
    //update();
    QPointF posEvent = mapToScene(event->pos());
    x1 = posEvent.x() - w/2;
    y1 = posEvent.y() - h/2;
    x2 = posEvent.x() + w/2;
    y2 = posEvent.y() + h/2;

//    cpUpLeft->setPosOld(QPointF(-w+cpUpLeft->SZ,-h+cpUpLeft->SZ));
//    cpDownLeft->setPosOld(QPointF(-w+cpDownLeft->SZ,0));
//    cpUpRight->setPosOld(QPointF(0,-h+cpUpRight->SZ));
//    cpDownRight->setPosOld(QPointF(0,0));

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

    cpUpLeft->setVisible(true);
    cpDownLeft->setVisible(true);
    cpUpRight->setVisible(true);
    cpDownRight->setVisible(true);

    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Rect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "Rect::hoverLeaveEvent: " << event->pos();
    hoverEntered = false;

    cpUpLeft->setVisible(false);
    cpDownLeft->setVisible(false);
    cpUpRight->setVisible(false);
    cpDownRight->setVisible(false);

    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Rect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //qDebug() << "Rect::hoverMoveEvent: " << event->pos();

    QGraphicsItem::hoverMoveEvent(event);
}

void Rect::moveUpLeftCorner(qreal dx, qreal dy)
{
    x1 += dx;
    y1 += dy;

    qreal weight = x2 - x1;
    qreal hight = y2 - y1;

    if(weight >= 0 && hight >= 0)
    {
        prepareGeometryChange();
        w = weight;
        h = hight;

        setPos(x1 + weight, y1 + hight);

        //rect->setRect(-w,-h,w,h);
        //cpUpLeft->setPos(-w+cpUpLeft->SZ,-h+cpUpLeft->SZ);
        cpDownLeft->setPos(-w+cpDownLeft->SZ,0);
        cpUpRight->setPos(0,-h+cpUpRight->SZ);
        cpDownRight->setPos(0,0);

        update();
        qDebug() << "x1 " << x1 << " x2 " << x2 << " y1 " << y1 << " y2 " << y2;
        qDebug() << "Rect::resize: " << w << " " << h;
    }
}

void Rect::moveUpRightCorner(qreal dx, qreal dy)
{

}

void Rect::moveDownLeftCorner(qreal dx, qreal dy)
{

}

void Rect::moveDownRightCorner(qreal dx, qreal dy)
{

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
        case Qt::RightButton:
        {
            auto item = itemAt(event->pos());
            if(item) scene()->removeItem(item);
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




