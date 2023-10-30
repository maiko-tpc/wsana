{

  char filename[128];
  char projcmd[128];
  char clonecmd[128];
  char Drawcmd[128];
  char SetRangecmd[128];
  char GetMeancmd[128];

  char nbinscmd[128];
  
  Int_t counter,counter2;
  Int_t n;

  ofstream myfile;
  
  int peak[80];
  
  for(Int_t i=0;i<16;i++)    {
      //sprintf(projcmd,"h->ProjectionX(\"hsakra_%d\",%d,%d,\"\");",i,i,i);
      //cout<<projcmd<<endl;
      //gROOT->ProcessLine(projcmd);
      
      h->ProjectionX("hsakra",i,i,"");
      hsakra->Draw();
      cout<<hsakra->GetMean();

      //sprintf(Drawcmd,"hsakra_%d->Draw(\"\")",i);
      //cout<<Drawcmd<<endl;
      //gROOT->ProcessLine(Drawcmd);

      //sprintf(SetRangecmd,"hsakra_%d->GetXaxis()->SetRange(3900,3950)",i);
      //cout<<SetRangecmd<<endl;
      //gROOT->ProcessLine(SetRangecmd);

      //sprintf(GetMeancmd,"cout<<%d<<\" \"<<-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      //gROOT->ProcessLine(GetMeancmd);

      //sprintf(GetMeancmd,"peak[%d]=-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      //gROOT->ProcessLine(GetMeancmd);

      //cout<<endl;

  }
/*
  for(Int_t i=16;i<32;i++)    {
      sprintf(projcmd,"h->ProjectionX(\"hsakra_%d\",%d,%d,\"\");",i,i,i);
      //cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Drawcmd,"hsakra_%d->Draw(\"\")",i);
      //cout<<Drawcmd<<endl;
      gROOT->ProcessLine(Drawcmd);

      sprintf(SetRangecmd,"hsakra_%d->GetXaxis()->SetRange(3910,3970)",i);
      //cout<<SetRangecmd<<endl;
      gROOT->ProcessLine(SetRangecmd);

      sprintf(GetMeancmd,"cout<<%d<<\" \"<<-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      sprintf(GetMeancmd,"peak[%d]=-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      cout<<endl;

  }

  for(Int_t i=32;i<48;i++)    {
      sprintf(projcmd,"h->ProjectionX(\"hsakra_%d\",%d,%d,\"\");",i,i,i);
      //cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Drawcmd,"hsakra_%d->Draw(\"\")",i);
      //cout<<Drawcmd<<endl;
      gROOT->ProcessLine(Drawcmd);

      sprintf(SetRangecmd,"hsakra_%d->GetXaxis()->SetRange(3930,4000)",i);
      //cout<<SetRangecmd<<endl;
      gROOT->ProcessLine(SetRangecmd);

      sprintf(GetMeancmd,"cout<<%d<<\" \"<<-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      sprintf(GetMeancmd,"peak[%d]=-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      cout<<endl;

  }

  for(Int_t i=48;i<64;i++)    {
      sprintf(projcmd,"h->ProjectionX(\"hsakra_%d\",%d,%d,\"\");",i,i,i);
      //cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Drawcmd,"hsakra_%d->Draw(\"\")",i);
      //cout<<Drawcmd<<endl;
      gROOT->ProcessLine(Drawcmd);

      sprintf(SetRangecmd,"hsakra_%d->GetXaxis()->SetRange(3920,3970)",i);
      //cout<<SetRangecmd<<endl;
      gROOT->ProcessLine(SetRangecmd);

      sprintf(GetMeancmd,"cout<<%d<<\" \"<<-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      sprintf(GetMeancmd,"peak[%d]=-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      cout<<endl;

  }


  for(Int_t i=64;i<80;i++)    {
      sprintf(projcmd,"h->ProjectionX(\"hsakra_%d\",%d,%d,\"\");",i,i,i);
      //cout<<projcmd<<endl;
      gROOT->ProcessLine(projcmd);

      sprintf(Drawcmd,"hsakra_%d->Draw(\"\")",i);
      //cout<<Drawcmd<<endl;
      gROOT->ProcessLine(Drawcmd);

      sprintf(SetRangecmd,"hsakra_%d->GetXaxis()->SetRange(3960,4020)",i);
      //cout<<SetRangecmd<<endl;
      gROOT->ProcessLine(SetRangecmd);

      sprintf(GetMeancmd,"cout<<%d<<\" \"<<-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      sprintf(GetMeancmd,"peak[%d]=-600-hsakra_%d->GetMean()",i,i);
      //cout<<i<<"   ";
      gROOT->ProcessLine(GetMeancmd);

      cout<<endl;

  }


  ofstream out;
  out.open("offsets-tmp.dat");

  for(Int_t i=0;i<80;i++)    {
  
      out<<i<<"  "<<peak[i]<<endl;

  }
  out.close();

*/
  

}
  
  

