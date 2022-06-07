#include "toolbarelementsfactory.h"

template<class T>
std::optional<T *> ToolbarElementsFactory<T>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon)
{
    Q_UNUSED(nameObject);
    Q_UNUSED(parent);
    Q_UNUSED(checkable);
    Q_UNUSED(icon);
    return std::nullopt;
}

template<>
std::optional<QAction *> ToolbarElementsFactory<QAction>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon)
{
    auto t = new QAction(parent);
    if(!t) return std::nullopt;
    t->setObjectName(nameObject);
    t->setCheckable(checkable);
    t->setIcon(icon);
    return std::make_optional<QAction*>(t);
}

template<>
std::optional<QMenu *> ToolbarElementsFactory<QMenu>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon)
{
    Q_UNUSED(checkable);
    auto t = new QMenu(parent);
    if(!t) return std::nullopt;
    t->setObjectName(nameObject);
    t->setIcon(icon);
    return std::make_optional<QMenu*>(t);
}

template<>
std::optional<QToolButton *> ToolbarElementsFactory<QToolButton>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon)
{
    Q_UNUSED(checkable);
    auto t = new QToolButton(parent);
    if(!t) return std::nullopt;
    t->setObjectName(nameObject);
    t->setIcon(icon);
    return std::make_optional<QToolButton*>(t);
}

template<>
std::optional<QComboBox *> ToolbarElementsFactory<QComboBox>::create(const QString &&nameObject, QWidget *parent, bool checkable, const QPixmap &&icon)
{
    Q_UNUSED(checkable);
    Q_UNUSED(icon);
    auto t = new QComboBox(parent);
    if(!t) return std::nullopt;
    t->setObjectName(nameObject);
    return std::make_optional<QComboBox*>(t);
}

template<class T>
void ToolbarElementsFactory<T>::setText(QObject *obj, const QString &&nameObject,const QString &&text)
{
    Q_UNUSED(obj);
    Q_UNUSED(nameObject);
    Q_UNUSED(text);
}

template<>
void ToolbarElementsFactory<QAction>::setText(QObject* obj,const QString&& nameObject,const QString&& text)
{
    Q_ASSERT(obj != nullptr);
    auto act = obj->findChild<QAction*>(nameObject);
    if(act != nullptr) act->setText(text);
}

template<>
void ToolbarElementsFactory<QMenu>::setText(QObject* obj,const QString&& nameObject,const QString&& text)
{
    Q_ASSERT(obj != nullptr);
    auto menu = obj->findChild<QMenu*>(nameObject);
    if(menu != nullptr) menu->setTitle(text);
}

template<>
void ToolbarElementsFactory<QComboBox>::setText(QObject* obj,const QString&& nameObject,const QString&& text)
{
    Q_ASSERT(obj != nullptr);
    auto combo = obj->findChild<QComboBox*>(nameObject);
    if(combo != nullptr) combo->setToolTip(text);
}
