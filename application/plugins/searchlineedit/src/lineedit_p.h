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

#ifndef Q4CW_SLE_LINEEDIT_P_H
#define Q4CW_SLE_LINEEDIT_P_H

#include <QtCore/QString>

#include <QWidget>

#include "global.h"
#if QT_VERSION < 0x040500
#include "lineedit.h"
#include "proxystyle_p.h"
#endif

class Q4CW_SLE_EXPORT LineEditPrivate
{
	Q_DECLARE_PUBLIC(LineEdit)

public:
	LineEditPrivate();
	virtual ~LineEditPrivate();

	void init();
	void layoutWidgets();
	void updateTextMargins();
	QRect adjustedContentsRect() const;

	LineEdit* q_ptr;

	QWidget* leftWidget;
	QWidget* rightWidget;

#if QT_VERSION < 0x040500
	int leftTextMargin;
	int topTextMargin;
	int rightTextMargin;
	int bottomTextMargin;
#endif // QT_VERSION
	int spacing;

	QString hintText;
};


#if QT_VERSION < 0x040500
class LineEditProxyStyle : public ProxyStyle
{
	Q_OBJECT

public:
	explicit LineEditProxyStyle(QWidget* parent = 0) : ProxyStyle(parent)
	{}

	QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget = 0) const
	{
		QRect r = ProxyStyle::subElementRect(element, option, widget);
		if(element == SE_LineEditContents)
		{
			const LineEdit* le = qobject_cast<const LineEdit*>(widget);
			if(le)
			{
				int left, top, right, bottom;
				le->getTextMargins(&left, &top, &right, &bottom);
				r.adjust(left, top, -right, -bottom);
			}
		}
		return r;
	}
};
#endif // QT_VERSION

#endif // Q4CW_SLE_LINEEDIT_P_H
