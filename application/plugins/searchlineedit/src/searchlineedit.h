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

#ifndef Q4CW_SLE_SEARCHLINEEDIT_H
#define Q4CW_SLE_SEARCHLINEEDIT_H

#include "global.h"
#include "lineedit.h"

class QMenu;

class SearchLineEditPrivate;
class Q4CW_SLE_EXPORT SearchLineEdit : public LineEdit
{
	Q_OBJECT
	Q_PROPERTY(bool showClearButton READ isClearButtonShown WRITE setClearButtonShown)

public:
	explicit SearchLineEdit(QWidget* parent = 0);
	explicit SearchLineEdit(const QString& text, QWidget* parent = 0);
	~SearchLineEdit();

	bool isClearButtonShown() const;
	void setClearButtonShown(bool showClearButton);

#ifndef QT_NO_MENU
	QMenu* menu() const;
	void setMenu(QMenu* menu);
#endif

protected:
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);

private:
	Q_DISABLE_COPY(SearchLineEdit)
	Q_DECLARE_PRIVATE(SearchLineEdit)
	Q_PRIVATE_SLOT(d_func(), void _q_updateClearButton())
	Q_PRIVATE_SLOT(d_func(), void _q_clearButtonClicked())
};

#endif // Q4CW_SLE_SEARCHLINEEDIT_H
