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

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView (parent)
    , currentItem(nullptr)
    , m_rotate(false)
{}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    switch (event->buttons()) {
        case Qt::LeftButton:
        {
            if(!currentItem) break;

            QPointF pos = mapToScene(event->pos());

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
            auto newitem = docView->getNewItem();
            if(newitem)
            {
                QPointF pos = mapToScene(event->pos());
                newitem->setPos(pos);
                scene()->addItem(newitem);
            }
            break;
        }
        case Qt::RightButton:
        {
            auto item = itemAt(event->pos());
            if(item) scene()->removeItem(item);
            break;
        }
        case Qt::MidButton:
        {
            m_rotate = true;
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
    case Qt::MidButton:
    {
        m_rotate = false;
        break;
    }
    default:
    break;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers() == Qt::KeyboardModifier::ControlModifier)
    {
        if(event->key() == Qt::Key_Equal)
        {
            scale(zoom_in,zoom_in);
            qDebug() << "Zoom" << zoom_in;
        }
        else if(event->key() == Qt::Key_Minus)
        {
            scale(zoom_out,zoom_out);
            qDebug() << "Zoom" << zoom_out;
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if(m_rotate)
    {
        auto pos = mapToScene(event->pos());
        QTransform transform;
        auto item = scene()->itemAt(pos,transform);
        if(item)
        {
            QPoint numDegrees = event->angleDelta();
            auto center = item->boundingRect().center();

            auto angle = item->data(GraphDocumentView::TypeData::Angle).toReal();

            if(numDegrees.y() > 0)
            {
                angle += 10;
            }
            else
            {
                angle -= 10;
            }
            transform.translate(center.x(), center.y()).rotate(angle).translate(-center.x(), -center.y());
            item->setTransform(transform);

            item->setData(GraphDocumentView::TypeData::Angle,angle);
        }
    }
    QGraphicsView::wheelEvent(event);
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

    scene = new QGraphicsScene(this);
    scene->setSceneRect(QRect(0,0,SIZE_SCENE,SIZE_SCENE));
    graphView->setScene(scene);
    graphView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

    graphView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    graphView->show();

    setWindowIcon(QPixmap(":/icons/new.png"));
    setWindowTitle(tr("Без названия"));

    currentPen.setColor(Qt::red);
    colorBrush = Qt::red;
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

QGraphicsItem* GraphDocumentView::getNewItem()
{
    switch(typeGraphElement)
    {
    case TypeGraphElement::Empty:
        qDebug() << "getNewItem: Empty";
        return nullptr;
    case TypeGraphElement::Rectangle:
    {
        qDebug() << "getNewItem: Rectangle";
        auto item = new QGraphicsRectItem(0,0,SIZE,SIZE);
        item->setPen(currentPen);
        item->setBrush(colorBrush);
        item->setFlags(QGraphicsItem::ItemIsMovable);
        int angle = 0;
        item->setData(TypeData::Angle,angle);
        return item;
    }
    case TypeGraphElement::Ellipse:
    {
        qDebug() << "getNewItem: Ellipse";
        auto item = new QGraphicsEllipseItem(0,0,SIZE,SIZE);
        item->setPen(currentPen);
        item->setBrush(colorBrush);
        item->setFlags(QGraphicsItem::ItemIsMovable);
        int angle = 0;
        item->setData(TypeData::Angle,angle);
        return item;
    }
    case TypeGraphElement::Star:
    {
        qDebug() << "getNewItem: Star";

        QPolygonF polygon;
        polygon << QPointF(0.5*SIZE, 0)
                << QPointF(0.6*SIZE, 0.4*SIZE)
                << QPointF(SIZE, 0.4*SIZE)
                << QPointF(0.7*SIZE, 0.6*SIZE)
                << QPointF(0.9*SIZE, 1.0*SIZE)
                << QPointF(0.5*SIZE, 0.7*SIZE)
                << QPointF(0.1*SIZE, 1.0*SIZE)
                << QPointF(0.3*SIZE, 0.6*SIZE)
                << QPointF(0.0*SIZE, 0.4*SIZE)
                << QPointF(0.4*SIZE, 0.4*SIZE);

        auto item = new QGraphicsPolygonItem(polygon);
        item->setPen(currentPen);
        item->setBrush(colorBrush);
        item->setFlags(QGraphicsItem::ItemIsMovable);
        int angle = 0;
        item->setData(TypeData::Angle,angle);
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




