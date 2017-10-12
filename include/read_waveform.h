#include "TRint.h"
#include "TROOT.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"

#define NUMCHS 4

int readwfm_b(int n_ch, int chans, Float_t *wave_x, Float_t *wave_y, Float_t *points, char filename[]);

int32_t readinfo_b(char filename[]);
