#ifndef WAVFILE_HPP
#define WAVFILE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <complex>
#include <fftw3.h>
#include <QtCore/qmath.h>
#include <QObject>
#include <QIODevice>
#include <QByteArray>

using namespace std;

typedef struct 
 {

// definir une structure correspondante a l'êntête des fichiers RIFF (WAV)
 char strFileType[4]; //Chunk ID identification du format (RIFF)
 unsigned int dwFileSize; //Chunk size taille du fichier (taille du fichier - 8)
 char strFileId[4]; //Format type de fichier (Wave)
 char strChunkFmtId[4]; //SubChunk1ID identification du chunk format (fmt ou data)
 unsigned int dwChunkFmtSize; //SubChunk1 size taille du chunck format (16)
 unsigned short wFormat; //Audio format (1)
 unsigned short wNbChannels;  //num channel nombre de canaux (mono:1 ou stéréo:2)
 unsigned int dwSamplingFreq; //sample rate fréquence d'échantillonnage (44,1 kHz)
 unsigned int dwBytesPerSec; //byterate nombre de bits par secondes (wDepth * wNbChannels / 8)
 unsigned short wBytesPerSample;  //block align nombre de bits par échantillonnage (dwBytesPerSec * wDepth)
 unsigned short wDepth; //Bits per sample nombre de bits par échantillonnage (8 ou 16 bits)
 } FMTHDR;
typedef struct {
	char strID[4];
	unsigned int dwSize;
	} SUBCHUNKHDR;


typedef struct {
        short R;
        short I;
	} PAIR;
	
typedef struct {
	unsigned short year;
	unsigned short month;
	unsigned short dayofweek;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	unsigned short millisecond;
	} WAVDATE;
	
typedef struct {
	char strSubChunk2ID[4];  //ID of subsequent subchunk
	unsigned int dwsubChunk2Size;
 	WAVDATE samplestart;
 	WAVDATE samplestop;
 	unsigned int frequency;
} DATAHDR;



class WavFile : public QIODevice
{

Q_OBJECT

public:
    //WavFile(); // create an empty array of default size (NFFT=1024)

    WavFile(int n, const string & strfileNameRef);
    WavFile( int NFFT = 0 );
    WavFile();
    // fonctions definition
    // NB: fileName is the full path file name (string)
	
    // void wavOpen ( const string & fileName ); // needed ?
    void getInfos   ( const string & fileName );
    //void readFrames ( const string & fileName, int NFFT, complex<double> * );
    //void readFrames ( int NFFT, complex<double> * );
    //void readFrames ( int NFFT ); // ideal situation
    void printInfos();  //print information from last read file
    //void ReadData(int n, complex<double> wav_IQ[]);
    bool ReadChunk(int n, double * I, double * Q );
    void setSize(int n);

    bool FileisDefined();
    void undefined();
    double getSampleFreq();
    void start();
    void stop();
    QByteArray DataBuffer;
    int BufPtr;
    int len;

private:
   std::complex<double> *wav_IQ;
   fftw_complex *in_fft, *out_fft, *out_ifft; //pointeur vers FFT structures
   bool boolFiledefined;

   FMTHDR wwavHdr;
   DATAHDR Dblock;

   double ADC_Range;

   int NFFT;
   int wsampleNo;
   int wnumsample;    // number of samples per chunk
   long filePositionPointer;
   ifstream wavFile;

protected:

   qint64 readData(char *data, qint64 len);
   qint64 writeData(const char *data, qint64 len);



};

#endif // WAVFILE_HPP
