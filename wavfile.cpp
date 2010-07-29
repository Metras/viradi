#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h> // for exit()
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QtCore/qmath.h>
#include <fftw3.h>
#include "wavfile.h"

using namespace std;

WavFile::WavFile( int n ) : filePositionPointer ( 0 )
{
   NFFT = n;


   cout << "Constructor: NFFT = " << NFFT << endl;
   boolFiledefined=false;
}
WavFile::WavFile()
{
    //conctructor default
    boolFiledefined=false;
    cout << "from default cponstructor\n";

}

WavFile::WavFile(int n, const string & strfileNameRef)


// constructor, input NFFT, str filename

{
        cout << "contructor \n";
	NFFT = n;
        cout << "with " << n << " FFT size\n";
	string strid;
        wsampleNo=0;  //initialize ca,ple read count
// allocate memorw segment for fft


   
   getInfos(strfileNameRef); //read and check headers
   len = NFFT * wwavHdr.wBytesPerSample * wwavHdr.wNbChannels;
   DataBuffer.resize(len);   //create the data buffer

}
void WavFile::printInfos() {

// print the header blocks from currently opened file need to do a check if a file is actually opened !!!

   string strid;
if (boolFiledefined) {
	cout << "RIFF File Information (printInfos)\n";
   strid.assign(wwavHdr.strFileType,0,4);
   cout << "File type:\t" << strid << " of " << wwavHdr.dwFileSize + 8 << " Bytes (inc header)\n";
   strid.assign(wwavHdr.strFileId,0,4);
   cout << "File format:\t" << strid << "\n"; 
   strid.assign(wwavHdr.strChunkFmtId,0,4);
   cout << "============== fmt block header ====================\n";
   cout << "SubChunk1 type (fmt):\t" << strid << ", subchunk1 size (inc header): " << wwavHdr.dwChunkFmtSize + 8 << " Bytes\n"; 
   cout << "Audio format:\t"<< wwavHdr.wFormat << "\n";
   cout << "Nbr channel:\t" << wwavHdr.wNbChannels << "\n";
   cout << "sampling freq:\t" << wwavHdr.dwSamplingFreq << " KHz \n";
   cout << "Bytes per second:\t" << wwavHdr.dwBytesPerSec << "\n";
   cout << "Bytes per sample:\t" << wwavHdr.wBytesPerSample << "\n";
   cout << "bits per sample:\t" << wwavHdr.wDepth << "\n ";
   cout << "============== auxi block header ====================\n";
   strid.assign(Dblock.strSubChunk2ID,0,4);
   cout << "SubChunk 2 ID (auxi):\t" << strid << "\n";
   cout << "SubChunk 2 Size:\t" << Dblock.dwsubChunk2Size + 8 << " Bytes (inc header)\n";
   cout << "Start time:\t" << Dblock.samplestart.year << "/" << Dblock.samplestart.month << "/" << Dblock.samplestart.day << " ";
   cout << Dblock.samplestart.hour << ":" << Dblock.samplestart.minute << ":" << Dblock.samplestart.second << "." << Dblock.samplestart.millisecond << "\n";
   cout << "Stop time:\t" << Dblock.samplestop.year << "/" << Dblock.samplestop.month << "/" << Dblock.samplestop.day << " ";
   cout << Dblock.samplestop.hour << ":" << Dblock.samplestop.minute << ":" << Dblock.samplestop.second << "." << Dblock.samplestop.millisecond << "\n";
   cout << "Frequency:\t" << Dblock.frequency << " KHz\n";
   cout << "=================================\n";
   cout << filePositionPointer << " File position pointer as calculated\n";
} else {
   cout << "sorry no file header has been read yet ...\n";
}
}
	
void WavFile::getInfos(const string &strfileNameRef) {
//open the file specified in argument, read the headers and stores them
// set the file position pointer to the data subchunk location

    SUBCHUNKHDR chunk;
    string strid;

    wavFile.open(strfileNameRef.c_str(), ios_base::binary);
   if( !wavFile )
   {
      cout << "Opening file failure!\n"; // to be improved
   }
   else cout << "File opened correctly.\n";

   wavFile.seekg(ios::beg); //start reading at first byte
   wavFile.read((char *) (&wwavHdr), sizeof(FMTHDR)); //read the Chunk and Chunk1 headers

   wavFile.read((char *) (&Dblock), sizeof(DATAHDR));  // read the chunk2 block header
	
   boolFiledefined=true; //set flag

//compute the first data byte position = RIFF Header + fmt Header chunk1 + chunk2 header
   filePositionPointer = 12 + wwavHdr.dwChunkFmtSize + 8 + Dblock.dwsubChunk2Size + 8 ;

//and get the first data chunk header info
   wavFile.seekg( (streampos) filePositionPointer ); //position file pointer
   filePositionPointer+=8;
   wavFile.read((char *) (&chunk), sizeof(SUBCHUNKHDR)); //read chunk header
   strid.assign(chunk.strID,0,4);

   if (strid != "data") {
           cout << "data ID not found !!!\n";
           exit (-2);
           }

   wnumsample = ( chunk.dwSize * 8) / (wwavHdr.wDepth * wwavHdr.wNbChannels);
   cout << "A total of:\t" << wnumsample << "samples are in the chunk\n";

//health check
// ID must be RIFF, type must be WAVE, chunk1 must be fmt and chunk2 auxi  
	strid.assign(wwavHdr.strFileType,0,4);
	if (strid != "RIFF") {
		cout << "not a RIFF file\n";
		exit (-1);
		}
	strid.assign(wwavHdr.strFileId,0,4);
	if (strid != "WAVE") {
		cout << "WAVE Header not found\n";
		exit (-1);
		}
	strid.assign(wwavHdr.strChunkFmtId,0,4);	
	if (strid != "fmt ") {
		cout << "fmt Header not found\n";
		exit (-1);
		}
	strid.assign(Dblock.strSubChunk2ID,0,4);	
	if (strid != "auxi") {
		cout << "auxi Header not found\n";
		exit (-1);
		}
}
/*******************************************************************/

