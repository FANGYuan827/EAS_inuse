//#include <stdlib.h>
//#include <fft.h>

//#define N 128  /* FFT Length  */
//#pragma DATA_SECTION(ipcb, "FFTipcb");
//CFFT32 fft = CFFT32_128P_DEFAULTS;
//long ipcb[2*N]; /* In place computation buffer */

//main()
//{ 
/* FFT initialization  */
//fft.ipcbptr=ipcb; /* FFT computation buffer */
//fft.magptr=ipcb; /* Store back the mag. square in ¡°ipcb¡± */
//fft.init(); /* Twiddle factor pointer initialization */
/* Acquire samples in bit reversed order or
 Bit-reverse the in-order data using bit-rev utility  */
/* FFT Computation  */
//fft.izero(&fft); /* Zero the imaginary part */
//fft.calc(&fft);  /* Compute the FFT  */
//fft.mag(&fft); /* Obtain the magnitude square  */
//}

