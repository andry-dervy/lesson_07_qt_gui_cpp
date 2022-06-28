#ifndef TOOLBARELEMENTSFACTORY_H
#define TOOLBARELEMENTSFACTORY_H

#include <QString>
#include <QAction>
#include <QMenu>
#include <QPixmap>
#include <QToolButton>
#include <QComboBox>

template<class T>
class ToolbarElementsFactory
{
public:
    ToolbarElementsFactory() = default;
    virtual ~ToolbarElementsFactory() = default;
public:
    static std::optional<T *> create(
            const QString&& nameObject, QWidget *parent = nullptr, bool checkable = false, const QPixmap&& icon = QPixmap());

    static void setText(QObject* parent,const QString&& nameObject, const QString &&text);
    static void setChecked(QObject* parent,const QString&& nameObject, const bool b);
    static bool isChecked(QObject* parent,const QString&& nameObject);
};



#endif // TOOLBARELEMENTSFACTORY_H
