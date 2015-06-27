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

#include "lineedit.h"
#include "lineedit_p.h"

#include <QApplication>
#ifndef QT_NO_COMBOBOX
#include <QComboBox>
#endif
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#define verticalMargin 1
#define horizontalMargin 2

LineEditPrivate::LineEditPrivate() : q_ptr(0),
	leftWidget(0), rightWidget(0),
#if QT_VERSION < 0x040500
	leftTextMargin(0), topTextMargin(0), rightTextMargin(0), bottomTextMargin(0),
#endif
	spacing(0)
{
}

LineEditPrivate::~LineEditPrivate()
{
}

void LineEditPrivate::init()
{
#if QT_VERSION < 0x040500
	Q_Q(LineEdit);

	q->setStyle(new LineEditProxyStyle(q));
#endif
}

void LineEditPrivate::layoutWidgets()
{
	if(!leftWidget && !rightWidget)
		return;

	Q_Q(const LineEdit);

	QStyleOptionFrameV2 opt;
	q->initStyleOption(&opt);

	QRect r = adjustedContentsRect();

	int left, top, right, bottom;
	q->getTextMargins(&left, &top, &right, &bottom);
	r.adjust(-left, -top, right, bottom);

	QPoint pos = r.topLeft();
	if(leftWidget && leftWidget->isVisible())
	{
		const QSize newWidgetSize(leftWidget->sizeHint().width(), r.height());
		if(newWidgetSize != leftWidget->size())
			leftWidget->resize(newWidgetSize);
		leftWidget->move(pos);
	}
	if(rightWidget && rightWidget->isVisible())
	{
		const QSize newWidgetSize(rightWidget->sizeHint().width(), r.height());
		if(newWidgetSize != rightWidget->size())
			rightWidget->resize(newWidgetSize);
		pos.rx() += r.width() - rightWidget->width();
		rightWidget->move(pos);
	}
}

void LineEditPrivate::updateTextMargins()
{
	Q_Q(LineEdit);

	int left, top, right, bottom;
	q->getTextMargins(&left, &top, &right, &bottom);
	if(leftWidget && leftWidget->isVisible() && leftWidget->width() > 0)
		left = leftWidget->width() + spacing;
	if(rightWidget && rightWidget->isVisible() && rightWidget->width() > 0)
		right = rightWidget->width() + spacing;
	q->setTextMargins(left, top, right, bottom);
	layoutWidgets();
}

QRect LineEditPrivate::adjustedContentsRect() const
{
	Q_Q(const LineEdit);

	QStyleOptionFrameV2 opt;
	q->initStyleOption(&opt);

	QRect r = q->style()->subElementRect(QStyle::SE_LineEditContents, &opt, q);
#if QT_VERSION >= 0x040500
	int left, top, right, bottom;
	q->getTextMargins(&left, &top, &right, &bottom);
	r.adjust(left, top, -right, -bottom);
#else
	// was adjusted in LineEditProxyStyle::subElementRect(...)
#endif

	return r;
}


LineEdit::LineEdit(QWidget* parent) : QLineEdit(parent),
	d_ptr(new LineEditPrivate)
{
	Q_D(LineEdit);

	d_ptr->q_ptr = this;
	d->init();
}

LineEdit::LineEdit(const QString& text, QWidget* parent) : QLineEdit(text, parent),
	d_ptr(new LineEditPrivate)
{
	Q_D(LineEdit);

	d_ptr->q_ptr = this;
	d->init();
}

LineEdit::LineEdit(LineEditPrivate& dd, QWidget* parent) : QLineEdit(parent),
	d_ptr(&dd)
{
	Q_D(LineEdit);

	d_ptr->q_ptr = this;
	d->init();
}

LineEdit::~LineEdit()
{
	delete d_ptr;
}

#if QT_VERSION < 0x040500
QSize LineEdit::sizeHint() const
{
	Q_D(const LineEdit);

	ensurePolished();

	QStyleOptionFrameV2 opt;
	initStyleOption(&opt);

	int left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);

	QFontMetrics fm(font());
	int h = qMax(fm.lineSpacing(), 14) + 2 * verticalMargin
		+ d->topTextMargin + d->bottomTextMargin
		+ top + bottom;
	int w = fm.width(QLatin1Char('x')) * 17 + 2 * horizontalMargin
		+ d->leftTextMargin + d->rightTextMargin
		+ left + right;
	QSize sz = QSize(w, h).expandedTo(QApplication::globalStrut());

	return style()->sizeFromContents(QStyle::CT_LineEdit, &opt, sz, this);
}

