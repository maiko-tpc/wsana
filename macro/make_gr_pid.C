{
  string pidfname = "macro/grpid.root";

  TCutG *grpid;
  //  grpid = (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG"); // for ROOT v5
  grpid = (TCutG*)gROOT->FindObject("CUTG"); // for ROOT v6

  if(grpid==NULL){
    printf("No CUTG found!\n");
    printf("First, make CUTG object by View-->Toolbar-->Graphical Cut\n");
  }

  if(grpid!=NULL){
    grpid->SetName("grpidcut");
    TFile *file_grpid = new TFile(pidfname.c_str(), "RECREATE");
    grpid->Write();
    file_grpid->Close();
    printf("Saved PID file: %s\n", pidfname.c_str());

  }
}
