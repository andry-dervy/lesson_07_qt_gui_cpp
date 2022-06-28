#ifndef GRAPHDOCUMENTVIEW_H
#define GRAPHDOCUMENTVIEW_H

#include "documentview.h"
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class GraphicsView: public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView() override {}

private:
    QPointF lastpos;
    QGraphicsItem* currentItem;
    bool m_rotate;

    const double zoom_in = 1.1;
    const double zoom_out = 0.9;

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

class GraphDocumentView: public DocumentView
{
    Q_OBJECT

public:
    enum class TypeGraphElement
    {
        Empty,Ellipse,Rectangle,Star,
    };
    Q_ENUM(TypeGraphElement)
    enum TypeData
    {
        Angle,
    };
    Q_ENUM(TypeData)

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
    const int SIZE = 250;
    const int SIZE_SCENE = 100;

public:
    QGraphicsItem* getNewItem();

public:
    void setTypeGraphElement(TypeGraphElement type) {typeGraphElement = type;}
    void setPenWidth(int aWidth) {currentPen.setWidth(aWidth);}
    void setPenColor(QColor aColor) {currentPen.setColor(aColor);}
    void setColorBrush(QColor aColor) {colorBrush = aColor;}

private:
    QGraphicsScene *scene;
    GraphicsView *graphView;
    QPen currentPen;
    QColor colorBrush;

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
