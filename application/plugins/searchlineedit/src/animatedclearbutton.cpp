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

#include "animatedclearbutton.h"

#include <QtCore/QString>
#include <QtCore/QTimeLine>

#include <QApplication>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

AnimatedClearButton::AnimatedClearButton(QWidget* parent) : QAbstractButton(parent),
	m_timeline(0),
	m_animationsEnabled(false)
{
#ifndef QT_NO_CURSOR
	setCursor(Qt::ArrowCursor);
#endif
	setFocusPolicy(Qt::NoFocus);
	setIcon(QIcon(QLatin1String(":/icons/clear.png")));
#ifndef QT_NO_TOOLTIP
	setToolTip(QApplication::translate("SearchLineEdit", "Clear text"));
#endif
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::ToolButton));

	setAnimationsEnabled(QApplication::isEffectEnabled(Qt::UI_FadeMenu));
}

QSize AnimatedClearButton::sizeHint() const
{
	ensurePolished();

	QSize sz = iconSize();
	sz = sz.expandedTo(QApplication::globalStrut());

	return sz;
}

bool AnimatedClearButton::isAnimationsEnabled() const
{
	return m_animationsEnabled;
}

void AnimatedClearButton::setAnimationsEnabled(bool animationsEnabled)
{
	if(m_animationsEnabled == animationsEnabled)
		return;

	m_animationsEnabled = animationsEnabled;
	if(!animationsEnabled)
	{
		delete m_timeline;
		m_timeline = 0;
	}
	else
	{
		m_timeline = new QTimeLine(250, this);
		m_timeline->setFrameRange(0, 255);

		connect(m_timeline, SIGNAL(finished()), this, SLOT(timelineFinished()));
		connect(m_timeline, SIGNAL(frameChanged(int)), this, SLOT(update()));
	}
}

void AnimatedClearButton::animateVisible(bool visible)
{
	if(!m_animationsEnabled)
	{
		setVisible(visible);
		return;
	}

	if(visible == isVisible() && m_timeline->state() != QTimeLine::Running)
		return;

	setVisible(true);

	if(m_timeline->state() == QTimeLine::Running)
		m_timeline->setPaused(true);
	m_timeline->setCurveShape(visible ? QTimeLine::EaseInCurve : QTimeLine::EaseOutCurve);
	m_timeline->setDirection(visible ? QTimeLine::Forward : QTimeLine::Backward);
	m_timeline->setDuration(visible ? 150 : 250);
	if(m_timeline->state() == QTimeLine::Paused)
		m_timeline->resume();
	else
		m_timeline->start();
}

void AnimatedClearButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.save(); // workaround internal globalOpacity bug fixed in 4.5.0
	if(m_animationsEnabled && m_timeline->state() == QTimeLine::Running)
		painter.setOpacity(m_timeline->currentValue());

	QStyleOption opt;
	opt.initFrom(this);
	opt.state |= QStyle::State_AutoRaise;
	if(isEnabled() && underMouse() && !isChecked() && !isDown())
		opt.state |= QStyle::State_Raised;
	if(isDown())
		opt.state |= QStyle::State_Sunken;
	if(isChecked())
		opt.state |= QStyle::State_On;
	opt.state |= QStyle::State_Selected;

	QIcon::Mode mode = opt.state & QStyle::State_Enabled ?
				(opt.state & QStyle::State_Raised ? QIcon::Active : QIcon::Normal)
				: QIcon::Disabled;
	if(!(opt.state & QStyle::State_Raised)
		&& !(opt.state & QStyle::State_Sunken)
		&& !(opt.state & QStyle::State_Selected))
		mode = QIcon::Disabled;
	QIcon::State state = opt.state & QStyle::State_On ? QIcon::On : QIcon::Off;
	QPixmap pixmap = icon().pixmap(opt.rect.size().boundedTo(iconSize()), mode, state);
	style()->drawItemPixmap(&painter, opt.rect, Qt::AlignCenter, pixmap);
	painter.restore();
}

void AnimatedClearButton::timelineFinished()
{
	if(m_animationsEnabled)
	{
		if(m_timeline->direction() == QTimeLine::Forward)
			update();
		else
			hide();
	}
}

#include "moc_animatedclearbutton.cpp"
