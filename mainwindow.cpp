#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolBar"
#include "QFileDialog"
#include "QTextStream"
#include "QDockWidget"
#include "QMdiSubWindow"
#include "languagevisitor.h"
#include "QPrinter"
#include "QPrintDialog"

void MainWindow::setMenu()
{
    auto menuFile = new QMenu(this);
    menuFile->setObjectName("menuFile");
    ui->menubar->addMenu(menuFile);
    auto actNew = new QAction(this);
    actNew->setObjectName("actNew");
    menuFile->addAction(actNew);
    connect(actNew, &QAction::triggered, this, &MainWindow::newFile);
    auto actOpen = new QAction(this);
    actOpen->setObjectName("actOpen");
    menuFile->addAction(actOpen);
    connect(actOpen, &QAction::triggered, this, &MainWindow::openFile);
    auto actOpenReadableOnly = new QAction(this);
    actOpenReadableOnly->setObjectName("actOpenReadableOnly");
    menuFile->addAction(actOpenReadableOnly);
    connect(actOpenReadableOnly, &QAction::triggered, this, &MainWindow::openFileReadableOnly);
    auto actSave = new QAction(this);
    actSave->setObjectName("actSave");
    menuFile->addAction(actSave);
    connect(actSave, &QAction::triggered, this, &MainWindow::saveFile);
    auto actSaveAs = new QAction(this);
    actSaveAs->setObjectName("actSaveAs");
    menuFile->addAction(actSaveAs);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    auto actPrint = new QAction(this);
    actPrint->setObjectName("actPrint");
    menuFile->addAction(actPrint);
    connect(actPrint, &QAction::triggered, this, &MainWindow::printFile);

    auto menuView = new QMenu(this);
    menuView->setObjectName("menuView");
    ui->menubar->addMenu(menuView);
    auto actViewTreeDirs = new QAction(this);
    actViewTreeDirs->setObjectName("actViewTreeDirs");
    actViewTreeDirs->setCheckable(true);
    actViewTreeDirs->setChecked(false);
    menuView->addAction(actViewTreeDirs);
    connect(actViewTreeDirs, &QAction::triggered, this, &MainWindow::viewTreeDirs);

    auto menuSettings = new QMenu(this);
    menuSettings->setObjectName("menuSettings");
    ui->menubar->addMenu(menuSettings);
    auto actAssignKeyboardShortcuts = new QAction(this);
    actAssignKeyboardShortcuts->setObjectName("actAssignKeyboardShortcuts");
    menuSettings->addAction(actAssignKeyboardShortcuts);
    connect(actAssignKeyboardShortcuts, &QAction::triggered, this, &MainWindow::assignKeyboardShortcuts);

    auto menuTheme = new QMenu(this);
    menuTheme->setObjectName("menuTheme");
    menuSettings->addMenu(menuTheme);
    auto actThemeMailSy = new QAction(this);
    actThemeMailSy->setObjectName("actThemeMailSy");
    menuTheme->addAction(actThemeMailSy);
    connect(actThemeMailSy, &QAction::triggered, this, &MainWindow::setThemeMailSy);
    auto actThemeIntegrid = new QAction(this);
    actThemeIntegrid->setObjectName("actThemeIntegrid");
    menuTheme->addAction(actThemeIntegrid);
    connect(actThemeIntegrid, &QAction::triggered, this, &MainWindow::setThemeIntegrid);

    auto menuLanguage = new QMenu(this);
    menuLanguage->setObjectName("menuLanguage");
    ui->menubar->addMenu(menuLanguage);
    auto actRussian = new QAction(this);
    actRussian->setObjectName("actRussian");
    actRussian->setCheckable(true);
    menuLanguage->addAction(actRussian);
    connect(actRussian, &QAction::triggered, this, &MainWindow::setRussianLanguage);
    auto actEnglish = new QAction(this);
    actEnglish->setObjectName("actEnglish");
    actEnglish->setCheckable(true);
    menuLanguage->addAction(actEnglish);
    connect(actEnglish, &QAction::triggered, this, &MainWindow::setEnglishLanguage);

    auto menuHelp = new QMenu(this);
    menuHelp->setObjectName("menuHelp");
    ui->menubar->addMenu(menuHelp);
    auto actCallingHelp = new QAction(this);
    actCallingHelp->setObjectName("actCallingHelp");
    menuHelp->addAction(actCallingHelp);
    connect(actCallingHelp, &QAction::triggered, this, &MainWindow::callingHelp);

    connect(menuHelp, &QMenu::triggered, this, &MainWindow::callingHelp);

    setThemeIntegrid();
}

