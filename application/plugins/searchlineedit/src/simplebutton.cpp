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

#include "simplebutton.h"

#include <QtGui/QActionEvent>
#include <QApplication>
#include <QtGui/QPainter>
#include <QStyle>
#include <QStyleOption>

#ifndef QT_NO_MENU
#include <QtCore/QPointer>

#include <QAction>
#include <QtGui/QBitmap>
#include <QDesktopWidget>
#include <QtGui/QGradient>
#include <QMenu>

#if QT_VERSION < 0x040400
#if defined(QT_NO_THREAD)
#define Q_GLOBAL_STATIC_WITH_INITIALIZER(TYPE, NAME, INITIALIZER) \
	static TYPE *NAME()                                           \
	{                                                             \
		static TYPE this_##NAME;                                  \
		static QGlobalStatic<TYPE > global_##NAME(0);             \
		if (!global_##NAME.pointer) {                             \
			TYPE *x = global_##NAME.pointer = &this_##NAME;       \
			INITIALIZER;                                          \
		}                                                         \
		return global_##NAME.pointer;                             \
	}
#else // QT_NO_THREAD
#define Q_GLOBAL_STATIC_WITH_INITIALIZER(TYPE, NAME, INITIALIZER)       \
	Q_GLOBAL_STATIC_INIT(TYPE, NAME);                                   \
	static TYPE *NAME()                                                 \
	{                                                                   \
		if (!this_##NAME.pointer && !this_##NAME.destroyed) {           \
			TYPE *x = new TYPE;                                         \
			INITIALIZER;                                                \
			if (!q_atomic_test_and_set_ptr(&this_##NAME.pointer, 0, x)) \
				delete x;                                               \
			else                                                        \
				static QGlobalStaticDeleter<TYPE > cleanup(this_##NAME); \
		}                                                               \
		return this_##NAME.pointer;                                     \
	}
#endif // QT_NO_THREAD
#endif // QT_VERSION

static const int arrow_sz = 7;
static const unsigned char arrow_bits[] = { 0x00, 0x00, 0x3e, 0x1c, 0x08, 0x00, 0x00 };
Q_GLOBAL_STATIC_WITH_ARGS(QBitmap, bmpArrow, (QBitmap::fromData(QSize(arrow_sz, arrow_sz), arrow_bits))) //Q_GLOBAL_STATIC_WITH_ARGS_INITIALIZER
#endif

class SimpleButtonPrivate
{
	Q_DECLARE_PUBLIC(SimpleButton)

public:
	SimpleButtonPrivate() : q_ptr(0)
	{}

	void init();
#ifndef QT_NO_MENU
	bool hasMenu() const;
	void _q_updateButtonDown();

	QPointer<QAction> menuAction; //the menu set by the user (setMenu)
#endif
	bool menuButtonDown;

	SimpleButton* q_ptr;
};

void SimpleButtonPrivate::init()
{
	Q_Q(SimpleButton);

	menuButtonDown = false;

#ifndef QT_NO_CURSOR
	q->setCursor(Qt::ArrowCursor);
#endif
	q->setFocusPolicy(Qt::NoFocus);
	q->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, QSizePolicy::ToolButton));
}

#ifndef QT_NO_MENU
bool SimpleButtonPrivate::hasMenu() const
{
	return menuAction && menuAction->menu();
}

void SimpleButtonPrivate::_q_updateButtonDown()
{
	Q_Q(SimpleButton);

	menuButtonDown = false;
	if(q->isDown())
		q->setDown(false);
	else
		q->repaint();
}
#endif // QT_NO_MENU


SimpleButton::SimpleButton(QWidget* parent) : QAbstractButton(parent),
	d_ptr(new SimpleButtonPrivate)
{
	Q_D(SimpleButton);

	d_ptr->q_ptr = this;
	d->init();
}

SimpleButton::SimpleButton(const QIcon& icon, QWidget* parent) : QAbstractButton(parent),
	d_ptr(new SimpleButtonPrivate)
{
	Q_D(SimpleButton);

	d_ptr->q_ptr = this;
	d->init();

	setIcon(icon);
}

SimpleButton::~SimpleButton()
{
	delete d_ptr;
}

QSize SimpleButton::minimumSizeHint() const
{
	return sizeHint();
}

QSize SimpleButton::sizeHint() const
{
	Q_D(const SimpleButton);

	ensurePolished();

	QSize sz = iconSize();
#ifndef QT_NO_MENU
	if(d->hasMenu())
	{
		QStyleOption opt;
		opt.initFrom(this);
		opt.rect.setSize(sz); // PM_MenuButtonIndicator depends on the height
		sz.rwidth() += style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);
	}
#endif
	sz = sz.expandedTo(QApplication::globalStrut());

	return sz;
}

