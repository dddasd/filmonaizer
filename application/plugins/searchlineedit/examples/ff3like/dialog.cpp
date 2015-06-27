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

#include "dialog.h"

#include <QtGui>

#include "lineedit.h"
#include "simplebutton.h"

Dialog::Dialog(QWidget* parent) : QDialog(parent)
{
	QColor c(238, 236, 219);
	QPalette pal = palette();
	pal.setColor(QPalette::AlternateBase, QColor(135, 215, 240));
	pal.setColor(QPalette::Button, c);
	pal.setColor(QPalette::Window, c);
	setPalette(pal);

	menu = createMenu();

	menuButton = new SimpleButton(this);
	menuButton->setAutoFillBackground(true);
	menuButton->setIconSize(QSize(18, 18));
	menuButton->setMenu(menu);


	searchButton = new SimpleButton(this);
	searchButton->setIcon(QIcon(QLatin1String(":/icons/glass.png")));
	searchButton->setIconSize(QSize(18, 18));
#ifndef QT_NO_TOOLTIP
	searchButton->setToolTip(tr("Search"));
#endif

	lineEdit = new LineEdit(this);
	lineEdit->setMinimumHeight(20);
	lineEdit->setWidget(LineEdit::LeftSide, menuButton);
	lineEdit->setWidget(LineEdit::RightSide, searchButton);

	connect(searchButton, SIGNAL(clicked()),
			lineEdit, SLOT(clear()));

	QLineEdit* dummyLineEdit = new QLineEdit(tr("dummy"), this);
	dummyLineEdit->setFocus();


	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(lineEdit);
	mainLayout->addWidget(dummyLineEdit);
	setLayout(mainLayout);

	setWindowTitle(tr("Search panel in FF3-style Look'n'Feel"));
	setWindowIcon(QIcon(QLatin1String(":/icons/ico01.png")));
	setMinimumWidth(270);
	setFixedHeight(sizeHint().height());

	Q_ASSERT(menu->actions().isEmpty());
	_slot_action_triggered(menu->actions().first());
}

QMenu* Dialog::createMenu()
{
	QActionGroup* actionGroup = new QActionGroup(this);
	{
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico00.png")), tr("Google"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico01.png")), tr("Yandex"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico02.png")), tr("Price.ru"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico03.png")), tr("Rambler"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico04.png")), tr("Wikipedia (ru)"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico05.png")), tr("Goods@Mail.Ru"));
		(void)actionGroup->addAction(QIcon(QLatin1String(":/icons/ico06.png")), tr("Yandex.Dictionaries"));
	}
	connect(actionGroup, SIGNAL(triggered(QAction*)),
			this, SLOT(_slot_action_triggered(QAction*)));

	QAction* dummyAction = new QAction(tr("Search plugins preferences..."), this);

	QMenu* resMenu = new QMenu(this);
	resMenu->addActions(actionGroup->actions());
	(void)resMenu->addSeparator();
	(void)resMenu->addAction(dummyAction);

	return resMenu;
}

void Dialog::_slot_action_triggered(QAction* action)
{
	if(!action)
		return;

	setUpdatesEnabled(false);

	lineEdit->setHintText(action->text());

	menuButton->setIcon(action->icon());
#ifndef QT_NO_TOOLTIP
	menuButton->setToolTip(tr("Search with %1").arg(action->text()));
#endif

	setUpdatesEnabled(true);
}

#include "moc_dialog.cpp"
