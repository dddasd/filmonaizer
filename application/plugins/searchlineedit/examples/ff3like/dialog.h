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

#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

class QMenu;

class LineEdit;
class SimpleButton;

class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(QWidget* parent = 0);

protected:
	QMenu* createMenu();

private Q_SLOTS:
	void _slot_action_triggered(QAction* action);

private:
	QMenu* menu;

	LineEdit* lineEdit;
	SimpleButton* menuButton;
	SimpleButton* searchButton;
};

#endif // DIALOG_H
