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
public slots:
};

class TextDocumentView: public DocumentView
{
    Q_OBJECT
public:
    TextDocumentView(QWidget* parent);
    ~TextDocumentView() override {}
    bool openDocument(QFile& file, ReadableWritable rw) override;
    bool saveDocument(QFile& file) override;
    void setReadableWritable(ReadableWritable aRW) override;
    void print(QPrinter* printer) const override;
    bool isTypeDoc(TypeDocumentView type) const override {return type == TypeDocumentView::TextDocumentView ? true: false;}
public:
    QTextEdit& te() {return *textEdit;}
private:
    QTextEdit* textEdit;
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

class GraphDocumentView: public DocumentView
{
    Q_OBJECT

public:
    enum class TypeGraphElement
    {
        Empty,Line,Rectangle,
    };

public:
    GraphDocumentView(QWidget* parent);
    ~GraphDocumentView() override {}
    bool openDocument(QFile& file, ReadableWritable rw) override;
    bool saveDocument(QFile& file) override;
    void setReadableWritable(ReadableWritable aRW) override;
    void print(QPrinter* printer) const override;
    bool isTypeDoc(TypeDocumentView type) const override {return type == TypeDocumentView::GraphDocumentView ? true: false;}

private:
    TypeGraphElement typeGraphElement;
public:
    void setTypeGraphElement(TypeGraphElement type) {typeGraphElement = type;}

private:
    QGraphicsScene *scence;
    QGraphicsView *graphView;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

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

class CreatorTextDocumentView: public CreatorDocumentView
{
public:
    ~CreatorTextDocumentView() override {}
    DocumentView* newDocumentView(QWidget* parent = nullptr) override;
    DocumentView* openDocumentView(QString& fileName, QWidget* parent = nullptr, ReadableWritable rw = ReadableWritable::WRITABLE) override;
    bool saveDocumentView(QString& fileName, DocumentView& docView) override;
protected:
    virtual void fun() override;
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

#endif // DOCUMENTVIEW_H
