#include <qapplication.h>
#include <qevent.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>
#include <qwt_scale_map.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <iostream>
#include "canvaspicker.h"
#include "specplot.h"
#include "specbox.h"


CanvasPicker::CanvasPicker(QwtPlot *plot):
    QObject(plot),
    d_selectedCurve(NULL),
    d_selectedPoint(-1)
{
    QwtPlotCanvas *canvas = plot->canvas();
    canvas->installEventFilter(this);

    // We want the focus, but no focus rect. The
    // selected point will be highlighted instead.

#if QT_VERSION >= 0x040000
    canvas->setFocusPolicy(Qt::StrongFocus);
#ifndef QT_NO_CURSOR
    canvas->setCursor(Qt::PointingHandCursor);
#endif
#else
    canvas->setFocusPolicy(QWidget::StrongFocus);
#ifndef QT_NO_CURSOR
    canvas->setCursor(Qt::pointingHandCursor);
#endif
#endif
    canvas->setFocusIndicator(QwtPlotCanvas::ItemFocusIndicator);
    canvas->setFocus();

    const char *text =
        "All points can be moved using the left mouse button "
        "or with these keys:\n\n"
        "- Up:\t\tSelect next curve\n"
        "- Down:\t\tSelect previous curve\n"
        "- Left, ´-´:\tSelect next point\n"
        "- Right, ´+´:\tSelect previous point\n"
        "- 7, 8, 9, 4, 6, 1, 2, 3:\tMove selected point";
#if QT_VERSION >= 0x040000
    canvas->setWhatsThis(text);
#else
    QWhatsThis::add(canvas, text);
#endif

    shiftCurveCursor(true);
}

bool CanvasPicker::event(QEvent *e)
{
    if ( e->type() == QEvent::User )
    {
        showCursor(true);
        return true;
    }
    return QObject::event(e);
}

bool CanvasPicker::eventFilter(QObject *object, QEvent *e)
{
    if ( object != (QObject *)plot()->canvas() )
        return false;

    switch(e->type())
    {
        case QEvent::FocusIn:
            showCursor(true);
        case QEvent::FocusOut:
            showCursor(false);

        case QEvent::Paint:
        {   
            QApplication::postEvent(this, new QEvent(QEvent::User));
            break;
        }
        case QEvent::MouseButtonPress:
        {
            select(((QMouseEvent *)e)->pos());
            return true; 
        }
        case QEvent::MouseMove:
        {
            move(((QMouseEvent *)e)->pos());
            return true; 
        }
        case QEvent::KeyPress:
        {
            const int delta = 5;
            switch(((const QKeyEvent *)e)->key())
            {
                case Qt::Key_Up:
                    shiftCurveCursor(true);
                    return true;
                    
                case Qt::Key_Down:
                    shiftCurveCursor(false);
                    return true;

                case Qt::Key_Right:
                case Qt::Key_Plus:
                    if ( d_selectedCurve )
                        shiftPointCursor(true);
                    else
                        shiftCurveCursor(true);
                    return true;

                case Qt::Key_Left:
                case Qt::Key_Minus:
                    if ( d_selectedCurve )
                        shiftPointCursor(false);
                    else
                        shiftCurveCursor(true);
                    return true;

                // The following keys represent a direction, they are
                // organized on the keyboard.
 
                case Qt::Key_1: 
                    moveBy(-delta, delta);
                    break;
                case Qt::Key_2:
                    moveBy(0, delta);
                    break;
                case Qt::Key_3: 
                    moveBy(delta, delta);
                    break;
                case Qt::Key_4:
                    moveBy(-delta, 0);
                    break;
                case Qt::Key_6: 
                    moveBy(delta, 0);
                    break;
                case Qt::Key_7:
                    moveBy(-delta, -delta);
                    break;
                case Qt::Key_8:
                    moveBy(0, -delta);
                    break;
                case Qt::Key_9:
                    moveBy(delta, -delta);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
    return QObject::eventFilter(object, e);
}

// Select the point at a position. If there is no point
// deselect the selected point

void CanvasPicker::select(const QPoint &pos)
{
    SpecBox * test;
    //std::cout << "mouse select" << pos.x() << "\t" << pos.y() << "\n";
    test = (SpecBox *) plot()->parent();
    // set the marker on the plot in plot coordinates
    test->setMarker(plot()->invTransform(QwtPlot::xBottom,pos.x()));
}

// Move the selected point
void CanvasPicker::moveBy(int dx, int dy)
{
    if ( dx == 0 && dy == 0 )
        return;

    if ( !d_selectedCurve )
        return;

    const int x = plot()->transform(d_selectedCurve->xAxis(),
        d_selectedCurve->x(d_selectedPoint)) + dx;
    const int y = plot()->transform(d_selectedCurve->yAxis(),
        d_selectedCurve->y(d_selectedPoint)) + dy;

    move(QPoint(x, y));
}

// Move the selected point
void CanvasPicker::move(const QPoint &pos)
{
    SpecBox * test;
    //std::cout << "mouse move" << pos.x() << "\t" << pos.y() << "\n";
    test = (SpecBox *) plot()->parent();

    test->setMarker(plot()->invTransform(QwtPlot::xBottom,pos.x()));
}

// Hightlight the selected point
void CanvasPicker::showCursor(bool showIt)
{
    if ( !d_selectedCurve )
        return;

    const QwtSymbol symbol = d_selectedCurve->symbol();

    QwtSymbol newSymbol = symbol;
    if ( showIt )
        newSymbol.setBrush(symbol.brush().color().dark(150));

    const bool doReplot = plot()->autoReplot();

    plot()->setAutoReplot(false);
    d_selectedCurve->setSymbol(newSymbol);

    d_selectedCurve->draw(d_selectedPoint, d_selectedPoint);

    d_selectedCurve->setSymbol(symbol);
    plot()->setAutoReplot(doReplot);
}

// Select the next/previous curve 
void CanvasPicker::shiftCurveCursor(bool up)
{
    QwtPlotItemIterator it;

    const QwtPlotItemList &itemList = plot()->itemList();

    QwtPlotItemList curveList;
    for ( it = itemList.begin(); it != itemList.end(); ++it )
    {
        if ( (*it)->rtti() == QwtPlotItem::Rtti_PlotCurve )
            curveList += *it;
    }
    if ( curveList.isEmpty() )
        return;

    it = curveList.begin();

    if ( d_selectedCurve )
    {
        for ( it = curveList.begin(); it != curveList.end(); ++it )
        {
            if ( d_selectedCurve == *it )
                break;
        }
        if ( it == curveList.end() ) // not found
            it = curveList.begin();

        if ( up )
        {
            ++it;
            if ( it == curveList.end() )
                it = curveList.begin();
        }
        else
        {
            if ( it == curveList.begin() )
                it = curveList.end();
            --it;
        }
    }
        
    showCursor(false);
    d_selectedPoint = 0;
    d_selectedCurve = (QwtPlotCurve *)*it;
    showCursor(true);
}

// Select the next/previous neighbour of the selected point
void CanvasPicker::shiftPointCursor(bool up)
{
    if ( !d_selectedCurve )
        return;

    int index = d_selectedPoint + (up ? 1 : -1);
    index = (index + d_selectedCurve->dataSize()) % d_selectedCurve->dataSize();

    if ( index != d_selectedPoint )
    {
        showCursor(false);
        d_selectedPoint = index;
        showCursor(true);
    }
}
