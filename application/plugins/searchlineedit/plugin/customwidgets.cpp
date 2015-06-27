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

#include "customwidgets.h"

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QPixmap>

static const char* groupC = "Q4Components";

// Custom Widgets

LineEdit_CW::LineEdit_CW(QObject* parent) : QObject(parent),
	CustomWidget<LineEdit>(QLatin1String("lineedit.h"),
							QLatin1String(groupC),
							tr("A Line edit with ability to set side widgets"),
							QIcon(QLatin1String(":/icons/lineedit.png")))
{
}


SearchLineEdit_CW::SearchLineEdit_CW(QObject* parent) : QObject(parent),
	CustomWidget<SearchLineEdit>(QLatin1String("searchlineedit.h"),
							QLatin1String(groupC),
							tr("A Line edit with a menu and clear buttons"),
							QIcon(QLatin1String(":/icons/searchlineedit.png")))
{
}

QWidget* SearchLineEdit_CW::createWidget(QWidget* parent)
{
	SearchLineEdit* sle = new SearchLineEdit(parent);
	QMenu* menu = new QMenu(sle);
	menu->addAction("Test");
	sle->setMenu(menu);
	sle->setClearButtonShown(true);
	return sle;
}


SimpleButton_CW::SimpleButton_CW(QObject* parent) : QObject(parent),
	CustomWidget<SimpleButton>(QLatin1String("simplebutton.h"),
							QLatin1String(groupC),
							tr("A simple ToolButton-like button with menu"),
							QIcon(QLatin1String(":/icons/simplebutton.png")))
{
}

QWidget* SimpleButton_CW::createWidget(QWidget* parent)
{
	SimpleButton* sb = new SimpleButton(parent);
	QMenu* menu = new QMenu(sb);
	menu->addAction("Test");
	sb->setMenu(menu);
	return sb;
}


// Collection

WidgetCollection::WidgetCollection(QObject* parent) : QObject(parent)
{
	m_plugins.push_back(new LineEdit_CW(this));
	m_plugins.push_back(new SearchLineEdit_CW(this));
	m_plugins.push_back(new SimpleButton_CW(this));
}

QList<QDesignerCustomWidgetInterface*> WidgetCollection::customWidgets() const
{
	return m_plugins;
}

Q_EXPORT_PLUGIN2(q4c_searchlineedit, WidgetCollection)
