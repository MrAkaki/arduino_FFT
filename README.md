# Libreria FFT para arduino

  Libreria FFT para placas basadas en (Arduino Uno)ATMEGA328 (Arduino Leonardo)ATMEGA32UA.
  Codigo basado en http://wiki.openmusiclabs.com/wiki/ArduinoFFT y https://github.com/adafruit/Adafruit_CircuitPlayground/

# Codigo

  Esta libreria define las siguientes funciones:
  ```code
       /**
       * Computa las muestras a valores complejos.
       * @param const uint16_t * samples, entrada de 64 muestras.
       * @param complex_t* out, salida de 64 numeros complejos.
       **/
       void fft_input(const int16_t *samples, complex_t *out);

       /**
       * Procesa los valores complejos.
       * @param complex_t *input, entrada de la funcion, usualmente es la salida de la funcion fft_input.
       **/
       void fft_execute(complex_t *input);

       /**
       * Transforma los valores complejos a espectro.
       * @param complex_t *input, entrada de la funcion, usualmente es la salida de la funcion fft_execute.
       * @param uint16_t *spectrum, salida de la funcion.
       **/
       void fft_output(complex_t *intput, uint16_t *spectrum);

       /**
       * Captura n_samples del microfono y lo almacena en el parametro out.
       * @param int16_t *out, vector de tamanio n_samples donde se almacena la salida.
       * @param uint8_t n_samples, cantidad de muestras a tomar.
       **/
       void capture(int16_t *out, uint8_t n_samples);

       /**
       * Rutina que llama todas las funciones anteriores y retorna el spectro con 32 entradas
       * @param uint16_t *spectrum, salida del proceso de fft con muestras 32
       */
       void fft(uint16_t *spectrum);
  ```
  Ademas define los siguientes tipos de datos:
  ```code
    // Estructura de datos para representar numeros complejos
    typedef struct {
      int16_t r;
      int16_t i;
    } complex_t;
  ```

<br/><br/>
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">FFT_Arduino</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/alexvargasbenamburg/FFT_Arduino" property="cc:attributionName" rel="cc:attributionURL">Alex Vargas Benamburg</a> <br/>is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.<br />Based on a work at <a xmlns:dct="http://purl.org/dc/terms/" href="http://wiki.openmusiclabs.com/wiki/ArduinoFFT" rel="dct:source">http://wiki.openmusiclabs.com/wiki/ArduinoFFT</a>.
