#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QTextStream"
#include "QDockWidget"
#include "QMdiSubWindow"
#include "languagevisitor.h"
#include "QPrinter"
#include "QToolButton"
#include "textformatetoolbarsingleton.h"
#include "drawgraphtoolbarsingleton.h"
#include "toolbarelementsfactory.h"

extern template void ToolbarElementsFactory<QAction>::setText(QObject* obj,const QString&& nameObject,const QString&& text);
extern template void ToolbarElementsFactory<QMenu>::setText(QObject* obj,const QString&& nameObject,const QString&& text);
extern template std::optional<QAction *> ToolbarElementsFactory<QAction>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);
extern template std::optional<QMenu *> ToolbarElementsFactory<QMenu>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);
extern template std::optional<QToolButton *> ToolbarElementsFactory<QToolButton>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon);

void MainWindow::setMenuFile()
{
    auto menuFile = ToolbarElementsFactory<QMenu>::create("menuFile",this);
    Q_ASSERT(menuFile != nullptr);
    ui->menubar->addMenu(*menuFile);
    auto menuNewFile = ToolbarElementsFactory<QMenu>::create("menuNewFile",this);
    Q_ASSERT(menuFile != nullptr);
    (*menuFile)->addMenu(*menuNewFile);

    auto act = ToolbarElementsFactory<QAction>::create("actNewTextFile",this);
    Q_ASSERT(act != nullptr);
    (*menuNewFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::newTextFile);
    act = ToolbarElementsFactory<QAction>::create("actNewGraphFile",this);
    Q_ASSERT(act != nullptr);
    (*menuNewFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::newGraphFile);

    act = ToolbarElementsFactory<QAction>::create("actOpen",this);
    Q_ASSERT(act != nullptr);
    (*menuFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::openFile);
    act = ToolbarElementsFactory<QAction>::create("actOpenReadableOnly",this);
    Q_ASSERT(act != nullptr);
    (*menuFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::openFileReadableOnly);

    act = ToolbarElementsFactory<QAction>::create("actSave",this);
    Q_ASSERT(act != nullptr);
    (*menuFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::saveFile);
    act = ToolbarElementsFactory<QAction>::create("actSaveAs",this);
    Q_ASSERT(act != nullptr);
    (*menuFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::saveFileAs);

    act = ToolbarElementsFactory<QAction>::create("actPrint",this);
    Q_ASSERT(act != nullptr);
    (*menuFile)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::printFile);
}

void MainWindow::setMenuView()
{
    auto menuView = ToolbarElementsFactory<QMenu>::create("menuView",this);
    Q_ASSERT(menuView != nullptr);
    ui->menubar->addMenu(*menuView);
    auto act = ToolbarElementsFactory<QAction>::create("actViewTreeDirs",this,true);
    Q_ASSERT(act != nullptr);
    (*act)->setChecked(false);
    (*menuView)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::viewTreeDirs);
    act = ToolbarElementsFactory<QAction>::create("actViewToolBarTextFormat",this,true);
    Q_ASSERT(act != nullptr);
    (*act)->setChecked(false);
    (*menuView)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::viewToolBarTextFormat);
    act = ToolbarElementsFactory<QAction>::create("actViewToolBarDrawing",this,true);
    Q_ASSERT(act != nullptr);
    (*act)->setChecked(false);
    (*menuView)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::viewToolBarDrawing);
}

void MainWindow::setMenuSettings()
{
    auto menuSettings = ToolbarElementsFactory<QMenu>::create("menuSettings",this);
    Q_ASSERT(menuSettings != nullptr);
    ui->menubar->addMenu(*menuSettings);
    auto act = ToolbarElementsFactory<QAction>::create("actAssignKeyboardShortcuts",this);
    Q_ASSERT(act != nullptr);
    (*menuSettings)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::assignKeyboardShortcuts);

    auto menuTheme = ToolbarElementsFactory<QMenu>::create("menuTheme",this);
    Q_ASSERT(menuTheme != nullptr);
    (*menuSettings)->addMenu(*menuTheme);
    act = ToolbarElementsFactory<QAction>::create("actThemeMailSy",this);
    Q_ASSERT(act != nullptr);
    (*menuTheme)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::setThemeMailSy);
    act = ToolbarElementsFactory<QAction>::create("actThemeIntegrid",this);
    Q_ASSERT(act != nullptr);
    (*menuTheme)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::setThemeIntegrid);
}

