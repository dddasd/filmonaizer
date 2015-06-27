/*******************************************************************************
Copyright (C) 2008-2009  Ritt K.

This file is part of a SearchLineEdit component.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
********************************************************************************/

#ifndef Q4CW_SLE_SIMPLEBUTTON_H
#define Q4CW_SLE_SIMPLEBUTTON_H

#include <QAbstractButton>

#include "global.h"

class QMenu;

class SimpleButtonPrivate;
class Q4CW_SLE_EXPORT SimpleButton : public QAbstractButton
{
	Q_OBJECT

public:
	explicit SimpleButton(QWidget* parent = 0);
	explicit SimpleButton(const QIcon& icon, QWidget* parent = 0);
	~SimpleButton();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

#ifndef QT_NO_MENU
	QMenu* menu() const;
	void setMenu(QMenu* menu);

public Q_SLOTS:
	void showMenu();
#endif

protected:
	bool event(QEvent* event);
	void actionEvent(QActionEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent*);

private:
	Q_DISABLE_COPY(SimpleButton)
	Q_DECLARE_PRIVATE(SimpleButton)
#ifndef QT_NO_MENU
	Q_PRIVATE_SLOT(d_func(), void _q_updateButtonDown())
#endif
	SimpleButtonPrivate* const d_ptr;
};

#endif // Q4CW_SLE_SIMPLEBUTTON_H
