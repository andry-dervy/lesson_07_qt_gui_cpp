#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QTextEdit>
#include "QPrinter"

enum ReadableWritable
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
protected:
    ReadableWritable rw;
    QString fileName;
public:
    const QString& getFileName() const {return fileName;}
    virtual void setReadableWritable(ReadableWritable aRW) {rw = aRW;}
    ReadableWritable getReadableWritable() const {return rw;}

signals:

public slots:
};

class TextDocumentView: public DocumentView
{
public:
    TextDocumentView(QWidget* parent);
    ~TextDocumentView() override {}
    bool openDocument(QFile& file, ReadableWritable rw) override;
    bool saveDocument(QFile& file) override;
    void setReadableWritable(ReadableWritable aRW) override;
    void print(QPrinter* printer) const override;
private:
    QTextEdit* textEdit;
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

#endif // DOCUMENTVIEW_H
