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

#ifndef Q4CW_PROXYSTYLE_P_H
#define Q4CW_PROXYSTYLE_P_H

#if QT_VERSION < 0x040500
#include <QApplication>
#include <QStyle>
#include <QWidget>

class ProxyStyle : public QStyle
{
	Q_OBJECT

public:
	explicit ProxyStyle(QWidget* parent = 0) : QStyle()
	{
		if(parent)
			setParent(parent);
		if(parent && parent->parentWidget())
			m_style = parent->parentWidget()->style();
		else
			m_style = QApplication::style();
	}

	QStyle* style() const
	{ return m_style; }

	virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = 0) const
	{ m_style->drawComplexControl(control, option, painter, widget); }
	virtual void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = 0) const
	{ m_style->drawControl(element, option, painter, widget); }
	virtual void drawItemPixmap(QPainter* painter, const QRect& rectangle, int alignment, const QPixmap& pixmap) const
	{ m_style->drawItemPixmap(painter, rectangle, alignment, pixmap); }
	virtual void drawItemText(QPainter* painter, const QRect& rectangle, int alignment, const QPalette& palette, bool enabled, const QString& text, QPalette::ColorRole textRole = QPalette::NoRole) const
	{ m_style->drawItemText(painter, rectangle, alignment, palette, enabled, text, textRole); }
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = 0) const
	{ m_style->drawPrimitive(element, option, painter, widget); }
	virtual QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap& pixmap, const QStyleOption* option) const
	{ return m_style->generatedIconPixmap(iconMode, pixmap, option); }
	virtual SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex* option, const QPoint& position, const QWidget* widget = 0) const
	{ return m_style->hitTestComplexControl(control, option, position, widget); }
	virtual QRect itemPixmapRect(const QRect& rectangle, int alignment, const QPixmap& pixmap) const
	{ return m_style->itemPixmapRect(rectangle, alignment, pixmap); }
	virtual QRect itemTextRect(const QFontMetrics& metrics, const QRect& rectangle, int alignment, bool enabled, const QString& text) const
	{ return m_style->itemTextRect(metrics, rectangle, alignment, enabled, text); }
	virtual int pixelMetric(PixelMetric metric, const QStyleOption* option = 0, const QWidget* widget = 0) const
	{ return m_style->pixelMetric(metric, option, widget); }
	virtual void polish(QWidget* widget)
	{ m_style->polish(widget); }
	virtual void polish(QApplication* application)
	{ m_style->polish(application); }
	virtual void polish(QPalette& palette)
	{ m_style->polish(palette); }
	virtual QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentsSize, const QWidget* widget = 0) const
	{ return m_style->sizeFromContents(type, option, contentsSize, widget); }
	virtual QPalette standardPalette() const
	{ return m_style->standardPalette(); }
	virtual int styleHint(StyleHint hint, const QStyleOption* option = 0, const QWidget* widget = 0, QStyleHintReturn* returnData = 0) const
	{ return m_style->styleHint(hint, option, widget, returnData); }
	virtual QRect subControlRect(ComplexControl control, const QStyleOptionComplex* option, SubControl subControl, const QWidget* widget = 0) const
	{ return m_style->subControlRect(control, option, subControl, widget); }
	virtual void unpolish(QWidget* widget)
	{ m_style->unpolish(widget); }
	virtual void unpolish(QApplication* application)
	{ m_style->unpolish(application); }
	virtual QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption* opt = 0, const QWidget* widget = 0) const
	{ return m_style->standardPixmap(standardPixmap, opt, widget); }
	virtual QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget = 0) const
	{ return m_style->subElementRect(element, option, widget); }
	virtual bool event(QEvent* e)
	{ return m_style->event(e); }
	virtual bool eventFilter(QObject* o, QEvent* e)
	{ return m_style->eventFilter(o, e); }

protected Q_SLOTS:
	int layoutSpacingImplementation(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption* option = 0, const QWidget* widget = 0) const
	{ return m_style->layoutSpacing(control1, control2, orientation, option, widget); }

	QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption* option = 0, const QWidget* widget = 0) const
	{ return m_style->standardIcon(standardIcon, option, widget); }

private:
	QStyle* m_style;
};
#endif // QT_VERSION

#endif // Q4CW_PROXYSTYLE_P_H
