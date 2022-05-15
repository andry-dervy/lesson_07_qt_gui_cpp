#include "documentview.h"
#include <QPixmap>
#include <QIcon>
#include <QTextStream>

DocumentView::DocumentView(QWidget* parent)
    :QWidget(parent)
    ,rw(ReadableWritable::WRITABLE)
{}

TextDocumentView::TextDocumentView(QWidget* parent)
    :DocumentView(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);
    setWindowIcon(QPixmap(":/icons/new.png"));
    setWindowTitle(tr("Без названия"));
}

bool TextDocumentView::openDocument(QFile& file, ReadableWritable aRW)
{
    if(!file.isOpen()) return false;

    QTextStream stream(&file);
    textEdit->setPlainText(stream.readAll());
    rw = aRW;
    switch (rw) {
    case ReadableWritable::READABLE:
    case ReadableWritable::READABLE_ONLY:
        textEdit->setReadOnly(true);
        break;
    case ReadableWritable::WRITABLE:
        textEdit->setReadOnly(false);
        break;
    }
    fileName = file.fileName();

    setWindowTitle(fileName.section('/',-1));

    return true;
}

bool TextDocumentView::saveDocument(QFile& file)
{
    if(!file.isOpen()) return false;

    QTextStream txtStream(&file);
    txtStream << textEdit->toPlainText();
    fileName = file.fileName();
    setWindowTitle(fileName.section('/',-1));
    return true;
}

void TextDocumentView::setReadableWritable(ReadableWritable aRW)
{
    rw = aRW;
    switch (rw) {
    case ReadableWritable::READABLE:
    case ReadableWritable::READABLE_ONLY:
        textEdit->setReadOnly(true);
        break;
    case ReadableWritable::WRITABLE:
        textEdit->setReadOnly(false);
        break;
    }
}

void TextDocumentView::print(QPrinter* printer) const
{
    textEdit->print(printer);
}

DocumentView* CreatorTextDocumentView::newDocumentView(QWidget *parent)
{
    return new TextDocumentView(parent);
}

DocumentView* CreatorTextDocumentView::openDocumentView(QString& aFileName, QWidget* parent, ReadableWritable rw)
{
    if(aFileName.isEmpty()) return nullptr;

    int index = aFileName.indexOf(".txt");
    if(index == -1 || aFileName.length() - 4 != index) return nullptr;

    QFile file(aFileName);
    if(!file.open(QFile::ReadOnly | QFile::ExistingOnly)) return nullptr;

    auto txtDocView = new TextDocumentView(nullptr);
    if(txtDocView == nullptr) return nullptr;

    bool opened = txtDocView->openDocument(file,rw);

    file.close();

    if(!opened)
    {
        delete txtDocView;
        return nullptr;
    }

    txtDocView->setParent(parent);
    return txtDocView;
}

bool CreatorTextDocumentView::saveDocumentView(QString& aFileName, DocumentView& docView)
{
    if(aFileName.isEmpty()) return false;

    QFile file(aFileName);
    if(!file.open(QFile::WriteOnly | QFile::Truncate)) return false;

    docView.saveDocument(file);
    file.close();

    return true;
}

void CreatorTextDocumentView::fun() {}

void CreatorDocumentView::fun(){}