#ifndef QT_NO_MENU
QMenu* SimpleButton::menu() const
{
	Q_D(const SimpleButton);

	return (d->menuAction ? d->menuAction->menu() : 0);
}

void SimpleButton::setMenu(QMenu* menu)
{
	Q_D(SimpleButton);

	if(d->menuAction)
		removeAction(d->menuAction);

	if(menu)
	{
		d->menuAction = menu->menuAction();
		addAction(d->menuAction);
	}
	update();
}

void SimpleButton::showMenu()
{
	Q_D(SimpleButton);

	d->menuButtonDown = d->hasMenu();
	if(d->menuButtonDown)
	{
		repaint();

		const bool repeat = autoRepeat();
		setAutoRepeat(false);

		QPointer<QMenu> actualMenu;
		bool mustDeleteActualMenu = false;
		if(d->menuAction)
		{
			actualMenu = d->menuAction->menu();
		}
		else
		{
			actualMenu = new QMenu(this);
			mustDeleteActualMenu = true;
			const QList<QAction*>& acts = this->actions();
			for(int i = 0; i < acts.size(); ++i)
				actualMenu->addAction(acts[i]);
		}

		QPoint p;
		QRect r = rect();
		QRect screen = QApplication::desktop()->availableGeometry(this);
		QSize sh = actualMenu->sizeHint();
		if(isLeftToRight())
		{
			if(mapToGlobal(QPoint(0, r.bottom())).y() + sh.height() <= screen.height())
				p = mapToGlobal(r.bottomLeft());
			else
				p = mapToGlobal(r.topLeft() - QPoint(0, sh.height()));
		}
		else
		{
			if(mapToGlobal(QPoint(0, r.bottom())).y() + sh.height() <= screen.height())
				p = mapToGlobal(r.bottomRight());
			else
				p = mapToGlobal(r.topRight() - QPoint(0, sh.height()));
			p.rx() -= sh.width();
		}
		p.rx() = qMax(screen.left(), qMin(p.x(), screen.right() - sh.width()));
		p.ry() += 1;

		QPointer<SimpleButton> that = this;
		//actualMenu->setNoReplayFor(this);
		QObject::connect(actualMenu, SIGNAL(aboutToHide()), this, SLOT(_q_updateButtonDown()));
		//actualMenu->d_func()->causedPopup.widget = this;
		//actualMenu->d_func()->causedPopup.action = defaultAction;
		QList<QAction*> actionsCopy = actions(); // the list of action may be modified in slots
		actualMenu->exec(p);
		QObject::disconnect(actualMenu, SIGNAL(aboutToHide()), this, SLOT(_q_updateButtonDown()));
		if(mustDeleteActualMenu)
			delete actualMenu;

		if(!that)
			return;

		actionsCopy.clear();

		setAutoRepeat(repeat);
	}
}
#endif // QT_NO_MENU

bool SimpleButton::event(QEvent* event)
{
	switch(event->type())
	{
		case QEvent::HoverEnter:
		case QEvent::HoverLeave:
			update();
			break;
		default:
			break;
	}

	return QAbstractButton::event(event);
}

