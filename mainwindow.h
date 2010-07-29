#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "specbox.h"
#include "/usr/include/qt4/QtMultimedia/QAudioOutput"
#include <QIODevice>
#include <QVector>
#include <QTimer>
#include <QFile>
#include<qwt_plot_curve.h>
#include "wavfile.h"

#include <QtGui/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SpecBox * widget;
    QwtWheel * freq;
    QFile inputFile;     // class member.
    QAudioOutput* audio; // class member.
    QIODevice * m_output;
    WavFile * wv;
    int nFFT;

public slots:
    void setFreq(double);
    void finishedPlaying(QAudio::State);
    void TimerExpired();
private:
        QTimer * Timer;

        QVector<double> x ;
        QVector<double> y ;
        QwtPlotCurve *curve ;

};

#endif // MAINWINDOW_H
