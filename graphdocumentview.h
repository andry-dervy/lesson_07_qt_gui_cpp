#ifndef GRAPHDOCUMENTVIEW_H
#define GRAPHDOCUMENTVIEW_H

#include "documentview.h"
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class BaseGraphicsItem: public QGraphicsItem
{
public:
    BaseGraphicsItem(QGraphicsScene* scene, QGraphicsItem *parent = nullptr);

    virtual ~BaseGraphicsItem() {}
    virtual void resize(qreal x2, qreal y2) = 0;
};

class ControlPoint: public QObject,  public BaseGraphicsItem
{
    Q_OBJECT
public:
    ControlPoint(QGraphicsScene* scene, QGraphicsItem *parent = nullptr);
    ~ControlPoint() override {}
    void resize(qreal x2, qreal y2) override {
        Q_UNUSED(x2);Q_UNUSED(y2);
    }
protected:
    QRectF boundingRect() const override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event) override;

private:
    const int SZ = 30;
};

class Line: public QObject, public BaseGraphicsItem
{
    Q_OBJECT
public:
    Line(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsScene* scene, QGraphicsItem *parent = nullptr);
    ~Line() override {}
    void setPen(QPen pen);
    void resize(qreal x2, qreal y2) override {
        Q_UNUSED(x2);Q_UNUSED(y2);
    }
private:
    qreal x1,y1,x2,y2;
    QGraphicsLineItem* itemLine;
    ControlPoint* p_begin;
    ControlPoint* p_end;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event) override;

};

class Rect: public QObject, public BaseGraphicsItem
{
    Q_OBJECT
public:
    Rect(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsScene* scene, QGraphicsItem *parent = nullptr);
    ~Rect(){}
    void setPen(QPen aPen);
    //void resize(int weight, int hight) override;
    void resize(qreal x2, qreal y2) override;
private:
    qreal x1,y1,x2,y2;
    bool hoverEntered;

    const int SZ = 100;
    qreal w;
    qreal h;


    QGraphicsRectItem* rect;
    QPen pen;

    ControlPoint* ctrlPoint;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
};


class GraphicsView: public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView() override {}

private:
    bool m_isKeyPressed;
    QPointF lastpos;
    BaseGraphicsItem* currentItem;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

class GraphDocumentView: public DocumentView
{
    Q_OBJECT

public:
    enum class TypeGraphElement
    {
        Empty,Line,Rectangle,
    };

    Q_ENUMS(TypeGraphElement)

public:
    GraphDocumentView(QWidget* parent);
    ~GraphDocumentView() override {}
    bool openDocument(QFile& file, ReadableWritable rw) override;
    bool saveDocument(QFile& file) override;
    void setReadableWritable(ReadableWritable aRW) override;
    void print(QPrinter* printer) const override;
    bool isTypeDoc(TypeDocumentView type) const override {return type == TypeDocumentView::GraphDocumentView ? true: false;}
    TypeGraphElement getTypeGraphElement() const {return typeGraphElement;}

private:
    TypeGraphElement typeGraphElement;
    bool isKeyPressed;

public:
    BaseGraphicsItem *getNewItem(qreal ax1, qreal ay1, qreal ax2, qreal ay2);

public:
    void setTypeGraphElement(TypeGraphElement type) {typeGraphElement = type;}

private:
    QGraphicsScene *scene;
    GraphicsView *graphView;
    QPen currentPen;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

class CreatorGraphDocumentView: public CreatorDocumentView
{
public:
    ~CreatorGraphDocumentView() override {}
    DocumentView* newDocumentView(QWidget* parent = nullptr) override;
    DocumentView* openDocumentView(QString& fileName, QWidget* parent = nullptr, ReadableWritable rw = ReadableWritable::WRITABLE) override;
    bool saveDocumentView(QString& fileName, DocumentView& docView) override;
protected:
    virtual void fun() override;
};

#endif // GRAPHDOCUMENTVIEW_H
