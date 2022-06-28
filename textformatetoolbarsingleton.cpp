#include "textformatetoolbarsingleton.h"
#include "toolbarelementsfactory.h"
#include "QDebug"

extern template void ToolbarElementsFactory<QAction>::setText(QObject* obj,const QString&& nameObject,const QString&& text);
extern template std::optional<QAction *> ToolbarElementsFactory<QAction>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);

TextFormateToolBarSingleton* TextFormateToolBarSingleton::getInstance(MainWindow* parent)
{
    static TextFormateToolBarSingleton* theInstance =
            new TextFormateToolBarSingleton(parent);
    return theInstance;
}

void TextFormateToolBarSingleton::retranslate()
{
    ToolbarElementsFactory<QAction>::setText(this,"actTextFormateLeft",tr("Выровнять влево"));
    ToolbarElementsFactory<QAction>::setText(this,"actTextFormateCenter",tr("По центру"));
    ToolbarElementsFactory<QAction>::setText(this,"actTextFormateRight",tr("Выровнять вправо"));
    ToolbarElementsFactory<QAction>::setText(this,"actTextFormateWidth",tr("По ширине"));
    ToolbarElementsFactory<QAction>::setText(this,"actTextFont",tr("Шрифт"));
    ToolbarElementsFactory<QAction>::setText(this,"actColorFont",tr("Цвет шрифта"));
    ToolbarElementsFactory<QAction>::setText(this,"actColorBackground",tr("Цвет фона"));
    ToolbarElementsFactory<QAction>::setText(this,"actClearFormat",tr("Очистка формата"));
    ToolbarElementsFactory<QAction>::setText(this,"actCopyFormat",tr("Копировать формат"));
}