void MainWindow::setMenuLanguage()
{
    auto menuLanguage = ToolbarElementsFactory<QMenu>::create("menuLanguage",this);
    Q_ASSERT(menuLanguage != nullptr);
    ui->menubar->addMenu(*menuLanguage);
    auto act = ToolbarElementsFactory<QAction>::create("actRussian",this,true);
    Q_ASSERT(act != nullptr);
    (*menuLanguage)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::setRussianLanguage);
    act = ToolbarElementsFactory<QAction>::create("actEnglish",this,true);
    Q_ASSERT(act != nullptr);
    (*menuLanguage)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::setEnglishLanguage);
}

void MainWindow::setMenuHelp()
{
    auto menuHelp = ToolbarElementsFactory<QMenu>::create("menuHelp",this);
    Q_ASSERT(menuHelp != nullptr);
    ui->menubar->addMenu(*menuHelp);
    auto act = ToolbarElementsFactory<QAction>::create("actCallingHelp",this);
    Q_ASSERT(act != nullptr);
    (*menuHelp)->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::callingHelp);
}

void MainWindow::setMenu()
{
    setMenuFile();
    setMenuView();
    setMenuSettings();
    setMenuLanguage();
    setMenuHelp();

    setThemeIntegrid();
}

void MainWindow::setToolBar()
{
    auto toptb = new QToolBar(this);
    Q_ASSERT(toptb != nullptr);
    addToolBar(Qt::TopToolBarArea,toptb);

    auto tb_newFile = ToolbarElementsFactory<QToolButton>::create("menuHelp",this,false,QPixmap(":/icons/new.png"));
    Q_ASSERT(tb_newFile != nullptr);
    (*tb_newFile)->setPopupMode(QToolButton::MenuButtonPopup);
    toptb->addWidget(*tb_newFile);

    auto menuNewFile = findChild<QMenu*>("menuNewFile");
    Q_ASSERT(menuNewFile != nullptr);
    (*tb_newFile)->setMenu(menuNewFile);

    auto act = ToolbarElementsFactory<QAction>::create("toolBarOpenFile",this,false,QPixmap(":/icons/open.png"));
    Q_ASSERT(act != nullptr);
    toptb->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::openFile);
    act = ToolbarElementsFactory<QAction>::create("toolBarSaveFile",this,false,QPixmap(":/icons/save.png"));
    Q_ASSERT(act != nullptr);
    toptb->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::saveFile);
    act = ToolbarElementsFactory<QAction>::create("toolBarPrintFile",this,false,QPixmap(":/icons/print.png"));
    Q_ASSERT(act != nullptr);
    toptb->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::printFile);

    toptb->addSeparator();

    act = ToolbarElementsFactory<QAction>::create("changeReadableWritable",this,false,QPixmap(":/icons/print.png"));
    Q_ASSERT(act != nullptr);
    changeReadableWritable = *act;
    toptb->addAction(*act);
    connect(*act, &QAction::triggered, this, &MainWindow::changeReadableWritable_triggered);
    setReadableWritable(ReadableWritable::WRITABLE);

    addToolBar(Qt::TopToolBarArea,TextFormateToolBarSingleton::getInstance(this));
    TextFormateToolBarSingleton::getInstance(this)->setAdded();
    TextFormateToolBarSingleton::getInstance(this)->setVisible(false);
    connect(this, &MainWindow::activatedDocumentView,
            TextFormateToolBarSingleton::getInstance(this), &TextFormateToolBarSingleton::activatedDocumentView);

    addToolBar(Qt::TopToolBarArea,DrawGraphToolBarSingleton::getInstance(this));
    DrawGraphToolBarSingleton::getInstance(this)->setAdded();
    DrawGraphToolBarSingleton::getInstance(this)->setVisible(false);
    connect(this, &MainWindow::activatedDocumentView,
            DrawGraphToolBarSingleton::getInstance(this), &DrawGraphToolBarSingleton::activatedDocumentView);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pathDir(QDir::currentPath())
    , wdtHelp(nullptr)
    , keyPressEvent(new KeyPressEventFilter(this))
    , wdgtSettings(nullptr)
    , dockTreeDirs(nullptr)
    , wdgtTreeDirs(nullptr)
    , tbTextFormate(nullptr)
{
    ui->setupUi(this);

    mdi = new QMdiArea(this);
    mdi->setViewMode(QMdiArea::ViewMode::TabbedView);
    connect(mdi, &QMdiArea::subWindowActivated, this, &MainWindow::subWindowActivated);
    setCentralWidget(mdi);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    installEventFilter(keyPressEvent);
    connect(keyPressEvent, &KeyPressEventFilter::newFile, this, &MainWindow::newTextFile);
    connect(keyPressEvent, &KeyPressEventFilter::openFile, this, &MainWindow::openFile);
    connect(keyPressEvent, &KeyPressEventFilter::saveFile, this, &MainWindow::saveFile);
    connect(keyPressEvent, &KeyPressEventFilter::quit, this, &MainWindow::quit);

    setMenu();
    setToolBar();

    setRussianLanguage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addSubWindow(DocumentView* docView)
{
    if(docView == nullptr) return;

    auto subwnd = mdi->addSubWindow(docView);
    subwnd->installEventFilter(docView);
    connect(docView,&DocumentView::closeSubWnd,this,&MainWindow::closeSubWnd);
    TextFormateToolBarSingleton::getInstance(this)->addTextDocumentView(docView);
    docView->show();
}

void MainWindow::newTextFile()
{
    CreatorTextDocumentView creatorTextDocView;
    DocumentView* docView = creatorTextDocView.newDocumentView(mdi);
    addSubWindow(docView);
}

void MainWindow::newGraphFile()
{
    CreatorGraphDocumentView creatorGraphDocView;
    auto docView = creatorGraphDocView.newDocumentView(mdi);
    addSubWindow(docView);
}

void MainWindow::openFilePath(QString& fileName, ReadableWritable aRW = ReadableWritable::WRITABLE)
{
    auto wnds = mdi->subWindowList();
    for(const auto& wnd: wnds)
    {
        auto docView = qobject_cast<DocumentView*>(wnd->widget());
        if(docView == nullptr) continue;
        if(docView->getFileName() == fileName)
        {
            wnd->setFocus();
            return;
        }
    }

    CreatorTextDocumentView creatorTextDocView;
    auto docView = creatorTextDocView.openDocumentView(fileName,mdi,aRW);
    addSubWindow(docView);
}

void MainWindow::openFile()
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.htxt)"));

    openFilePath(fileName);
    pathDir = fileDialog.directory().absolutePath();
}