void MainWindow::setToolBar()
{
    auto toptb = new QToolBar(this);
    auto toolBarNewFile = new QAction(this);
    toolBarNewFile->setObjectName("toolBarNewFile");
    toolBarNewFile->setIcon(QPixmap(":/icons/new.png"));
    connect(toolBarNewFile, &QAction::triggered, this, &MainWindow::newFile);
    toptb->addAction(toolBarNewFile);
    auto toolBarOpenFile = new QAction(this);
    toolBarOpenFile->setObjectName("toolBarOpenFile");
    toolBarOpenFile->setIcon(QPixmap(":/icons/open.png"));
    connect(toolBarOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    toptb->addAction(toolBarOpenFile);
    auto toolBarSaveFile = new QAction(this);
    toolBarSaveFile->setObjectName("toolBarSaveFile");
    toolBarSaveFile->setIcon(QPixmap(":/icons/save.png"));
    connect(toolBarSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    toptb->addAction(toolBarSaveFile);
    auto toolBarPrintFile = new QAction(this);
    toolBarPrintFile->setObjectName("toolBarPrintFile");
    toolBarPrintFile->setIcon(QPixmap(":/icons/print.png"));
    connect(toolBarPrintFile, &QAction::triggered, this, &MainWindow::printFile);
    toptb->addAction(toolBarPrintFile);

    toptb->addSeparator();

    changeReadableWritable = new QAction(this);
    changeReadableWritable->setObjectName("changeReadableWritable");
    setReadableWritable(ReadableWritable::WRITABLE);
    connect(changeReadableWritable, &QAction::triggered, this, &MainWindow::changeReadableWritable_triggered);
    toptb->addAction(changeReadableWritable);

    addToolBar(Qt::TopToolBarArea,toptb);
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
{
    ui->setupUi(this);

    mdi = new QMdiArea(this);
    mdi->setViewMode(QMdiArea::ViewMode::TabbedView);
    connect(mdi, &QMdiArea::subWindowActivated, this, &MainWindow::subWindowActivated);
    setCentralWidget(mdi);

    statusLabel = new QLabel(this);
    statusBar()->addWidget(statusLabel);

    installEventFilter(keyPressEvent);
    connect(keyPressEvent, &KeyPressEventFilter::newFile, this, &MainWindow::newFile);
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

void MainWindow::newFile()
{
    CreatorTextDocumentView creatorTextDocView;
    DocumentView* docView = creatorTextDocView.newDocumentView(mdi);
    mdi->addSubWindow(docView);
    docView->show();
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
    if(docView != nullptr)
    {
        mdi->addSubWindow(docView);
        docView->show();
    }
}

void MainWindow::openFile()
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.txt)"));

    openFilePath(fileName);
    pathDir = fileDialog.directory().absolutePath();
}

void MainWindow::openFileReadableOnly()
{
    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(this,
        tr("Открыть файл"), pathDir, tr("Текстовые файлы (*.txt)"));

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
            tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.txt)"));

        if(fileName.isEmpty()) return;

        int index = fileName.indexOf(".txt");
        if(index == -1 || fileName.length() - 4 != index)
        {
            fileName += ".txt";
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
        tr("Сохранить файл"), pathDir, tr("Текстовые файлы (*.txt)"));

    if(fileName.isEmpty()) return;

    int index = fileName.indexOf(".txt");
    if(index == -1 || fileName.length() - 4 != index)
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

void MainWindow::subWindowActivated(QMdiSubWindow* subWnd)
{
    if(subWnd == nullptr) return;
    auto docView = qobject_cast<DocumentView*>(subWnd->widget());
    if(docView == nullptr) return;

    setReadableWritable(docView->getReadableWritable());
    statusLabel->setText(docView->getFileName());
}

void MainWindow::closeDockTreeDirs()
{
    dockTreeDirs = nullptr;

    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setChecked(false);
}

void MainWindow::retranslate()
{
    setWindowTitle(tr("Простой текстовый редактор"));

    auto menuFile = findChild<QMenu*>("menuFile");
    if(menuFile != nullptr) menuFile->setTitle(tr("&Файл"));
    auto actNew = findChild<QAction*>("actNew");
    if(actNew != nullptr) actNew->setText(tr("Новый файл"));
    auto actOpen = findChild<QAction*>("actOpen");
    if(actOpen != nullptr) actOpen->setText(tr("Открыть файл"));
    auto actOpenReadableOnly = findChild<QAction*>("actOpenReadableOnly");
    if(actOpenReadableOnly != nullptr) actOpenReadableOnly->setText(tr("Открыть файл только для чтения"));
    auto actSave = findChild<QAction*>("actSave");
    if(actSave != nullptr) actSave->setText(tr("Сохранить файл"));
    auto actSaveAs = findChild<QAction*>("actSaveAs");
    if(actSaveAs != nullptr) actSaveAs->setText(tr("Сохранить файл как"));
    auto actPrint = findChild<QAction*>("actPrint");
    if(actPrint != nullptr) actPrint->setText(tr("Печатать..."));

    auto menuView = findChild<QMenu*>("menuView");
    if(menuView != nullptr) menuView->setTitle(tr("&Вид"));
    auto actViewTreeDirs = findChild<QAction*>("actViewTreeDirs");
    if(actViewTreeDirs != nullptr) actViewTreeDirs->setText(tr("Дерево каталогов"));

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

    auto menuLanguage = findChild<QMenu*>("menuLanguage");
    if(menuLanguage != nullptr) menuLanguage->setTitle(tr("&Язык интерфейса"));
    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setText(tr("Русский"));
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setText(tr("Английский"));

    auto menuHelp = findChild<QMenu*>("menuHelp");
    if(menuHelp != nullptr) menuHelp->setTitle(tr("&Справка"));
    auto actCallingHelp = findChild<QAction*>("actCallingHelp");
    if(actCallingHelp != nullptr) actCallingHelp->setText(tr("Вызов справки"));

    auto toolBarNewFile = findChild<QAction*>("toolBarNewFile");
    if(toolBarNewFile != nullptr) toolBarNewFile->setIconText(tr("Новый файл"));
    auto toolBarOpenFile = findChild<QAction*>("toolBarOpenFile");
    if(toolBarOpenFile != nullptr) toolBarOpenFile->setIconText(tr("Открыть файл"));
    auto toolBarSaveFile = findChild<QAction*>("toolBarSaveFile");
    if(toolBarSaveFile != nullptr) toolBarSaveFile->setIconText(tr("Сохранить файл"));
    auto toolBarPrintFile = findChild<QAction*>("toolBarPrintFile");
    if(toolBarPrintFile != nullptr) toolBarPrintFile->setIconText(tr("Печатать файл"));

    setReadableWritable(rw);

    if(wdtHelp != nullptr)
        wdtHelp->setWindowTitle(tr("Справка"));

    if(wdgtSettings != nullptr)
        wdgtSettings->retranslate();

    if(wdgtTreeDirs != nullptr)
        wdgtTreeDirs->retranslate();
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

void MainWindow::acceptLanguage(std::shared_ptr<LanguageVisitor>& lv)
{
    lv->retranslate(this);
}

void MainWindow::setRussianLanguage()
{
    std::shared_ptr<LanguageVisitor> languageVisitor =
            std::make_shared<LanguageRussianVisitor>();
    acceptLanguage(languageVisitor);

    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setChecked(true);
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setChecked(false);
}

void MainWindow::setEnglishLanguage()
{
    std::shared_ptr<LanguageVisitor> languageVisitor =
            std::make_shared<LanguageEnglishVisitor>();
    acceptLanguage(languageVisitor);

    auto actRussian = findChild<QAction*>("actRussian");
    if(actRussian != nullptr) actRussian->setChecked(false);
    auto actEnglish = findChild<QAction*>("actEnglish");
    if(actEnglish != nullptr) actEnglish->setChecked(true);
}

void MainWindow::setThemeMailSy()
{
    QFile fileQss(":/styles/MailSy.qss");
    if(!fileQss.open(QFile::ReadOnly)) return;

    QTextStream stream(&fileQss);
    setStyleSheet(stream.readAll());
    fileQss.close();

    auto actThemeMailSy = findChild<QAction*>("actThemeMailSy");
    if(actThemeMailSy != nullptr) actThemeMailSy->setIcon(QPixmap(":/icons/enable.png"));
    auto actThemeIntegrid = findChild<QAction*>("actThemeIntegrid");
    if(actThemeIntegrid != nullptr) actThemeIntegrid->setIcon(QPixmap());
}

void MainWindow::setThemeIntegrid()
{
    QFile fileQss(":/styles/Integrid.qss");
    if(!fileQss.open(QFile::ReadOnly)) return;

    QTextStream stream(&fileQss);
    setStyleSheet(stream.readAll());
    fileQss.close();

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

