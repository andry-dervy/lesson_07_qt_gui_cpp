#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QMdiArea>

#include <memory>
#include <keypresseventfilter.h>
#include <widgetsettings.h>
#include <widgettreedirs.h>
#include "documentview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LanguageVisitor;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void retranslate();

    std::optional<DocumentView*> currentSubWindow();

private:
    Ui::MainWindow *ui;

    QString pathDir;
    QDialog* wdtHelp;
    KeyPressEventFilter* keyPressEvent;
    WidgetSettings* wdgtSettings;
    QDockWidget* dockTreeDirs;
    WidgetTreeDirs* wdgtTreeDirs;

    ReadableWritable rw;

    QMdiArea* mdi;
    QLabel* statusLabel;
    QAction* changeReadableWritable;

private:
    void acceptLanguage(std::shared_ptr<LanguageVisitor>& lv);
    void setMenu();
    void setToolBar();

    void setReadableWritable(ReadableWritable aRW);

private slots:
    void setRussianLanguage();
    void setEnglishLanguage();

    void setThemeMailSy();
    void setThemeIntegrid();

    void viewTreeDirs();
    void newFile();
    void openFilePath(QString& path, ReadableWritable aRW);
    void openFile();
    void openFileReadableOnly();
    void openFilePathFromFileSystemModel(QString& fileName);

    void saveFile();
    void saveFileAs();

    void printFile();

    void callingHelp();
    void changeReadableWritable_triggered();

    void quit();
    void assignKeyboardShortcuts();
    void subWindowActivated(QMdiSubWindow* subWnd);
    void closeDockTreeDirs();
};



#endif // MAINWINDOW_H