void MainWindow::openFileReadableOnly()
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.htxt)"));

    openFilePath(fileName,ReadableWritable::READABLE_ONLY);
    pathDir = fileDialog.directory().absolutePath();
}

void MainWindow::openFilePathFromFileSystemModel(QString& fileName)
{
    openFilePath(fileName, ReadableWritable::WRITABLE);
}

void MainWindow::saveFile()
{
    auto optDocView = currentSubWindow();
    if(!optDocView) return;
    auto docView = *optDocView;

    QString fileName = docView->getFileName();
    if(fileName.isEmpty())
    {
        QFileDialog fileDialog;

        fileName = fileDialog.getSaveFileName(this,
            tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.htxt)"));

        if(fileName.isEmpty()) return;

        const QString ext = ".htxt";
        int index = fileName.indexOf(ext);
        if(index == -1 || fileName.length() - ext.length() != index)
        {
            fileName += ext;
        }
    }

    CreatorTextDocumentView creatorTextDocView;
    creatorTextDocView.saveDocumentView(fileName,*docView);
}

void MainWindow::saveFileAs()
{
    auto optDocView = currentSubWindow();
    if(!optDocView) return;
    auto docView = *optDocView;

    QFileDialog fileDialog;

    QString fileName = fileDialog.getSaveFileName(this,
        tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.htxt)"));

    if(fileName.isEmpty()) return;

    const QString ext = ".htxt";
    int index = fileName.indexOf(ext);
    if(index == -1 || fileName.length() - ext.length() != index)
    {
        fileName += ".txt";
    }

    CreatorTextDocumentView creatorTextDocView;
    creatorTextDocView.saveDocumentView(fileName,*docView);
}

void MainWindow::printFile()
{
    auto optDocView = currentSubWindow();
    if(!optDocView) return;
    auto docView = *optDocView;

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle("Print");
    if (dlg.exec() != QDialog::Accepted)
        return;
    docView->print(&printer);
}

