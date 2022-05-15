#ifndef WIDGETSETTINGS_H
#define WIDGETSETTINGS_H

#include <QDialog>
#include <QLineEdit>

enum class  eTYPE_HOTKEY
{
    NewFile,
    OpenFile,
    SaveFile,
    Quit,
};

class WidgetSettings : public QDialog
{
    Q_OBJECT
public:
    explicit WidgetSettings(QWidget *parent = nullptr);
    void retranslate();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLineEdit* leHotKey_NewFile;
    QLineEdit* leHotKey_OpenFile;
    QLineEdit* leHotKey_SaveFile;
    QLineEdit* leHotKey_Quit;

    QString getStrFromHotKeys(std::pair<int,Qt::KeyboardModifiers>& p);

signals:
    void setHotKey(eTYPE_HOTKEY typeHotKey,std::pair<int,Qt::KeyboardModifiers>& p);
};

#endif // WIDGETSETTINGS_H