bool WavFile::ReadChunk(int n, double * x, double * power_spectrum_dBm ) {


SUBCHUNKHDR chunk;
PAIR data;
string strid;
in_fft   = (fftw_complex*) fftw_malloc( sizeof( fftw_complex ) * n );
out_fft  = (fftw_complex*) fftw_malloc( sizeof( fftw_complex ) * n );
out_ifft = (fftw_complex*) fftw_malloc( sizeof( fftw_complex ) * n );
BufPtr = 0;  //initialise pointer

ADC_Range=pow(2.0, (double) 15);
//cout << "Starting data read @ position FPP\t " << filePositionPointer << "\n" << "ADC_Range =" << ADC_Range << "\n";

wavFile.seekg( (streampos) filePositionPointer ); //position file pointer

//cout << "reading " << n << " sample\n";

	for ( int i = 0; i < n; i++) {
	// read at most wnumsample from the chunk
	if ( wsampleNo < wnumsample) {
		wavFile.read((char *) (&data), sizeof(PAIR));
                DataBuffer[BufPtr++]= (char) data.R; //data.R.p[0];
                DataBuffer[BufPtr++]= (char) data.R >> 8;
                DataBuffer[BufPtr++]= (char) data.I;
                DataBuffer[BufPtr++]= (char) data.I >> 8;

                in_fft[i][0]=(double)data.R / ADC_Range; filePositionPointer += 2;
                in_fft[i][1]=(double)data.I / ADC_Range; filePositionPointer += 2;
                wsampleNo++;   //update number of sample read
//                cout << i << "\t" << data.R << "\t" << data.I << scientific << I[ i ] << "\t" <<Q[i] << "\n";
		} else {
		// we should be at the start of another chunk
			wavFile.read((char *) (&chunk), sizeof(SUBCHUNKHDR)); //read chunk header
			strid.assign(chunk.strID,0,4);
			wsampleNo = 0;  //reset sample counter
			if (strid != "data") {
				cout << "data ID not found !!!\n";
                                return (false);
			}
			wnumsample = ( chunk.dwSize * 8) / (wwavHdr.wDepth * wwavHdr.wNbChannels);
                        //cout << "A total of:\t" << wnumsample << "samples are in the chunk\n";
                        //cout << "reading " << n << " sample\n";

		}
	}
// compute the fft
fftw_plan p = fftw_plan_dft_1d( n, in_fft, out_fft, FFTW_FORWARD, FFTW_ESTIMATE );
fftw_execute(p);
for ( int k = 0; k < n; k++)
   {
      out_fft[k][0] /= (double)n;
      out_fft[k][1] /= (double)n;
      power_spectrum_dBm[k] = 20.0 *
         log10( sqrt(out_fft[k][0]*out_fft[k][0] + out_fft[k][1]*out_fft[k][1]) / 50.0 );

      x[k]= ((double)k-((double)n/2.0)) * (wwavHdr.dwSamplingFreq/(double)n);

      //cout << "Power spectrum :\t" << Q[k] << "\n";
   }
fftw_free( in_fft );
fftw_free( out_fft );
fftw_free( out_ifft );
return (true);
}

void WavFile::setSize(int n) {

    NFFT = n;

}

bool WavFile::FileisDefined() {
    if (boolFiledefined) {
        cout << "file is defined\n";
    } else {
        cout << "file is not yet defined\n";
    }
    return boolFiledefined;
}

void WavFile::undefined() {
    boolFiledefined=false;
}

double WavFile::getSampleFreq() {
    return wwavHdr.dwSamplingFreq;
}
qint64 WavFile::readData(char *data, qint64 len)
{
    char sampled[4];
    SUBCHUNKHDR chunk;
    string strid;
    wavFile.seekg( (streampos) filePositionPointer ); //position file pointer
    len = 0;
    cout << "reading 2048  sample\n";

            for ( int i = 0; i < 2048; i++) {
            // read at most wnumsample from the chunk
            if ( wsampleNo < wnumsample) {
                    wavFile.read(sampled, 4);
                    *data++ = sampled[0];
                    *data++ = sampled[1];
                    *data++ = sampled[2];
                    *data++ = sampled[3];
                    filePositionPointer += 2;
                    filePositionPointer += 2;
                    wsampleNo++;   //update number of sample read
                    len += 4;
    //                cout << i << "\t" << data.R << "\t" << data.I << scientific << I[ i ] << "\t" <<Q[i] << "\n";
                    } else {
                    // we should be at the start of another chunk
                            wavFile.read((char *) (&chunk), sizeof(SUBCHUNKHDR)); //read chunk header
                            strid.assign(chunk.strID,0,4);
                            wsampleNo = 0;  //reset sample counter
                            if (strid != "data") {
                                    cout << "data ID not found !!!\n";
                                    return (false);
                            }
                            wnumsample = ( chunk.dwSize * 8) / (wwavHdr.wDepth * wwavHdr.wNbChannels);
                            //cout << "A total of:\t" << wnumsample << "samples are in the chunk\n";
                            //cout << "reading " << n << " sample\n";

                    }
            }

    return 0;
}

qint64 WavFile::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

void WavFile::start()
{

    cout << "starting the QIODevice\n";
        open(QIODevice::ReadOnly);
    }

void WavFile::stop()
{
    close();
}






