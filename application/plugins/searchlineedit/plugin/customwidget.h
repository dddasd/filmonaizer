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

#ifndef Q4CW_SLE_CUSTOMWIDGET_H
#define Q4CW_SLE_CUSTOMWIDGET_H

#include <QtCore/QSize>
#include <QtCore/QString>

#include <QtDesigner/QDesignerCustomWidgetInterface>

#include <QtGui/QIcon>

// Parametrizable Template for custom widgets.
// Originated by QtCreator's CustomWidget.

template <class Widget>
class CustomWidget : public QDesignerCustomWidgetInterface
{
public:
	explicit CustomWidget(const QString& includeFile, const QString& group,
							const QString& toolTip = QString(), const QIcon& icon = QIcon(),
							const QSize& size = QSize(), bool isContainer = false) :
		m_includeFile(includeFile),
		m_group(group),
		m_toolTip(toolTip),
		m_icon(icon),
		m_size(size),
		m_isContainer(isContainer),
		m_initialized(false)
	{}

	QWidget* createWidget(QWidget* parent)
	{ return new Widget(parent); }

	QString name() const
	{ return QLatin1String(Widget::staticMetaObject.className()); }
	QString group() const
	{ return m_group; }
	QString includeFile() const
	{ return m_includeFile; }
	QString toolTip() const
	{ return m_toolTip; }
	QString whatsThis() const
	{ return m_toolTip; }
	QIcon icon() const
	{ return m_icon; }
	bool isContainer() const
	{ return m_isContainer; }
	bool isInitialized() const
	{ return m_initialized; }
	void initialize(QDesignerFormEditorInterface*)
	{ if(!m_initialized) m_initialized = true; }
	QString domXml() const;

private:
	const QString m_includeFile;
	const QString m_group;
	const QString m_toolTip;
	const QIcon m_icon;
	const QSize m_size;
	const bool m_isContainer;

	bool m_initialized;
};

template <class Widget>
QString CustomWidget<Widget>::domXml() const
{
	const QString className = name();

	QString displayName = className;
	// Strip namespaces
	const int index = displayName.lastIndexOf(QLatin1Char(':'));
	if(index != -1)
		displayName.remove(0, index + 1);

	QString name = className;
	// Name: 'QClass' -> 'class'
	if(name.startsWith(QLatin1Char('Q')))
		name.remove(0, 1);
	name[0] = name.at(0).toLower();

	QString geometryProperty;
	if(!m_size.isEmpty())
	{
		geometryProperty = QString("<property name=\"geometry\"><rect><x>0</x><y>0</y><width>%1</width><height>%2</height></rect></property>")
							.arg(m_size.width())
							.arg(m_size.height());
	}

	QString str = QString("<ui displayname=\"%1\" language=\"c++\"><widget class=\"%2\" name=\"%3\">%4</widget></ui>")
			.arg(displayName)
			.arg(className)
			.arg(name)
			.arg(geometryProperty);

	return str;
}

#endif // Q4CW_SLE_CUSTOMWIDGET_H