void MainWindow::callingHelp()
{
    if(wdtHelp != nullptr)
    {
        wdtHelp->show();
        return;
    }

    QFile file(":/help/help.txt");
    if(!file.open(QIODevice::OpenModeFlag::ReadOnly)) return;

    wdtHelp = new QDialog(this);
    QTextStream textStream(&file);
    auto textHelp = new QPlainTextEdit(textStream.readAll(),this);
    textHelp->setReadOnly(true);
    auto vBoxLayout = new QVBoxLayout(wdtHelp);
    vBoxLayout->addWidget(textHelp);
    wdtHelp->setLayout(vBoxLayout);
    wdtHelp->setMinimumSize(QSize(200,200));
    retranslate();
    wdtHelp->show();
}

void MainWindow::setReadableWritable(ReadableWritable aRW)
{
    rw = aRW;
    switch (rw) {
    case ReadableWritable::READABLE:
    case ReadableWritable::READABLE_ONLY:
        changeReadableWritable->setIcon(QPixmap(":/icons/readable.png"));
        changeReadableWritable->setIconText(tr("Нередактируемый текст"));
        break;
    case ReadableWritable::WRITABLE:
        changeReadableWritable->setIcon(QPixmap(":/icons/writable.png"));
        changeReadableWritable->setIconText(tr("Редактируемый текст"));
        break;
    }
}

void MainWindow::changeReadableWritable_triggered()
{
    auto wnd = mdi->activeSubWindow();
    if(wnd == nullptr) return;

    auto docView = qobject_cast<DocumentView*>(wnd->widget());
    if(docView == nullptr) return;

    switch (docView->getReadableWritable()) {
    case ReadableWritable::READABLE:
        docView->setReadableWritable(ReadableWritable::WRITABLE);
        break;
    case ReadableWritable::READABLE_ONLY:
        break;
    case ReadableWritable::WRITABLE:
        docView->setReadableWritable(ReadableWritable::READABLE);
        break;
    }
    setReadableWritable(docView->getReadableWritable());
}

void MainWindow::quit()
{
    qApp->quit();
}

void MainWindow::assignKeyboardShortcuts()
{
    if(wdgtSettings != nullptr)
    {
        wdgtSettings->show();
        return;
    }

    wdgtSettings = new WidgetSettings(this);
    connect(wdgtSettings, &WidgetSettings::setHotKey, keyPressEvent, &KeyPressEventFilter::setHotKey);
    wdgtSettings->show();
}

void MainWindow::setVisibleTextFormatToolBar(bool visible)
{
    auto actViewToolBarTextFormat = findChild<QAction*>("actViewToolBarTextFormat");
    if(actViewToolBarTextFormat == nullptr) return;
    TextFormateToolBarSingleton::getInstance(this)->setVisible(visible);
    actViewToolBarTextFormat->setChecked(visible);
}

void MainWindow::setVisibleDrawGraphToolBar(bool visible)
{
    auto actViewToolBarDrawing = findChild<QAction*>("actViewToolBarDrawing");
    if(actViewToolBarDrawing == nullptr) return;
    DrawGraphToolBarSingleton::getInstance(this)->setVisible(visible);
    actViewToolBarDrawing->setChecked(visible);
}

void MainWindow::subWindowActivated(QMdiSubWindow* subWnd)
{
    if(subWnd == nullptr) return;
    auto docView = qobject_cast<DocumentView*>(subWnd->widget());
    if(docView == nullptr) return;

    setReadableWritable(docView->getReadableWritable());
    statusLabel->setText(docView->getFileName());

    setVisibleTextFormatToolBar(docView->isTypeDoc(TypeDocumentView::TextDocumentView));
    setVisibleDrawGraphToolBar(docView->isTypeDoc(TypeDocumentView::GraphDocumentView));

    emit activatedDocumentView(docView);
}

void MainWindow::closeSubWnd(TypeDocumentView type)
{
    switch(type)
    {
    case TypeDocumentView::TextDocumentView:
        setVisibleTextFormatToolBar(false);
        break;
    case TypeDocumentView::GraphDocumentView:
        setVisibleDrawGraphToolBar(false);
        break;
    }
}

void MainWindow::closeDockTreeDirs()
{
    dockTreeDirs = nullptr;

    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setChecked(false);
}

