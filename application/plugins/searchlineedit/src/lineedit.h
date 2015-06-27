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

#ifndef Q4CW_SLE_LINEEDIT_H
#define Q4CW_SLE_LINEEDIT_H

#include <QLineEdit>

#include "global.h"

class LineEditPrivate;
class Q4CW_SLE_EXPORT LineEdit : public QLineEdit
{
	Q_OBJECT
	Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
	Q_PROPERTY(QString hintText READ hintText WRITE setHintText)

public:
	enum WidgetPosition
	{
		LeftSide,
		RightSide
	};

	explicit LineEdit(QWidget* parent = 0);
	explicit LineEdit(const QString& text, QWidget* parent = 0);
	~LineEdit();

#if QT_VERSION < 0x040500
	QSize sizeHint() const;

	void getTextMargins(int* left, int* top, int* right, int* bottom) const;
	void setTextMargins(int left, int top, int right, int bottom);
#endif

	int spacing() const;
	void setSpacing(int spacing);

	QString hintText() const;
	void setHintText(const QString& text);

	QWidget* widget(WidgetPosition position) const;
	void setWidget(WidgetPosition position, QWidget* widget);

protected:
	bool eventFilter(QObject* watched, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);
#if QT_VERSION < 0x040500
	void changeEvent(QEvent* event);
#endif

protected:
	LineEdit(LineEditPrivate& dd, QWidget* parent);

protected:
	LineEditPrivate* const d_ptr;

private:
	Q_DISABLE_COPY(LineEdit)
	Q_DECLARE_PRIVATE(LineEdit)
};

#endif // Q4CW_SLE_LINEEDIT_H
