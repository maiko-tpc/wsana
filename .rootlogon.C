{
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);

  
  // read the GR PID file
  string fname_grpid = "macro/grpid.root";
  TFile *fgrpid = new TFile(fname_grpid.c_str());
  if(fgrpid->IsOpen() == false) printf("No GR PID file found: %s\n", fname_grpid.c_str());

  if(fgrpid->IsOpen() == true){
    TCutG *grpid = (TCutG*)fgrpid->Get("grpidcut");
    grpidcut->SetLineColor(2);
    grpidcut->SetLineWidth(2);    
    printf("\n------------\n");
    printf("GR PID cut name: %s\n", grpid->GetName());
    printf("PID Par X: %s\n", grpidcut->GetVarX());
    printf("PID Par Y: %s\n", grpidcut->GetVarY());    
    printf("------------\n\n");

  }
}
