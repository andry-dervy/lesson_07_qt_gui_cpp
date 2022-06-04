#ifndef TEXTDOCUMENTVIEW_H
#define TEXTDOCUMENTVIEW_H

#include "documentview.h"

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
signals:
    void currentAlignment(Qt::Alignment alignment);
    void close(TextDocumentView* txtDocView);

public slots:
    void cursorPositionChanged();
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

#endif // TEXTDOCUMENTVIEW_H