void SimpleButton::actionEvent(QActionEvent* event)
{
#ifndef QT_NO_MENU
	Q_D(SimpleButton);

	switch(event->type())
	{
		case QEvent::ActionAdded:
			update();
			break;
		case QEvent::ActionRemoved:
			update();
			if(event->action() == d->menuAction)
				d->menuAction = 0;
			break;
		case QEvent::ActionChanged:
		default:
			break;
	}
#endif // QT_NO_MENU

	QAbstractButton::actionEvent(event);
}

void SimpleButton::mousePressEvent(QMouseEvent* event)
{
#ifndef QT_NO_MENU
	Q_D(const SimpleButton);

	if(d->hasMenu() && event->button() == Qt::LeftButton)
	{
		showMenu();
		return;
	}
#endif

	QAbstractButton::mousePressEvent(event);
}

void SimpleButton::paintEvent(QPaintEvent*)
{
	Q_D(const SimpleButton);

	QPainter painter(this);

	QStyleOption opt;
	opt.initFrom(this);
	opt.state |= QStyle::State_AutoRaise;
	if(isEnabled() && underMouse() && !isChecked() && !isDown() && !d->menuButtonDown)
		opt.state |= QStyle::State_Raised;
	if(isDown() || d->menuButtonDown)
		opt.state |= QStyle::State_Sunken;
	if(isChecked())
		opt.state |= QStyle::State_On;
	opt.state |= QStyle::State_Selected;

#ifndef QT_NO_MENU
	int mbi = 0;
	bool hasMenu = d_func()->hasMenu();
	if(hasMenu)
	{
		mbi = style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);
		if(isLeftToRight())
			opt.rect.adjust(0, 0, -mbi, 0);
		else
			opt.rect.adjust(mbi, 0, 0, 0);
	}
#endif

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
#ifndef QT_NO_MENU
	if(hasMenu)
	{
		if(isLeftToRight())
			opt.rect.adjust(opt.rect.width(), 0, mbi, 0);
		else
			opt.rect.adjust(-mbi, 0, -opt.rect.width(), 0);
		if(opt.rect.width() <= arrow_sz || opt.rect.height() <= arrow_sz)
			return;

		painter.setClipRect(opt.rect);

		if(opt.state & QStyle::State_Sunken)
		{
			QRectF rf(opt.rect);
			QPointF center = rf.center();
			qreal radius = qMin(rf.height(), rf.width()) * 0.4;

			QRectF ellipse(center.x() - radius, center.y() - radius, radius * 2, radius * 2);

			QRadialGradient gradient(center, radius);
			QColor c = opt.palette.alternateBase().color();
			qreal ca = c.alphaF();
			gradient.setColorAt(0.0, c);
			c.setAlphaF(ca * 0.85);
			gradient.setColorAt(0.5, c);
			c.setAlphaF(ca * 0.5);
			gradient.setColorAt(0.85, c);
			c = opt.palette.background().color();
			c.setAlphaF(0.2);
			gradient.setColorAt(1.0, c);

			painter.save();
			painter.setBrush(gradient);
			painter.setPen(Qt::NoPen);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.drawEllipse(ellipse);
			painter.restore();
		}

		int xOffset = opt.rect.x() + (opt.rect.width() - arrow_sz) / 2;
		int yOffset = opt.rect.y() + (opt.rect.height() - arrow_sz) / 2;
		if(!(opt.state & QStyle::State_Enabled))
		{
			xOffset += 1;
			yOffset += 1;
			painter.setBrush(opt.palette.light().color());
			painter.setPen(opt.palette.light().color());
			painter.drawPixmap(xOffset, yOffset, *bmpArrow());
			xOffset -= 1;
			yOffset -= 1;
			painter.setBrush(opt.palette.mid().color());
			painter.setPen(opt.palette.mid().color());
		}
		painter.drawPixmap(xOffset, yOffset, *bmpArrow());
	}
#endif
}

#include "moc_simplebutton.cpp"
