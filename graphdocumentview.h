#ifndef GRAPHDOCUMENTVIEW_H
#define GRAPHDOCUMENTVIEW_H

#include "documentview.h"
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>


class ControlPoint: public QObject,  public QGraphicsRectItem
{
public:
    ControlPoint(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    ~ControlPoint(){}
protected:
    QRectF boundingRect() const;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event);

private:
};

class Line: public QObject, public QGraphicsItemGroup
{
public:
    Line(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    ~Line(){}
    void setPen(QPen pen);
private:
    qreal x1,y1,x2,y2;
    QGraphicsLineItem* itemLine;
    ControlPoint* p_begin;
    ControlPoint* p_end;

};

class Rect: public QObject, public QGraphicsItemGroup
{
public:
    Rect(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    ~Rect(){}
    void setPen(QPen pen);
private:
    qreal x1,y1,x2,y2;
    QGraphicsRectItem* rect;
protected:
    QRectF boundingRect() const;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

class GraphicsView: public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView() override {}

private:
    bool m_isKeyPressed;
    QPointF lastpos;
    QGraphicsItem* currentItem;
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
    QGraphicsItemGroup *getNewItem(qreal ax1, qreal ay1, qreal ax2, qreal ay2);

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
