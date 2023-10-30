{
  // to do
  // loop 160 to 191

  char filename[128];
  char projcmd[128];
  char clonecmd[128];
  char Savecmd[128];
  char nbinscmd[128];
  
  Int_t counter,counter2;
  Int_t n;

  ofstream myfile;
  
  
  for(Int_t i=11;i<19;i++)    {
 
      counter=i-10;	  
      //cout<<counter<<endl;

      sprintf(projcmd,"h1->ProjectionY(\"hLaBr_S_HiG_%d\",%d,%d,\"\");",counter,i,i);
      cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Savecmd,"hLaBr_S_HiG_%d->SaveAs(\"LaBr%d-S-HiG-run9225.C\")",counter,counter);
      cout<<Savecmd<<endl;
      gROOT->ProcessLine(Savecmd);

  }


  for(Int_t i=19;i<27;i++)    {

      counter2=i-18;	  
      //cout<<counter2<<endl;

      sprintf(projcmd,"h1->ProjectionY(\"hLaBr_S_LoG_%d\",%d,%d,\"\");",counter2,i,i);
      cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Savecmd,"hLaBr_S_LoG_%d->SaveAs(\"LaBr%d-S-LoG-run9225.C\")",counter2,counter2);
      cout<<Savecmd<<endl;
      gROOT->ProcessLine(Savecmd);

  }
  

}
  
  