void MainWindow::retranslateMenuFile()
{
    ToolbarElementsFactory<QMenu>::setText(this,"menuFile",tr("&Файл"));

    ToolbarElementsFactory<QMenu>::setText(this,"menuNewFile",tr("Новый файл"));
    ToolbarElementsFactory<QAction>::setText(this,"actNewTextFile",tr("Новый текстовый файл"));
    ToolbarElementsFactory<QAction>::setText(this,"actNewGraphFile",tr("Новый графический файл"));

    ToolbarElementsFactory<QAction>::setText(this,"actOpen",tr("Открыть файл"));
    ToolbarElementsFactory<QAction>::setText(this,"actOpenReadableOnly",tr("Открыть файл только для чтения"));

    ToolbarElementsFactory<QAction>::setText(this,"actSave",tr("Сохранить файл"));
    ToolbarElementsFactory<QAction>::setText(this,"actSaveAs",tr("Сохранить файл как"));
    ToolbarElementsFactory<QAction>::setText(this,"actPrint",tr("Печатать..."));

    auto tb_newFile = findChild<QToolButton*>("tb_newFile");
    if(tb_newFile != nullptr) tb_newFile->setWindowTitle(tr("Новый файл"));

    auto toolBarOpenFile = findChild<QAction*>("toolBarOpenFile");
    if(toolBarOpenFile != nullptr) toolBarOpenFile->setIconText(tr("Открыть текстовый файл"));
    auto toolBarSaveFile = findChild<QAction*>("toolBarSaveFile");
    if(toolBarSaveFile != nullptr) toolBarSaveFile->setIconText(tr("Сохранить файл"));
    auto toolBarPrintFile = findChild<QAction*>("toolBarPrintFile");
    if(toolBarPrintFile != nullptr) toolBarPrintFile->setIconText(tr("Печатать файл"));

    setReadableWritable(rw);
}
void MainWindow::retranslateMenuView()
{
    auto menuView = findChild<QMenu*>("menuView");
    if(menuView != nullptr) menuView->setTitle(tr("&Вид"));
    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setText(tr("Дерево каталогов"));
    auto actViewToolBarTextFormat = findChild<QAction*>("actViewToolBarTextFormat");
    if(actViewToolBarTextFormat != nullptr) actViewToolBarTextFormat->setText(tr("Формат текста"));
    ToolbarElementsFactory<QAction>::setText(this,"actViewToolBarDrawing",tr("Рисование"));
}
void MainWindow::retranslateMenuSettings()
{
    auto menuSettings = findChild<QMenu*>("menuSettings");
    if(menuSettings != nullptr) menuSettings->setTitle(tr("&Настройки"));
    auto actAssignKeyboardShortcuts = findChild<QAction*>("actAssignKeyboardShortcuts");
    if(actAssignKeyboardShortcuts != nullptr) actAssignKeyboardShortcuts->setText(tr("Установка горячик клавиш"));
    auto menuTheme = findChild<QMenu*>("menuTheme");
    if(menuTheme != nullptr) menuTheme->setTitle(tr("&Выбор темы"));
    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setText(tr("Тема MailSy"));
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setText(tr("Тема Integrid"));
}
void MainWindow::retranslateMenuLanguage()
{
    auto menuLanguage = findChild<QMenu*>("menuLanguage");
    if(menuLanguage != nullptr) menuLanguage->setTitle(tr("&Язык интерфейса"));
    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setText(tr("Русский"));
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setText(tr("Английский"));
}
void MainWindow::retranslateMenuHelp()
{
    auto menuHelp = findChild<QMenu*>("menuHelp");
    if(menuHelp != nullptr) menuHelp->setTitle(tr("&Справка"));
    auto actCallingHelp = findChild<QAction*>("actCallingHelp");
    if(actCallingHelp != nullptr) actCallingHelp->setText(tr("Вызов справки"));
}

void MainWindow::retranslate()
{
    setWindowTitle(tr("Простой текстовый редактор"));

    retranslateMenuFile();
    retranslateMenuView();
    retranslateMenuSettings();
    retranslateMenuLanguage();
    retranslateMenuHelp();

    if(wdtHelp != nullptr)
        wdtHelp->setWindowTitle(tr("Справка"));

    if(wdgtSettings != nullptr)
        wdgtSettings->retranslate();

    if(wdgtTreeDirs != nullptr)
        wdgtTreeDirs->retranslate();

    TextFormateToolBarSingleton::getInstance(this)->retranslate();
    DrawGraphToolBarSingleton::getInstance(this)->retranslate();
}

