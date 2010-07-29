#include <qwt_plot.h>
#include "specplot.h"
#include <qwt_plot_grid.h>
#include <QtGui>
#include <qwt_plot_marker.h>

SpecPlot::SpecPlot(QWidget *parent) : QwtPlot(parent)
{
    this->setTitle("Spectrum");
    QwtPlotGrid *grid = new QwtPlotGrid;
        grid->enableXMin(true);
        grid->setMajPen(QPen(Qt::green, 0, Qt::SolidLine));
 //       grid->setMinPen(QPen(Qt::green, 0 , Qt::DotLine));
        grid->attach(this);

//        LabelFrequency = new QwtPlotMarker();
//        LabelFrequency->setXValue(750);
//        LabelFrequency->setYValue(5);
//        LabelFrequency->setLinePen(QPen(Qt::yellow, 0, Qt::SolidLine));
//        LabelFrequency->setLabel(QwtText("text"));
//        LabelFrequency->setLabelAlignment(Qt::AlignTop | Qt::AlignLeft);
//        LabelFrequency->attach(this);




}
