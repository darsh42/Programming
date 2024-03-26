#include "../scientific.h"

/*
** Discrete Fourier Transform
** @INPUTS:
**   - transform -> resulting transformation array
**   - data      -> data to be fourier transformed
**   - samples   -> number of samples
** @OUTPUTS:
** @EQUATION: X_k = (n-1) ^ sum _ (m = 0) x_m * e ^ (-i*2*pi*k*m/n)
*/
void DFT(int *transform, int *data, int samples) {
    for (int k = 0, result = 0; k < samples; k++, result = 0) {
        for (int m = 0, x = data[m]; m < samples; m++, x = data[m])
            result += x * exp(-1.0*I*2*M_PI*k*m/samples);
        transform[k] = result;
    }
}

/*
** Inverse Discrete Fourier Transform
** @INPUTS:
**   - transform -> resulting transformation array
**   - data      -> data to be fourier transformed
**   - samples   -> number of samples
** @OUTPUTS:
** @EQUATION: x_m = 1/n * (n - 1)^sum_(k = 0) X_k * e ^ (i*2*pi*k*m/n)
*/
void IDFT(int *transform, int *data, int samples) {
    for (int m = 0, result = 0; m < samples; m++, result = 0) {
        for (int k = 0, X = data[k]; k < samples; k++, X = data[k])
            result += 1.0/samples * X * exp(1.0*I*2*M_PI*k*m/samples);
        transform[m] = result;
    }
}

/*
** Fast Fourier Transform
** @INPUTS:
**   - transform -> resulting transformation array
**   - data      -> data to be fourier transformed
**   - samples   -> number of samples
** @OUTPUTS:
** @EQUATIONS:
*/
void FFT(int *transform, int *data, int samples) {

}


/*
** Short Time Fourier Transform
** @INPUTS:
** @OUTPUTS:
** @EQUATIONS:
*/
void STFT(int *transform, int *data, int samples, int window, int overlap) {

}