std::optional<DocumentView*> MainWindow::currentSubWindow()
{
    auto subwindow = mdi->activeSubWindow();
    if(subwindow == nullptr) return std::nullopt;
    if(subwindow->widget() == nullptr) return std::nullopt;
    DocumentView* docView = qobject_cast<DocumentView*>(subwindow->widget());
    if(docView == nullptr) return std::nullopt;

    return std::make_optional<DocumentView*>(docView);
}

void MainWindow::switchCheckedActionLanguage(std::shared_ptr<LanguageVisitor>& lv)
{
    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setChecked(lv->isLanguage(LANGUAGE::Russian));
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setChecked(lv->isLanguage(LANGUAGE::English));
}

void MainWindow::acceptLanguage(std::shared_ptr<LanguageVisitor>&& lv)
{
    lv->retranslate(this);
    switchCheckedActionLanguage(lv);
}

void MainWindow::setRussianLanguage()
{
    acceptLanguage(std::make_shared<LanguageRussianVisitor>());
}

void MainWindow::setEnglishLanguage()
{
    acceptLanguage(std::make_shared<LanguageEnglishVisitor>());
}

void MainWindow::setTheme(QString&& nfileQss)
{
    QFile fileQss(nfileQss);
    if(!fileQss.open(QFile::ReadOnly)) return;

    QTextStream stream(&fileQss);
    setStyleSheet(stream.readAll());

    auto listwnd = mdi->subWindowList();
    for (auto& wnd: listwnd) {
        if(wnd) wnd->setStyleSheet(stream.readAll());
    }

    fileQss.close();
}

void MainWindow::setThemeMailSy()
{
    setTheme(":/styles/MailSy.qss");

    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setIcon(QPixmap(":/icons/enable.png"));
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setIcon(QPixmap());
}

void MainWindow::setThemeIntegrid()
{
    setTheme(":/styles/Integrid.qss");

    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setIcon(QPixmap());
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setIcon(QPixmap(":/icons/enable.png"));
}

void MainWindow::viewTreeDirs()
{
    if(dockTreeDirs == nullptr)
    {
        auto wdgtTreeDirs = new WidgetTreeDirs(this);
        connect(wdgtTreeDirs, &WidgetTreeDirs::openFile, this, &MainWindow::openFilePathFromFileSystemModel);
        dockTreeDirs = new QDockWidget(this);
        dockTreeDirs->setWidget(wdgtTreeDirs);
        auto ef = new TreeDirsEventFilter();
        dockTreeDirs->installEventFilter(ef);
        connect(ef, &TreeDirsEventFilter::closeDock, this, &MainWindow::closeDockTreeDirs);
    }

    addDockWidget(Qt::LeftDockWidgetArea, dockTreeDirs);

    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setChecked(true);
}

void MainWindow::viewToolBarTextFormat()
{
    if(!TextFormateToolBarSingleton::getInstance(this)->isAdded())
    {
        addToolBar(Qt::TopToolBarArea,TextFormateToolBarSingleton::getInstance(this));
        TextFormateToolBarSingleton::getInstance(this)->setAdded();
    }

    auto actViewToolBarTextFormate = findChild<QAction*>("actViewToolBarTextFormat");
    if(actViewToolBarTextFormate == nullptr) return;
    if(!actViewToolBarTextFormate->isChecked())
    {
        TextFormateToolBarSingleton::getInstance(this)->setVisible(false);
        actViewToolBarTextFormate->setChecked(false);
    }
    else
    {
        TextFormateToolBarSingleton::getInstance(this)->setVisible(true);
        actViewToolBarTextFormate->setChecked(true);
    }
}

void MainWindow::viewToolBarDrawing()
{
    if(!DrawGraphToolBarSingleton::getInstance(this)->isAdded())
    {
        addToolBar(Qt::TopToolBarArea,DrawGraphToolBarSingleton::getInstance(this));
        DrawGraphToolBarSingleton::getInstance(this)->setAdded();
    }

    auto actViewToolBarDrawing = findChild<QAction*>("actViewToolBarDrawing");
    if(actViewToolBarDrawing == nullptr) return;
    if(!actViewToolBarDrawing->isChecked())
    {
        DrawGraphToolBarSingleton::getInstance(this)->setVisible(false);
        actViewToolBarDrawing->setChecked(false);
    }
    else
    {
        DrawGraphToolBarSingleton::getInstance(this)->setVisible(true);
        actViewToolBarDrawing->setChecked(true);
    }
}