void LineEdit::getTextMargins(int* left, int* top, int* right, int* bottom) const
{
	Q_D(const LineEdit);

	if(left)
		*left = d->leftTextMargin;
	if(top)
		*top = d->topTextMargin;
	if(right)
		*right = d->rightTextMargin;
	if(bottom)
		*bottom = d->bottomTextMargin;
}

void LineEdit::setTextMargins(int left, int top, int right, int bottom)
{
	Q_D(LineEdit);

	d->leftTextMargin = left;
	d->topTextMargin = top;
	d->rightTextMargin = right;
	d->bottomTextMargin = bottom;
	updateGeometry();
	update();
}
#endif // QT_VERSION

int LineEdit::spacing() const
{
	Q_D(const LineEdit);

	return d->spacing;
}

void LineEdit::setSpacing(int spacing)
{
	Q_D(LineEdit);

	if(d->spacing == spacing)
		return;

	d->spacing = spacing;
	d->updateTextMargins();
}

QString LineEdit::hintText() const
{
	Q_D(const LineEdit);

	return d->hintText;
}

void LineEdit::setHintText(const QString& text)
{
	Q_D(LineEdit);

	if(d->hintText == text)
		return;

	d->hintText = text;
	update();
}

QWidget* LineEdit::widget(WidgetPosition position) const
{
	Q_D(const LineEdit);

	return (position == LeftSide ? d->leftWidget : d->rightWidget);
}

void LineEdit::setWidget(WidgetPosition position, QWidget* widget)
{
	Q_D(LineEdit);

	QWidget** wp = (position == LeftSide ? &d->leftWidget : &d->rightWidget);
	if(*wp == widget)
		return;

	QWidget* w = *wp;
	bool bWasVisible = w ? w->isVisible() : true;

	if(w)
	{
		w->removeEventFilter(this);
		w->hide();
		w->deleteLater();
	}

	*wp = widget;

	if(widget)
	{
		widget->setParent(this);
		widget->lower();
		widget->setVisible(bWasVisible);
		widget->installEventFilter(this);
	}

	d->updateTextMargins();
}

bool LineEdit::eventFilter(QObject* watched, QEvent* event)
{
	Q_D(LineEdit);

	if(watched == d->leftWidget || watched == d->rightWidget)
	{
		switch(event->type())
		{
			case QEvent::Resize:
			case QEvent::Hide:
			case QEvent::Show:
				d->updateTextMargins();
				break;
			default:
				break;
		}
	}

	return QLineEdit::eventFilter(watched, event);
}

void LineEdit::paintEvent(QPaintEvent* event)
{
	QLineEdit::paintEvent(event);

	Q_D(const LineEdit);

	if(text().isEmpty() && !d->hintText.isEmpty() && !hasFocus())
	{
#ifndef QT_NO_COMBOBOX
		if(const QComboBox* combo = qobject_cast<const QComboBox*>(parent()))
		{
			const QList<QObject*>& children = combo->children();
			for(int i = 0, n = children.size(); i < n; ++i)
			{
				const QWidget* child = qobject_cast<const QWidget*>(children[i]);
				if(child && child->hasFocus())
					return;
			}
		}
#endif

		const QRect r = d->adjustedContentsRect();

		const QFontMetrics fm = fontMetrics();
		const Qt::Alignment va = QStyle::visualAlignment(layoutDirection(), QFlag(alignment()));
		int vscroll = r.y();
		switch(va & Qt::AlignVertical_Mask)
		{
			case Qt::AlignBottom:
				vscroll += r.height() - fm.height() - verticalMargin;
				break;
			case Qt::AlignTop:
				vscroll += verticalMargin;
				break;
			default:
				// center
				vscroll += (r.height() - fm.height() + 1) / 2;
				break;
		}
		const QRect lineRect(r.x() + horizontalMargin, vscroll, r.width() - horizontalMargin * 2, fm.height());

		QPainter painter(this);
		painter.setClipRect(r);
		painter.setPen(palette().brush(QPalette::Disabled, QPalette::Text).color());
		painter.drawText(lineRect, va, d->hintText);
	}
}

void LineEdit::resizeEvent(QResizeEvent* event)
{
	QLineEdit::resizeEvent(event);

	Q_D(LineEdit);

	d->layoutWidgets();
}

#if QT_VERSION < 0x040500
void LineEdit::changeEvent(QEvent* event)
{
	QLineEdit::changeEvent(event);

	if(event->type() == QEvent::StyleChange)
	{
		if(!qobject_cast<LineEditProxyStyle*>(style()))
			setStyle(new LineEditProxyStyle(this));
	}
}
#endif // QT_VERSION

#include "moc_lineedit.cpp"
