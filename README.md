# Libreria FFT para arduino

  Libreria FFT para placas basadas en (Arduino Uno)ATMEGA328 (Arduino Leonardo)ATMEGA32UA.
  Codigo basado en http://wiki.openmusiclabs.com/wiki/ArduinoFFT y https://github.com/adafruit/Adafruit_CircuitPlayground/

# Codigo

  Esta libreria define las siguientes funciones:
  ```code
       void fft_input(const int16_t *, complex_t *);
       void fft_execute(complex_t *);
       void fft_output(complex_t *, uint16_t *);
       void capture(int16_t*, uint8_t);
       void fft(uint16_t *);
  ```
  Ademas define los siguientes tipos de datos:
  ```code
    typedef struct {
      int16_t r;
      int16_t i;
    } complex_t;
  ```

<br/><br/>
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">FFT_Arduino</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/alexvargasbenamburg/FFT_Arduino" property="cc:attributionName" rel="cc:attributionURL">Alex Vargas Benamburg</a> <br/>is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.<br />Based on a work at <a xmlns:dct="http://purl.org/dc/terms/" href="http://wiki.openmusiclabs.com/wiki/ArduinoFFT" rel="dct:source">http://wiki.openmusiclabs.com/wiki/ArduinoFFT</a>.
