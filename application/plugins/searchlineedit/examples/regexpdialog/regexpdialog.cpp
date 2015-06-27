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

#include "regexpdialog.h"

#include <QtGui>

#include "searchlineedit.h"

RegExpDialog::RegExpDialog(QWidget* parent) : QDialog(parent)
{
	menu = createMenu();

	patternLineEdit = new SearchLineEdit(this);
	patternLineEdit->setHintText(tr("(empty)"));
	patternLineEdit->setMenu(menu);
	patternLineEdit->setClearButtonShown(true);

	patternComboBox = new QComboBox(this);
	patternComboBox->setEditable(true);
	patternComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	patternComboBox->setLineEdit(patternLineEdit);

	patternLabel = new QLabel(tr("&Pattern:"), this);
	patternLabel->setBuddy(patternComboBox);

	escapedPatternLineEdit = new QLineEdit(this);
	escapedPatternLineEdit->setReadOnly(true);
	QPalette palette = escapedPatternLineEdit->palette();
	palette.setBrush(QPalette::Base, palette.brush(QPalette::Disabled, QPalette::Base));
	escapedPatternLineEdit->setPalette(palette);

	escapedPatternLabel = new QLabel(tr("&Escaped Pattern:"), this);
	escapedPatternLabel->setBuddy(escapedPatternLineEdit);

	syntaxComboBox = new QComboBox(this);
	syntaxComboBox->addItem(tr("Regular expression v1"), QRegExp::RegExp);
	syntaxComboBox->addItem(tr("Regular expression v2"), QRegExp::RegExp2);
	syntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
	syntaxComboBox->addItem(tr("Fixed string"), QRegExp::FixedString);

	syntaxLabel = new QLabel(tr("&Pattern Syntax:"), this);
	syntaxLabel->setBuddy(syntaxComboBox);

	textEdit = new SearchLineEdit(this);
	textEdit->setHintText(tr("(empty)"));
	textEdit->setClearButtonShown(true);

	textLabel = new QLabel(tr("&Text:"), this);
	textLabel->setBuddy(textEdit);

	caseSensitiveCheckBox = new QCheckBox(tr("Case &Sensitive"), this);
	caseSensitiveCheckBox->setChecked(true);
	minimalCheckBox = new QCheckBox(tr("&Minimal"), this);

	indexLabel = new QLabel(tr("Index of Match:"), this);
	indexEdit = new QLineEdit(this);
	indexEdit->setReadOnly(true);

	matchedLengthLabel = new QLabel(tr("Matched Length:"), this);
	matchedLengthEdit = new QLineEdit(this);
	matchedLengthEdit->setReadOnly(true);

	for(int i = 0; i < MaxCaptures; ++i)
	{
		captureLabels[i] = new QLabel(tr("Capture %1:").arg(i), this);
		captureEdits[i] = new QLineEdit(this);
		captureEdits[i]->setReadOnly(true);
	}
	captureLabels[0]->setText(tr("Match:"));

	QHBoxLayout* checkBoxLayout = new QHBoxLayout;
	checkBoxLayout->addWidget(caseSensitiveCheckBox);
	checkBoxLayout->addWidget(minimalCheckBox);
	checkBoxLayout->addStretch(1);

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addWidget(patternLabel, 0, 0);
	mainLayout->addWidget(patternComboBox, 0, 1);
	mainLayout->addWidget(escapedPatternLabel, 1, 0);
	mainLayout->addWidget(escapedPatternLineEdit, 1, 1);
	mainLayout->addWidget(syntaxLabel, 2, 0);
	mainLayout->addWidget(syntaxComboBox, 2, 1);
	mainLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
	mainLayout->addWidget(textLabel, 4, 0);
	mainLayout->addWidget(textEdit, 4, 1);
	mainLayout->addWidget(indexLabel, 5, 0);
	mainLayout->addWidget(indexEdit, 5, 1);
	mainLayout->addWidget(matchedLengthLabel, 6, 0);
	mainLayout->addWidget(matchedLengthEdit, 6, 1);

	for(int j = 0; j < MaxCaptures; ++j)
	{
		mainLayout->addWidget(captureLabels[j], 7 + j, 0);
		mainLayout->addWidget(captureEdits[j], 7 + j, 1);
	}
	setLayout(mainLayout);

	connect(patternComboBox, SIGNAL(editTextChanged(const QString&)),
			this, SLOT(refresh()));
	connect(textEdit, SIGNAL(textChanged(const QString&)),
			this, SLOT(refresh()));
	connect(caseSensitiveCheckBox, SIGNAL(toggled(bool)),
			this, SLOT(refresh()));
	connect(minimalCheckBox, SIGNAL(toggled(bool)), this, SLOT(refresh()));
	connect(syntaxComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(refresh()));

	patternComboBox->addItem(QLatin1String("[A-Za-z_]+([A-Za-z_0-9]*)"));
	textEdit->setText(QLatin1String("(10 + delta4) * 32"));

	setWindowTitle(tr("RegExp Example with SearchLineEdits"));
	setWindowIcon(QIcon(QLatin1String(":/icons/clear_search.png")));
	setMinimumWidth(350);
	setFixedHeight(sizeHint().height());

	refresh();
}

