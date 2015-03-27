/***********************************************************************
** MyAcolito, copyright @2012,
** Autor, Antonio Ramírez Marti
** morgulero@gmail.com
**
** Codigo extraido de
** http://www.developer.nokia.com/Community/Wiki/Creating_a_custom_custom_line_editor_from_QLineEdit
***********************************************************************/

#ifndef ACOLITOCUSTOMLINEEDIT_H
#define ACOLITOCUSTOMLINEEDIT_H

//Ajuste de version
#include <QLineEdit>
#include <QPaintEvent>
#include <QFocusEvent>
#include <QString>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionFrame>
#include <QStyleOption>

class AcolitoCustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    AcolitoCustomLineEdit(QWidget *parent = 0);
    virtual ~AcolitoCustomLineEdit();

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

#endif // AcolitoCustomLineEdit_H
