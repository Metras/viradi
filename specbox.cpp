#include <QtGui>
#include "specbox.h"
#include "specplot.h"
#include "mainwindow.h"
#include <qwt_slider.h>
#include <qwt_wheel.h>
#include <qwt_picker.h>
#include <qwt_plot_picker.h>
#include <qwt_text.h>
#include <iostream>
#include <sstream>

#include "canvaspicker.h"

using namespace std;
SpecBox::SpecBox(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    DBScale = 5;  //(dB / Div)
    RefLevel = 0;
    dBDiv = new QwtSlider(this, Qt::Vertical, QwtSlider::NoScale,QwtSlider::BgTrough);
    dBDiv->setRange(1,30,0);
    RefLvl = new QwtWheel(this);
    RefLvl->setOrientation(Qt::Vertical);
    RefLvl->setRange(-100,100,0);
    RefLvl->setMass(10.0);


    plot = new SpecPlot(this);
    plot->setCanvasBackground(Qt::black);

    fcenter = new QwtPlotMarker();

    fcenter->setLineStyle(QwtPlotMarker::VLine);
    fcenter->setLinePen(QPen(Qt::red, 0, Qt::SolidLine));
    fcenter->setXValue(100.0);
    fcenter->setVisible(true);
    QString out;
    out.setNum(500.0,'g',3);
    fcenter->setLabel(QwtText(out));
    plot->replot();

    RefLevel=10.0;
    DBScale=5.0; // 5 dB/Div
    plot->setAxisScale(QwtPlot::yLeft,RefLevel - DBScale*10, RefLevel);

    fcenter->attach(plot);

fcenter->setXValue(200.0);

    layout->addWidget(dBDiv);    QwtPlotMarker * marker;
    layout->addWidget(plot);
    layout->addWidget(RefLvl);
    connect(RefLvl,SIGNAL(valueChanged(double)),this,SLOT(setRefLevel(double)));
    connect(dBDiv,SIGNAL(valueChanged(double)),this,SLOT(setdBscale(double)));
//   (void) new CanvasPicker(plot);
//    QwtPicker *picker = new QwtPicker(plot);
           picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
           QwtPicker::PointSelection | QwtPicker::DragSelection,
           QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn,
           plot->canvas());

//    QwtText * text = picker->trackerText(
    picker->setTrackerPen(QPen(Qt::white, Qt::SolidLine));

//    text->setColor(Qt::white);
    connect(picker,SIGNAL(moved(QwtDoublePoint)),this,SLOT(setMarker(QwtDoublePoint)));
    connect(picker,SIGNAL(selected(QwtDoublePoint)),this,SLOT(setMarker(QwtDoublePoint)));
    picker->setRubberBandPen(QPen(Qt::red, 0, Qt::SolidLine));
    picker->setTrackerMode(QwtPicker::AlwaysOn);
//    picker->setSelectionFlags(QwtPicker::RectSelection | QwtPicker::DragSelection);


}




void SpecBox::setRefLevel(double n) {
    RefLevel = n;
    plot->setAxisScale(QwtPlot::yLeft,RefLevel - DBScale*10, RefLevel);
    plot->replot();
}

void SpecBox::setdBscale(double n) {
    DBScale = n;
    plot->setAxisScale(QwtPlot::yLeft,RefLevel - DBScale*10, RefLevel);
    plot->replot();
}

void SpecBox::setMarker(double n) {
    QString out;
    out.setNum(n,'g',3);
    //std::cout << "marker set at " << n << "\n";
    plot->replot();

}
void SpecBox::setMarker(QwtDoublePoint n) {
    QString out;
    out.setNum(n.x(),'g',3);
    //picker->transform(n);
    //cout << "marker set at " << n.x() << "\n";

    //fcenter->setXValue((double) n.x());
    //fcenter->setLabel(QwtText(out));
    //plot->LabelFrequency->setLabel(QwtText(out));
    plot->replot();

}

