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

#ifndef Q4CW_SLE_CUSTOMWIDGETS_H
#define Q4CW_SLE_CUSTOMWIDGETS_H

#include <QtCore/QList>
#include <QtCore/QtPlugin>

#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>

#include "customwidget.h"

#include "lineedit.h"
#include "searchlineedit.h"
#include "simplebutton.h"

// Custom Widgets

class LineEdit_CW : public QObject, public CustomWidget<LineEdit>
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	explicit LineEdit_CW(QObject* parent = 0);

	//QWidget* createWidget(QWidget* parent);
};


class SearchLineEdit_CW : public QObject, public CustomWidget<SearchLineEdit>
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	explicit SearchLineEdit_CW(QObject* parent = 0);

	QWidget* createWidget(QWidget* parent);
};


class SimpleButton_CW : public QObject, public CustomWidget<SimpleButton>
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	explicit SimpleButton_CW(QObject* parent = 0);

	QWidget* createWidget(QWidget* parent);
};


// Collection

class WidgetCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
	explicit WidgetCollection(QObject* parent = 0);

	QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
	QList<QDesignerCustomWidgetInterface*> m_plugins;
};

#endif // Q4CW_SLE_CUSTOMWIDGETS_H
