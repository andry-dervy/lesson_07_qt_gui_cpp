#include "widgetsettings.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>

WidgetSettings::WidgetSettings(QWidget *parent) : QDialog(parent)
{
    QFormLayout* formLayout = new QFormLayout(this);

    QLabel* lblShotKey_NewFile = new QLabel(this);
    lblShotKey_NewFile->setObjectName("lblShotKey_NewFile");
    leHotKey_NewFile = new QLineEdit("Ctrl+N",this);
    leHotKey_NewFile->setReadOnly(true);
    leHotKey_NewFile->installEventFilter(this);
    formLayout->addRow(lblShotKey_NewFile,leHotKey_NewFile);

    QLabel* lblShotKey_OpenFile = new QLabel(this);
    lblShotKey_OpenFile->setObjectName("lblShotKey_OpenFile");
    leHotKey_OpenFile = new QLineEdit("Ctrl+O",this);
    leHotKey_OpenFile->setReadOnly(true);
    leHotKey_OpenFile->installEventFilter(this);
    formLayout->addRow(lblShotKey_OpenFile,leHotKey_OpenFile);

    QLabel* lblShotKey_SaveFile = new QLabel(this);
    lblShotKey_SaveFile->setObjectName("lblShotKey_SaveFile");
    leHotKey_SaveFile = new QLineEdit("Ctrl+S",this);
    leHotKey_SaveFile->setReadOnly(true);
    leHotKey_SaveFile->installEventFilter(this);
    formLayout->addRow(lblShotKey_SaveFile,leHotKey_SaveFile);

    QLabel* lblShotKey_Quit = new QLabel(this);
    lblShotKey_Quit->setObjectName("lblShotKey_Quit");
    leHotKey_Quit = new QLineEdit("Ctrl+Q",this);
    leHotKey_Quit->setReadOnly(true);
    leHotKey_Quit->installEventFilter(this);
    formLayout->addRow(lblShotKey_Quit,leHotKey_Quit);

    setLayout(formLayout);

    setMinimumSize(QSize(200,150));
    retranslate();
}

void WidgetSettings::retranslate()
{
    setWindowTitle(tr("Установка горячих клавиш"));
    QLabel* lblShotKey_NewFile = findChild<QLabel*>("lblShotKey_NewFile");
    if(lblShotKey_NewFile != nullptr) lblShotKey_NewFile->setText(tr("Создать новый файл"));
    QLabel* lblShotKey_OpenFile = findChild<QLabel*>("lblShotKey_OpenFile");
    if(lblShotKey_OpenFile != nullptr) lblShotKey_OpenFile->setText(tr("Открыть файл"));
    QLabel* lblShotKey_SaveFile = findChild<QLabel*>("lblShotKey_SaveFile");
    if(lblShotKey_SaveFile != nullptr) lblShotKey_SaveFile->setText(tr("Сохранить файл"));
    QLabel* lblShotKey_Quit = findChild<QLabel*>("lblShotKey_Quit");
    if(lblShotKey_Quit != nullptr) lblShotKey_Quit->setText(tr("Выход"));
}

QString WidgetSettings::getStrFromHotKeys(std::pair<int,Qt::KeyboardModifiers>& p)
{
    QString mods;

    switch (p.second) {
    case Qt::KeyboardModifier::ControlModifier:
        mods += "Ctrl+";
        break;
    }

    QString s;

    if(p.first >= Qt::Key_F1 &&
       p.first <= Qt::Key_F12)
    {
        switch (p.first) {
        case Qt::Key_F1: s = "F1"; break;
        case Qt::Key_F2: s = "F2"; break;
        case Qt::Key_F3: s = "F3"; break;
        case Qt::Key_F4: s = "F4"; break;
        case Qt::Key_F5: s = "F5"; break;
        case Qt::Key_F6: s = "F6"; break;
        case Qt::Key_F7: s = "F7"; break;
        case Qt::Key_F8: s = "F8"; break;
        case Qt::Key_F9: s = "F9"; break;
        case Qt::Key_F10: s = "F10"; break;
        case Qt::Key_F11: s = "F11"; break;
        case Qt::Key_F12: s = "F12"; break;
        }
        if(p.second != Qt::KeyboardModifier::ControlModifier) return s;
        return "Ctrl+"+s;
    }
    else if(p.first >= Qt::Key_A &&
            p.first <= Qt::Key_Z)
    {
        if(p.second != Qt::KeyboardModifier::ControlModifier) return QString();
        s = QChar(p.first);
        return "Ctrl+"+s;
    }
    return QString();
}

bool WidgetSettings::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        return true;
    }
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent == nullptr) return false;

        auto hotKey = std::make_pair(keyEvent->key(), keyEvent->modifiers());
        QString strHotKey = getStrFromHotKeys(hotKey);
        QLineEdit* le = qobject_cast<QLineEdit*>(obj);
        le->clear();
        if(strHotKey.isEmpty()) return false;

        if(le == leHotKey_NewFile)
        {
            leHotKey_NewFile->setText(strHotKey);
            emit setHotKey(eTYPE_HOTKEY::NewFile,hotKey);
        }
        else if(le == leHotKey_OpenFile)
        {
            leHotKey_OpenFile->setText(strHotKey);
            emit setHotKey(eTYPE_HOTKEY::OpenFile,hotKey);
        }
        else if(le == leHotKey_SaveFile)
        {
            leHotKey_SaveFile->setText(strHotKey);
            emit setHotKey(eTYPE_HOTKEY::SaveFile,hotKey);
        }
        else if(le == leHotKey_Quit)
        {
            leHotKey_Quit->setText(strHotKey);
            emit setHotKey(eTYPE_HOTKEY::Quit,hotKey);
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}
