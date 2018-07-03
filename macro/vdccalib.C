#include <iostream>
#include <fstream>

int vdccalib(float tmin, float tmax){

  using namespace std;
  
  const float DRIFT_GAP = 10.0;
  
  if(tmin > tmax){
    printf("vdccalib(float tmin, float tmax)\n");
    printf("error: tmax should be larger than tmin.\n");
    return -1;
  }

  TH1F *hx1 = new TH1F;
  TH1F *hu1 = new TH1F;
  TH1F *hx2 = new TH1F;
  TH1F *hu2 = new TH1F;

  int integ_x1, integ_u1, integ_x2, integ_u2;
  
  /* get the histograms from the current file */
  gDirectory->GetObject("h1010",hx1);
  gDirectory->GetObject("h1011",hu1);
  gDirectory->GetObject("h1012",hx2);
  gDirectory->GetObject("h1013",hu2);

  int start_integ = h1010->FindBin(tmin);
  int stop_integ = h1010->FindBin(tmax);

  printf("\n");
  printf("-------------------------------------\n");
  printf("start=%f(bin=%d), stop=%f(bin=%d) \n",
	 tmin, start_integ, tmax, stop_integ);
  printf("\n");

  printf("X1 output: ./calib/vdccalib_x1.dat\n");
  printf("U1 output: ./calib/vdccalib_u1.dat\n");
  printf("X2 output: ./calib/vdccalib_x2.dat\n");
  printf("U2 output: ./calib/vdccalib_u2.dat\n");
  printf("\n");
  printf("-------------------------------------\n");
  
  /* Calculate integrals */
  integ_x1 = hx1->Integral(start_integ, stop_integ);
  integ_u1 = hu1->Integral(start_integ, stop_integ);
  integ_x2 = hx2->Integral(start_integ, stop_integ);
  integ_u2 = hu2->Integral(start_integ, stop_integ);

  int nbin_x1 = hx1->GetNbinsX();
  int nbin_u1 = hu1->GetNbinsX();
  int nbin_x2 = hx2->GetNbinsX();
  int nbin_u2 = hu2->GetNbinsX();

  ofstream ofs_x1("calib/vdccalib_x1.dat", ios::out);
  ofstream ofs_u1("calib/vdccalib_u1.dat", ios::out);
  ofstream ofs_x2("calib/vdccalib_x2.dat", ios::out);
  ofstream ofs_u2("calib/vdccalib_u2.dat", ios::out);

  float bin_center;
  float drift_length;

  /* x1 */
  int tmp_sum=0;
  for(int i=1; i<nbin_x1; i++){
    tmp_sum += hx1->GetBinContent(i);
    bin_center = hx1->GetBinCenter(i);
    drift_length = DRIFT_GAP*tmp_sum/(float)integ_x1;
    if(drift_length>DRIFT_GAP) drift_length = DRIFT_GAP;
    if(bin_center<tmin) drift_length=0.0;
    if(bin_center>tmax) drift_length=10.0;
    
    ofs_x1 << bin_center << " " << drift_length << endl;
  }

  /* u1 */
  int tmp_sum=0;
  for(int i=1; i<nbin_u1; i++){
    tmp_sum += hu1->GetBinContent(i);
    bin_center = hu1->GetBinCenter(i);
    drift_length = DRIFT_GAP*tmp_sum/(float)integ_u1;
    if(drift_length>DRIFT_GAP) drift_length = DRIFT_GAP;
    if(bin_center<tmin) drift_length=0.0;
    if(bin_center>tmax) drift_length=10.0;
    
    ofs_u1 << bin_center << " " << drift_length << endl;
  }

  /* x2 */
  int tmp_sum=0;
  for(int i=1; i<nbin_x2; i++){
    tmp_sum += hx2->GetBinContent(i);
    bin_center = hx2->GetBinCenter(i);
    drift_length = DRIFT_GAP*tmp_sum/(float)integ_x2;
    if(drift_length>DRIFT_GAP) drift_length = DRIFT_GAP;
    if(bin_center<tmin) drift_length=0.0;
    if(bin_center>tmax) drift_length=10.0;
    
    ofs_x2 << bin_center << " " << drift_length << endl;
  }

  /* u2 */
  int tmp_sum=0;
  for(int i=1; i<nbin_u2; i++){
    tmp_sum += hu2->GetBinContent(i);
    bin_center = hu2->GetBinCenter(i);
    drift_length = DRIFT_GAP*tmp_sum/(float)integ_u2;
    if(drift_length>DRIFT_GAP) drift_length = DRIFT_GAP;
    if(bin_center<tmin) drift_length=0.0;
    if(bin_center>tmax) drift_length=10.0;
    
    ofs_u2 << bin_center << " " << drift_length << endl;
  }
  
  ofs_x1.close();
  ofs_u1.close();
  ofs_x2.close();
  ofs_u2.close();
	
  return 1;
}
