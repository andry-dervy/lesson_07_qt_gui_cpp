#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QTextEdit>
#include "QPrinter"
#include <QGraphicsView>
#include <QGraphicsScene>

enum class TypeDocumentView
{
    TextDocumentView,
    GraphDocumentView,
};

enum class ReadableWritable
{
    READABLE,
    READABLE_ONLY,
    WRITABLE,
};

class DocumentView : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentView(QWidget *parent = nullptr);
    virtual ~DocumentView() = default;
    virtual bool openDocument(QFile& file, ReadableWritable rw) = 0;
    virtual bool saveDocument(QFile& file) = 0;
    virtual void print(QPrinter* printer) const = 0;
    virtual bool isTypeDoc(TypeDocumentView type) const = 0;
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) = 0;

protected:
    ReadableWritable rw;
    QString fileName;
public:
    const QString& getFileName() const {return fileName;}
    virtual void setReadableWritable(ReadableWritable aRW) {rw = aRW;}
    ReadableWritable getReadableWritable() const {return rw;}

signals:
    void closeSubWnd(TypeDocumentView type);
};

class CreatorDocumentView
{
public:
    virtual ~CreatorDocumentView() = default;
    virtual DocumentView* newDocumentView(QWidget* parent) = 0;
    virtual DocumentView* openDocumentView(QString& fileName, QWidget* parent, ReadableWritable rw = ReadableWritable::WRITABLE) = 0;
    virtual bool saveDocumentView(QString& fileName, DocumentView& docView) = 0;
protected:
    virtual void fun();
};

#endif // DOCUMENTVIEW_H
