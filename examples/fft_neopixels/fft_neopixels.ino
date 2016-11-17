#include <Adafruit_NeoPixel.h>
#include <FFT.h>

#define PIXELS_PIN 6
#define PIXELS_N 21

#define BINS   10          // FFT output is filtered down to this many bins
#define FRAMES 4           // This many FFT cycles are averaged for leveling

const float ADD = ((float)BINS)/((float)PIXELS_N);

uint8_t lvl[FRAMES][BINS], // Bin levels for the prior #FRAMES frames
        avgLo[BINS],       // Pseudo rolling averages for bins -- lower and
        avgHi[BINS],       // upper limits -- for dynamic level adjustment.
        frameIdx = 0;      // Counter for lvl storage
const uint8_t PROGMEM
  // Low-level noise initially subtracted from each of 32 FFT bins
  noise[]    = { 0x04,0x03,0x03,0x03, 0x02,0x02,0x02,0x02,
                 0x02,0x02,0x02,0x02, 0x01,0x01,0x01,0x01,
                 0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01,
                 0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01 },
  bin0data[] = { 1, 2, 147 },
  bin1data[] = { 2, 2, 89, 14 },
  bin2data[] = { 2, 3, 89, 14 },
  bin3data[] = { 4, 3, 15, 181, 58, 3 },
  bin4data[] = { 4, 4, 15, 181, 58, 3 },
  bin5data[] = { 6, 5, 6, 89, 185, 85, 14, 2 },
  bin6data[] = { 7, 7, 5, 60, 173, 147, 49, 9, 1 },
  bin7data[] = { 10, 8, 3, 23, 89, 170, 176, 109, 45, 14, 4, 1 },
  bin8data[] = { 13, 11, 2, 12, 45, 106, 167, 184, 147, 89, 43, 18, 6, 2, 1 },
  bin9data[] = { 18, 14, 2, 6, 19, 46, 89, 138, 175, 185, 165, 127, 85, 51, 27, 14, 7, 3, 2, 1 },
  * const binData[] = { bin0data, bin1data, bin2data, bin3data, bin4data,
                        bin5data, bin6data, bin7data, bin8data, bin9data };
const uint16_t PROGMEM
  binMul[] = { 405, 508, 486, 544, 533, 487, 519, 410,
               481, 413, 419, 410, 397, 424, 412, 411,
               511, 591, 588, 577, 554, 529, 524, 570,
               546, 559, 511, 552, 439, 488, 483, 547, },
  binDiv[]   = { 147, 103, 103, 257, 257, 381, 444, 634, 822, 1142 };

uint16_t spectrum[32];

Adafruit_NeoPixel pixels =  Adafruit_NeoPixel(PIXELS_N, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  uint8_t i;
  for(i=0; i<BINS; i++) {
    avgLo[i] = 0;
    avgHi[i] = 255;
  }
  for(i=0; i<FRAMES; i++) {
    memset(&lvl[i], 127, sizeof(lvl[i]));
  }
}

uint8_t niveles[BINS];
uint8_t valores[PIXELS_N];
float d;

void loop() {
  fft(spectrum);
  uint8_t  i, N;
  uint16_t S;
  for(i=0; i<32; i++) {
    N = pgm_read_byte(&noise[i]);
    if(spectrum[i] > N) { // Above noise threshold: scale & clip
      S = ((spectrum[i] - N) * (uint32_t)pgm_read_word(&binMul[i])) >> 8;
      spectrum[i] = (S < 255) ? S : 255;
    } else {
      spectrum[i] = 0;
    }
  }

  uint16_t sum, level;
  uint8_t  j, minLvl, maxLvl, nBins, binNum, *data;
  
  for(i=0; i<BINS; i++) { // For each output bin (and each pixel)...
    data   = (uint8_t *)pgm_read_word(&binData[i]);
    nBins  = pgm_read_byte(&data[0]); // Number of input bins to merge
    binNum = pgm_read_byte(&data[1]); // Index of first input bin
    data  += 2;
    for(sum=0, j=0; j<nBins; j++) {
      sum += spectrum[binNum++] * pgm_read_byte(&data[j]); // Total
    }
    sum /= pgm_read_word(&binDiv[i]);                      // Average
    lvl[frameIdx][i] = sum;      // Save for rolling averages
    minLvl = maxLvl = lvl[0][i]; // Get min and max range for bin
    for(j=1; j<FRAMES; j++) {    // from prior stored frames
      if(lvl[j][i] < minLvl)      minLvl = lvl[j][i];
      else if(lvl[j][i] > maxLvl) maxLvl = lvl[j][i];
    }
    if((maxLvl - minLvl) < 23) {
      maxLvl = (minLvl < (255-23)) ? minLvl + 23 : 255;
    }
    avgLo[i] = (avgLo[i] * 7 + minLvl) / 8; // Dampen min/max levels
    avgHi[i] = (maxLvl >= avgHi[i]) ?       // (fake rolling averages)
      (avgHi[i] *  3 + maxLvl) /  4 :       // Fast rise
      (avgHi[i] * 31 + maxLvl) / 32;        // Slow decay

    level = 1 + ((sum <= avgLo[i]) ? 0 :
                 256L * (sum - avgLo[i]) / (long)(avgHi[i] - avgLo[i]));
    
    if(level>255) level = 255;
    niveles[i] = level;
  }
  uint8_t index;
  for(uint8_t i=0; i<PIXELS_N; ++i){
    d = ADD*(float)i;
    index = d;
    d -= index;
    uint8_t intensidad = ((float)niveles[index])*d + ((float)niveles[index])*(1.0-d);
    pixels.setPixelColor(i,intensidad,0,0);
  }
  pixels.show();
}
