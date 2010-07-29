#ifndef SPECPLOT_H
#define SPECPLOT_H
#include <qwt_plot.h>
#include <qwt_plot_marker.h>

class SpecPlot : public QwtPlot
{
    Q_OBJECT

public:
    SpecPlot(QWidget * = NULL);


};

#endif // SPECPLOT_H
