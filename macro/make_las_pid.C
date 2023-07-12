{
  string pidfname = "macro/laspid.root";

  TCutG *laspid;
  //  laspid = (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG"); // for ROOT v5
  laspid = (TCutG*)gROOT->FindObject("CUTG"); // for ROOT v6

  if(laspid==NULL){
    printf("No CUTG found!\n");
    printf("First, make CUTG object by View-->Toolbar-->Graphical Cut\n");
  }

  if(laspid!=NULL){
    laspid->SetName("laspidcut");
    TFile *file_laspid = new TFile(pidfname.c_str(), "RECREATE");
    laspid->Write();
    file_laspid->Close();
    printf("Saved PID file: %s\n", pidfname.c_str());

  }
}
