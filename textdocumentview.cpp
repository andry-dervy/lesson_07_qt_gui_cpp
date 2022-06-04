#include "textdocumentview.h"
#include <QPixmap>
#include <QIcon>
#include <QTextStream>
#include <QEvent>
#include "QMdiSubWindow"
#include <QResizeEvent>
#include <QDebug>
#include <QColor>

TextDocumentView::TextDocumentView(QWidget* parent)
    :DocumentView(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);
    setWindowIcon(QPixmap(":/icons/new.png"));
    setWindowTitle(tr("Без названия"));
    textEdit->installEventFilter(this);

    connect(textEdit,&QTextEdit::cursorPositionChanged,this,&TextDocumentView::cursorPositionChanged);
}

bool TextDocumentView::openDocument(QFile& file, ReadableWritable aRW)
{
    if(!file.isOpen()) return false;

    QTextStream stream(&file);
    textEdit->setHtml(stream.readAll());
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
    txtStream << textEdit->toHtml();
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

bool TextDocumentView::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::Close:
        {
            auto subWindow = dynamic_cast<QMdiSubWindow*>(obj);
            if(subWindow == nullptr) break;

            emit closeSubWnd(TypeDocumentView::TextDocumentView);

            break;
        }
        default:
            break;
    }
    return QObject::eventFilter(obj, event);
}

void TextDocumentView::cursorPositionChanged()
{
    emit currentAlignment(textEdit->alignment());
}

DocumentView* CreatorTextDocumentView::newDocumentView(QWidget *parent)
{
    return new TextDocumentView(parent);
}

DocumentView* CreatorTextDocumentView::openDocumentView(QString& aFileName, QWidget* parent, ReadableWritable rw)
{
    if(aFileName.isEmpty()) return nullptr;

    QString ext = ".htxt";
    int index = aFileName.indexOf(ext);
    if(index == -1 || aFileName.length() - ext.length() != index) return nullptr;

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
