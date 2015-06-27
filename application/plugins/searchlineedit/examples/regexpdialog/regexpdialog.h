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

#ifndef REGEXPDIALOG_H
#define REGEXPDIALOG_H

#include <QtGui/QDialog>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QMenu;

class SearchLineEdit;

class RegExpDialog : public QDialog
{
	Q_OBJECT

public:
	RegExpDialog(QWidget* parent = 0);

protected:
	QMenu* createMenu();

private Q_SLOTS:
	void refresh();

	void _slot_syntaxAction_triggered(QAction* action);
	void _slot_csAction_triggered();
	void _slot_minimalAction_triggered();

private:
	QMenu* menu;

	QLabel* patternLabel;
	QLabel* escapedPatternLabel;
	QLabel* syntaxLabel;
	QLabel* textLabel;
	SearchLineEdit* patternLineEdit;
	QComboBox* patternComboBox;
	QLineEdit* escapedPatternLineEdit;
	SearchLineEdit* textEdit;
	QCheckBox* caseSensitiveCheckBox;
	QCheckBox* minimalCheckBox;
	QComboBox* syntaxComboBox;

	QLabel* indexLabel;
	QLabel* matchedLengthLabel;
	QLineEdit* indexEdit;
	QLineEdit* matchedLengthEdit;

	enum { MaxCaptures = 6 };
	QLabel* captureLabels[MaxCaptures];
	QLineEdit* captureEdits[MaxCaptures];
};

#endif // REGEXPDIALOG_H
