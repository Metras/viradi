#include <QtGui>
#include "mainwindow.h"
#include "specplot.h"
#include "specbox.h"
#include "wavfile.h"
#include <qwt_wheel.h>
#include <qwt_plot_curve.h>
#include <iostream>
#include "/usr/include/qt4/QtMultimedia/QAudioOutput"
#include <QIODevice>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    Timer(new QTimer(this)),
    m_output(0)
{

//    SpecPlot * myplot;
    x = QVector<double> (4096);
    y = QVector<double> (4096);
    QWidget *main = new QWidget(this);
    this->setCentralWidget(main);

    QVBoxLayout *layout = new QVBoxLayout;

    widget = new SpecBox();
    widget->setMarker(500.0);

    freq = new QwtWheel(this);
    freq->setOrientation(Qt::Horizontal);
    freq->setRange(0,1000,0);
    freq->setMass(10.0);
    freq->setValue(500.0);
    nFFT = 2048;

    connect(freq,SIGNAL(valueChanged(double)),this,SLOT(setFreq(double)));

//   setCentralWidget(widget);
    layout->addWidget(widget);
    layout->addWidget(freq);
    main->setLayout(layout);
    QString fileName = QFileDialog::getOpenFileName(this);
    wv = new WavFile(nFFT, fileName.toStdString());


    inputFile.setFileName(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QAudioFormat format;
    // Set up the format, eg.
    format.setFrequency(wv->getSampleFreq());
    format.setChannels(2);
    format.setSampleSize(16);

    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioOutput(format, this);
m_output = audio->start();

    connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
    connect(Timer,SIGNAL(timeout()),SLOT(TimerExpired()));
    int twindow = (int) (1000 * nFFT)/wv->getSampleFreq();
    cout << "starting timer @\t" << twindow << " milliseconds interval Sample rate is\t" << wv->getSampleFreq() << "Hz\n";
    Timer->start(twindow);
    //audio->start(&inputFile);
    //audio->start((QIODevice *) wv);
    //cout << audio->state() << "\n";

    widget->plot->setAxisScale(QwtPlot::xBottom, wv->getSampleFreq()/-2.0, wv->getSampleFreq()/2.0);
    widget->plot->setAxisScale(QwtPlot::yLeft,-150,-100,0);
    curve = new QwtPlotCurve("Courbe");
    widget->plot->clear();

    QApplication::processEvents();


}


MainWindow::~MainWindow()
{
}

void MainWindow::setFreq(double n) {

    widget->setMarker(n);

}

void MainWindow::TimerExpired()
{
    //cout << "timer expired, reading chunk\n";

    wv->ReadChunk(nFFT,x.data(),y.data());  //read the next chunk
    curve->setData(x.data(), y.data(), nFFT);
    curve->setPen(QPen(Qt::green));
    curve->attach(widget->plot);
m_output->write(wv->DataBuffer,wv->len);
    widget->plot->replot();
    // moutput->write wv->DataBuffer}
}

void MainWindow::finishedPlaying(QAudio::State state)
 {
   if(state == QAudio::IdleState) {
     audio->stop();
     inputFile.close();
     qWarning()<<"finished playing ...";
     delete audio;
   }
 }

