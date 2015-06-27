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

#include "searchlineedit.h"

#include <QtCore/QMetaObject>

#ifndef QT_NO_MENU
#include <QMenu>
#endif

#include "animatedclearbutton.h"
#include "lineedit_p.h"
#include "simplebutton.h"

class SearchLineEditPrivate : public LineEditPrivate
{
	Q_DECLARE_PUBLIC(SearchLineEdit)

public:
	SearchLineEditPrivate() : LineEditPrivate(),
		menuButton(0), clearButton(0),
		showClearButton(false),
		isReadOnly(false)
	{}

	void _q_updateClearButton();
	void _q_clearButtonClicked();

	SimpleButton* menuButton;
	AnimatedClearButton* clearButton;

	bool showClearButton;
	bool isReadOnly;
};

void SearchLineEditPrivate::_q_updateClearButton()
{
	Q_Q(const SearchLineEdit);

	if(clearButton)
	{
		bool bNeedVisible = !q->text().isEmpty() && !q->isReadOnly();
		if(bNeedVisible)
		{
			const QRect r = adjustedContentsRect();
			const int buttonWidth = clearButton->sizeHint().width();
			const QFontMetrics fm = q->fontMetrics();
			const int fmwx = fm.width(QLatin1Char('x'));
			// make sure we have enough room for the clear button
			// no sense in showing it if we can't also see a few characters as well
			bNeedVisible = r.width() > fmwx * 4 + buttonWidth;
		}
		if(q->sender())
			clearButton->animateVisible(bNeedVisible);
		else
			clearButton->setVisible(bNeedVisible);
	}
}

void SearchLineEditPrivate::_q_clearButtonClicked()
{
	Q_Q(SearchLineEdit);

	q->clear();
}


SearchLineEdit::SearchLineEdit(QWidget* parent) :
	LineEdit(*new SearchLineEditPrivate, parent)
{
}

SearchLineEdit::SearchLineEdit(const QString& text, QWidget* parent) :
	LineEdit(*new SearchLineEditPrivate, parent)
{
	setText(text);
}

SearchLineEdit::~SearchLineEdit()
{
}

bool SearchLineEdit::isClearButtonShown() const
{
	Q_D(const SearchLineEdit);

	return d->showClearButton;
}

void SearchLineEdit::setClearButtonShown(bool showClearButton)
{
	Q_D(SearchLineEdit);

	if(d->showClearButton == showClearButton)
		return;

	d->showClearButton = showClearButton;
	if(!showClearButton)
	{
		disconnect(this, SIGNAL(textChanged(const QString&)),
				this, SLOT(_q_updateClearButton()));

		setWidget(RightSide, 0);
		d->clearButton = 0;
	}
	else
	{
		d->clearButton = new AnimatedClearButton(this);
		setWidget(RightSide, d->clearButton);
		d->_q_updateClearButton();

		connect(this, SIGNAL(textChanged(const QString&)),
				this, SLOT(_q_updateClearButton()));
		connect(d->clearButton, SIGNAL(clicked()),
				this, SLOT(_q_clearButtonClicked()));
	}
}

#ifndef QT_NO_MENU
QMenu* SearchLineEdit::menu() const
{
	Q_D(const SearchLineEdit);

	return d->menuButton ? d->menuButton->menu() : 0;
}

void SearchLineEdit::setMenu(QMenu* menu)
{
	Q_D(SearchLineEdit);

	if(!menu)
	{
		setWidget(LeftSide, 0);
		d->menuButton = 0;
	}
	else
	{
		if(!d->menuButton)
		{
			d->menuButton = new SimpleButton(this);
			d->menuButton->setIcon(QIcon(QLatin1String(":/icons/magnify_glass.png")));
			setWidget(LeftSide, d->menuButton);
		}
		d->menuButton->setMenu(menu);
	}
}
#endif // QT_NO_MENU

void SearchLineEdit::paintEvent(QPaintEvent* event)
{
	LineEdit::paintEvent(event);

	Q_D(SearchLineEdit);

	if(d->isReadOnly != isReadOnly())
	{
		d->isReadOnly = isReadOnly();
		d->_q_updateClearButton();
	}
}

void SearchLineEdit::resizeEvent(QResizeEvent* event)
{
	LineEdit::resizeEvent(event);

	Q_D(SearchLineEdit);

	d->_q_updateClearButton();
}

#include "moc_searchlineedit.cpp"