QMenu* RegExpDialog::createMenu()
{
	QActionGroup* syntaxActionGroup = new QActionGroup(this);
	syntaxActionGroup->setExclusive(true);
	{
		{
			QAction* action = syntaxActionGroup->addAction(tr("Regular expression v1"));
			action->setCheckable(true);
			action->setChecked(true);
			action->setData(0);
		}
		{
			QAction* action = syntaxActionGroup->addAction(tr("Regular expression v2"));
			action->setCheckable(true);
			action->setData(1);
		}
		{
			QAction* action = syntaxActionGroup->addAction(tr("Wildcard"));
			action->setCheckable(true);
			action->setData(2);
		}
		{
			QAction* action = syntaxActionGroup->addAction(tr("Fixed string"));
			action->setCheckable(true);
			action->setData(3);
		}
	}
	connect(syntaxActionGroup, SIGNAL(triggered(QAction*)),
			this, SLOT(_slot_syntaxAction_triggered(QAction*)));

	QAction* csAction = new QAction(tr("Case Sensitive"), this);
	csAction->setCheckable(true);
	csAction->setChecked(true);
	connect(csAction, SIGNAL(triggered()),
			this, SLOT(_slot_csAction_triggered()));

	QAction* minimalAction = new QAction(tr("Minimal"), this);
	minimalAction->setCheckable(true);
	connect(minimalAction, SIGNAL(triggered()),
			this, SLOT(_slot_minimalAction_triggered()));

	QMenu* resMenu = new QMenu(this);
	resMenu->addActions(syntaxActionGroup->actions());
	(void)resMenu->addSeparator();
	(void)resMenu->addAction(csAction);
	(void)resMenu->addSeparator();
	(void)resMenu->addAction(minimalAction);

	return resMenu;
}

void RegExpDialog::refresh()
{
	setUpdatesEnabled(false);

	QString pattern = patternComboBox->currentText();
	QString text = textEdit->text();

	QString escaped = pattern;
	escaped.replace("\\", "\\\\");
	escaped.replace("\"", "\\\"");
	escaped.prepend("\"");
	escaped.append("\"");
	escapedPatternLineEdit->setText(escaped);

	QRegExp rx(pattern);
	Qt::CaseSensitivity cs = Qt::CaseInsensitive;
	if(caseSensitiveCheckBox->isChecked())
		cs = Qt::CaseSensitive;
	rx.setCaseSensitivity(cs);
	rx.setMinimal(minimalCheckBox->isChecked());
	int iSyntax = syntaxComboBox->itemData(syntaxComboBox->currentIndex()).toInt();
	rx.setPatternSyntax(static_cast<QRegExp::PatternSyntax>(iSyntax));

	QPalette palette = patternComboBox->palette();
	QColor c(Qt::red);
	if(rx.isValid())
		c = textEdit->palette().color(QPalette::Active, QPalette::Text);
	palette.setColor(QPalette::Active, QPalette::Text, c);
	patternComboBox->setPalette(palette);

	indexEdit->setText(QString::number(rx.indexIn(text)));
	matchedLengthEdit->setText(QString::number(rx.matchedLength()));
	for(int i = 0; i < MaxCaptures; ++i)
	{
		captureLabels[i]->setEnabled(i <= rx.numCaptures());
		captureEdits[i]->setEnabled(i <= rx.numCaptures());
		captureEdits[i]->setText(rx.cap(i));
	}

	setUpdatesEnabled(true);
}

void RegExpDialog::_slot_syntaxAction_triggered(QAction* action)
{
	if(action)
		syntaxComboBox->setCurrentIndex(action->data().toInt());
}

void RegExpDialog::_slot_csAction_triggered()
{
	const QAction* action = qobject_cast<const QAction*>(sender());
	if(action)
		caseSensitiveCheckBox->setChecked(action->isChecked());
}

void RegExpDialog::_slot_minimalAction_triggered()
{
	const QAction* action = qobject_cast<const QAction*>(sender());
	if(action)
		minimalCheckBox->setChecked(action->isChecked());
}

#include "moc_regexpdialog.cpp"
