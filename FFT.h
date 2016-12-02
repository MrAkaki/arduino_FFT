#ifndef MIC_PIN
  #define MIC_PIN 0
#endif

#define DC_OFFSET       (1023 / 3)
#define NOISE_THRESHOLD 3

typedef struct {
  int16_t r;
  int16_t i;
} complex_t;

extern "C" { // In ffft.S
  void fft_input(const int16_t *, complex_t *);
  void fft_execute(complex_t *);
  void fft_output(complex_t *, uint16_t *);
}


void capture(int16_t *buf, uint8_t nSamples) {
  uint8_t admux_save, adcsra_save, adcsrb_save, timsk0_save, channel;
  int16_t adc;
  channel     = analogPinToChannel(MIC_PIN); // Pin A4 to ADC channel
  admux_save  = ADMUX;                 // Save ADC config registers
  adcsra_save = ADCSRA;
  adcsrb_save = ADCSRB;
  // Init ADC free-run mode; f = ( 8MHz/prescaler ) / 13 cycles/conversion
  ADCSRA = 0;                          // Stop ADC interrupt, if any
  ADMUX  = _BV(REFS0) | channel;       // Aref=AVcc, channel sel, right-adj
  ADCSRB = 0;                          // Free run mode, no high MUX bit
  ADCSRA = _BV(ADEN)  |                // ADC enable
           _BV(ADSC)  |                // ADC start
           _BV(ADATE) |                // Auto trigger
           _BV(ADIF)  |                // Reset interrupt flag
           _BV(ADPS2) | _BV(ADPS1);    // 64:1 / 13 = 9615 Hz
  for(uint8_t i=0; i<nSamples; i++) {
    while(!(ADCSRA & _BV(ADIF)));      // Wait for ADC result
    adc     = ADC;
    ADCSRA |= _BV(ADIF);               // Clear bit
    if(adc <= (DC_OFFSET - NOISE_THRESHOLD)) {
      adc  -= DC_OFFSET;
    } else if(adc >= (DC_OFFSET + NOISE_THRESHOLD)) {
      adc  -= DC_OFFSET;
      if(adc > (DC_OFFSET * 2)) adc = DC_OFFSET * 2;
    } else {
      adc   = 0; // Below noise threshold
    }
    buf[i]  = adc;
  }
  ADMUX  = admux_save;                 // Restore ADC config
  ADCSRB = adcsrb_save;
  ADCSRA = adcsra_save;
  (void)analogRead(MIC_PIN);                // Purge residue from ADC register
}

void fft(uint16_t *spectrum) {               // Spectrum output buffer, uint16_t[32]
    int16_t   capBuf[64];            // Audio capture buffer
    complex_t butterfly[64];         // FFT "butterfly" buffer
    capture(capBuf, 64);             // Collect mic data into capBuf
    fft_input(capBuf, butterfly);    // Samples -> complex #s
    fft_execute(butterfly);          // Process complex data
    fft_output(butterfly, spectrum); // Complex -> spectrum (32 bins)
}