TextFormateToolBarSingleton::TextFormateToolBarSingleton(MainWindow* parent)
    : QToolBar(parent)
    , added(false)
{
    auto act = ToolbarElementsFactory<QAction>::create(
                "actTextFormateLeft", this,true,QPixmap(":/icons/textformate/left.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textFormateLeft()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actTextFormateCenter", this,true,QPixmap(":/icons/textformate/center.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textFormateCenter()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actTextFormateRight", this,true,QPixmap(":/icons/textformate/right.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textFormateRight()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actTextFormateWidth", this,true,QPixmap(":/icons/textformate/width.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textFormateWidth()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actTextFont", this,false,QPixmap(":/icons/textformate/font.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textFont()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actColorFont", this,false,QPixmap(":/icons/textformate/colorfont.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textColorFont()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actColorBackground", this,false,QPixmap(":/icons/textformate/colorbackground.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textColorBackground()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actClearFormat", this,false,QPixmap(":/icons/textformate/clear.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textClearFormat()));
    addAction(*act);

    act = ToolbarElementsFactory<QAction>::create(
                "actCopyFormat", this,true,QPixmap(":/icons/textformate/copyformat.png"));
    Q_ASSERT(act != nullptr);
    connect(*act,SIGNAL(triggered()),this,SLOT(textCopyFormat()));
    addAction(*act);

    retranslate();
}

void TextFormateToolBarSingleton::setActionsChecked(Qt::Alignment alignment)
{
    ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateLeft",false);
    ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateCenter",false);
    ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateRight",false);
    ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateWidth",false);

    qDebug() << "setActionsChecked(" << alignment <<")";

    switch (alignment) {
    case Qt::AlignLeft:
        ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateLeft",true);
        break;
    case Qt::AlignHCenter:
    case Qt::AlignVCenter:
    case Qt::AlignCenter:
        ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateCenter",true);
        break;
    case Qt::AlignRight:
    case Qt::AlignTrailing|Qt::AlignAbsolute:
        ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateRight",true);
        break;
    case Qt::AlignJustify:
        ToolbarElementsFactory<QAction>::setChecked(this,"actTextFormateWidth",true);
        break;
    }
}

void TextFormateToolBarSingleton::setTextFormatAlignment(Qt::Alignment alignment)
{
    auto wnd = qobject_cast<MainWindow*>(parent());
    if(!wnd) return;

    auto optDocView = wnd->currentSubWindow();
    if(!optDocView) return;
    auto textDocView = qobject_cast<TextDocumentView*>(*optDocView);
    if(!textDocView) return;

    textDocView->te().setAlignment(alignment);

    alignment_ = alignment;
    setActionsChecked(alignment);
}

void TextFormateToolBarSingleton::textFormateLeft()
{
    setTextFormatAlignment(Qt::AlignLeft);
}

void TextFormateToolBarSingleton::textFormateCenter()
{
    setTextFormatAlignment(Qt::AlignCenter);
}

void TextFormateToolBarSingleton::textFormateRight()
{
    setTextFormatAlignment(Qt::AlignRight);
}

void TextFormateToolBarSingleton::textFormateWidth()
{
    setTextFormatAlignment(Qt::AlignJustify);
}

void TextFormateToolBarSingleton::textFont()
{
    auto pr = getCursorCurrentSubWindow();
    if(!pr) return;

    auto fmt = pr->second.charFormat();
    QFont font = fmt.font();
    bool b[] = {true};
    font = QFontDialog::getFont(b,font);
    if (b[0])
    {
        QTextCharFormat fmt;
        fmt.setFont(font);
        pr->first->te().setCurrentFont(font);
    }
}

void TextFormateToolBarSingleton::textColorFont()
{
    auto pr = getCursorCurrentSubWindow();
    if(!pr) return;

    auto fmt = pr->second.charFormat();
    auto color = fmt.foreground().color();
    fmt.setForeground(QColorDialog::getColor(color));
    pr->first->te().setCurrentCharFormat(fmt);
}

void TextFormateToolBarSingleton::textColorBackground()
{
    auto pr = getCursorCurrentSubWindow();
    if(!pr) return;

    auto fmt = pr->second.charFormat();
    auto color = fmt.background().color();
    fmt.setBackground(QColorDialog::getColor(color));
    pr->first->te().setCurrentCharFormat(fmt);
}

void TextFormateToolBarSingleton::textClearFormat()
{
    auto pr = getCursorCurrentSubWindow();
    if(!pr) return;

    auto fmt = pr->second.charFormat();
    fmt.clearBackground();
    fmt.clearForeground();
    pr->first->te().setCurrentCharFormat(fmt);
    QFont font;
    font.cleanup();
    pr->first->te().setCurrentFont(font);
    textFormateLeft();
}

void TextFormateToolBarSingleton::textCopyFormat()
{
    static std::tuple<QTextCharFormat,Qt::Alignment> tpl;
    static bool checked = false;

    auto pr = getCursorCurrentSubWindow();
    if(!pr) return;

    auto act = findChild<QAction*>("actCopyFormat");
    if(act == nullptr) return;

    if(checked)
    {
        act->setChecked(false);
        checked = false;

        pr->first->te().textCursor().setCharFormat(std::get<0>(tpl));
        setTextFormatAlignment(std::get<1>(tpl));
    }
    else
    {
        act->setChecked(true);
        checked = true;
        tpl = std::make_tuple<QTextCharFormat,Qt::Alignment>(
                    pr->second.charFormat(),
                    pr->first->te().alignment());
    }
}

void TextFormateToolBarSingleton::activatedDocumentView(DocumentView* docView)
{
    if(!docView) return;
    auto textDocView = qobject_cast<TextDocumentView*>(docView);
    if(!textDocView) return;

    setActionsChecked(textDocView->te().alignment());
}

void TextFormateToolBarSingleton::currentAlignment(Qt::Alignment alignment)
{
    setActionsChecked(alignment);
}

std::optional<std::pair<TextDocumentView*,QTextCursor>> TextFormateToolBarSingleton::getCursorCurrentSubWindow() const
{
    auto wnd = qobject_cast<MainWindow*>(parent());
    if(!wnd) return std::nullopt;

    auto optDocView = wnd->currentSubWindow();
    if(!optDocView) return std::nullopt;
    auto textDocView = qobject_cast<TextDocumentView*>(*optDocView);
    if(!textDocView) return std::nullopt;

    return std::make_optional<std::pair<TextDocumentView*,QTextCursor>>(textDocView, textDocView->te().textCursor());
}

void TextFormateToolBarSingleton::addTextDocumentView(DocumentView *docView)
{
    if(!docView) return;
    auto txtDocView = qobject_cast<TextDocumentView*>(docView);
    if(!txtDocView) return;
    if(docViews.find(txtDocView) != docViews.end()) return;
    docViews.insert(txtDocView);
    connect(txtDocView,&TextDocumentView::currentAlignment,this,&TextFormateToolBarSingleton::currentAlignment);
    connect(txtDocView,&TextDocumentView::close,this,&TextFormateToolBarSingleton::eraseTextDocumentView);
}

void TextFormateToolBarSingleton::eraseTextDocumentView(TextDocumentView *txtDocView)
{
    if(docViews.find(txtDocView) == docViews.end()) return;
    docViews.erase(docViews.find(txtDocView));
}
