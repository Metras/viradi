#ifndef SPECBOX_H
#define SPECBOX_H

#include <QWidget>
#include "specplot.h"
#include <qwt_slider.h>
#include <qwt_wheel.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_marker.h>

class SpecBox : public QWidget
{
Q_OBJECT
public:
    explicit SpecBox(QWidget *parent = 0);
    SpecPlot * plot;
    QwtSlider * dBDiv;
    QwtWheel * RefLvl;
    QwtPlotPicker * picker;
    QwtPlotMarker * fcenter;
    double RefLevel;
    double DBScale;
signals:

public slots:
    void setRefLevel(double);
    void setdBscale(double);
    void setMarker(double);
    void setMarker(QwtDoublePoint n);
};

#endif // SPECBOX_H
