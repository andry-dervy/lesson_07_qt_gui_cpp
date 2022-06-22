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
                case GraphDocumentView::TypeGraphElement::Rectangle:
                {
                    auto newitem = docView->getNewItem(pos.x(), pos.y(), pos.x() + SIZE, pos.y() + SIZE);
                    newitem->setPos(pos);
                    if(newitem)
                    {
                        //scene()->addItem(newitem);
                        currentItem = newitem;
                        lastpos = pos;
                    }
                    break;
                }
                case GraphDocumentView::TypeGraphElement::Star:
                {
                    break;
                }
                case GraphDocumentView::TypeGraphElement::Elipse:
                {
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

QGraphicsItem* GraphDocumentView::getNewItem(qreal ax1, qreal ay1, qreal ax2, qreal ay2)
{
    switch(typeGraphElement)
    {
    case TypeGraphElement::Empty:
        qDebug() << "getNewItem: Empty";
        return nullptr;
    case TypeGraphElement::Rectangle:
    {
        qDebug() << "getNewItem: Rectangle";
//        auto item = new Rect(ax1,ay1,ax2,ay2,scene);
//        item->setPen(currentPen);
//        return item;
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




