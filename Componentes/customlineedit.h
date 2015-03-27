/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** Codigo extraido de
** http://www.developer.nokia.com/Community/Wiki/Creating_a_custom_custom_line_editor_from_QLineEdit
***********************************************************************/

#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

//Ajuste de version
#include <QLineEdit>
#include <QPaintEvent>
#include <QFocusEvent>
#include <QString>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QStyleOptionFrame>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = 0);
    virtual ~CustomLineEdit();

public:
    void setEmptyMessage( const QString &msg );

protected:
    void paintEvent( QPaintEvent *ev );
    void focusInEvent( QFocusEvent *ev );
    void focusOutEvent( QFocusEvent *ev );

private:
    QString emptyMessage;
    bool drawEmptyMsg;
};

#endif // CustomLineEdit_H
