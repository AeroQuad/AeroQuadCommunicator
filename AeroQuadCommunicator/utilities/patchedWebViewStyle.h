#ifndef PATCHEDWEBVIEWSTYLE_H
#define PATCHEDWEBVIEWSTYLE_H

#include <QProxyStyle>
#include <QWebView>
#include <QStyleOption>

class PatchedWebViewStyle : public QProxyStyle
{
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
    {
        if( element == QStyle::CE_CheckBox ||
            element == QStyle::CE_RadioButton )
            option->styleObject->setProperty( "_q_no_animation", true );
        QProxyStyle::drawControl( element, option, painter, widget );
    }
};

#endif // PATCHEDWEBVIEWSTYLE_H
