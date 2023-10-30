{
  // to do
  // loop 160 to 191

  char filename[128];
  char treecmd[128];
  char clonecmd[128];
  char Savecmd[128];
  char nbinscmd[128];
  
  Int_t counter;
  Int_t n;

  ofstream myfile;
  
  
  for(Int_t i=160;i<192;i++)    {
      //sprintf(filename,"/home/pandora/ana/furuno/labr/madc-chan%3d-run9132.txt",i);
      //cout<<filename<<endl;

      counter=1+(i-160)%8;
      //cout<<counter<<endl;

      if(i<168)
	  {
	    sprintf(treecmd,"tree->Draw(\"madc.adc[%3d]>>h%ds(8000,0,8000)\",\"\",\"\")",i,counter);
	    cout<<treecmd<<endl;
            gROOT->ProcessLine(treecmd);

            sprintf(nbinscmd,"n = h%ds->GetNbinsX()",counter);
	    cout<<nbinscmd<<endl;
            gROOT->ProcessLine(nbinscmd);

            //sprintf(clonecmd," TH1F *h= (TH1F*)h%ds->Clone(\"h\")",counter);
	    //cout<<clonecmd<<endl;
            //gROOT->ProcessLine(clonecmd);

            sprintf(Savecmd,"h%ds->SaveAs(\"/home/pandora/ana/furuno/labr/tst/madc-chan%3d-run9132.C\")",counter,i);
            cout<<Savecmd<<endl;
            gROOT->ProcessLine(Savecmd);
	    
	  }
      
      if(i>167 && i< (167+8+1))
	  {
	    sprintf(treecmd,"tree->Draw(\"madc.adc[%3d]>>h%ds2(8000,0,8000)\",\"\",\"\")",i,counter);
	    cout<<treecmd<<endl;
            gROOT->ProcessLine(treecmd);

            sprintf(nbinscmd,"n = h%ds2->GetNbinsX()",counter);
	    cout<<nbinscmd<<endl;
            gROOT->ProcessLine(nbinscmd);

            //sprintf(clonecmd," TH1F *h= (TH1F*)h%ds2->Clone(\"h\")",counter);
	    //cout<<clonecmd<<endl;
            //gROOT->ProcessLine(clonecmd);
	    
            sprintf(Savecmd,"h%ds2->SaveAs(\"/home/pandora/ana/furuno/labr/tst/madc-chan%3d-run9132.C\")",counter,i);
            cout<<Savecmd<<endl;
            gROOT->ProcessLine(Savecmd);

	  }
      if(i>175 && i< (175+8+1))
	  {
	    sprintf(treecmd,"tree->Draw(\"madc.adc[%3d]>>h%df(8000,0,8000)\",\"\",\"\")",i,counter);
	    cout<<treecmd<<endl;
            gROOT->ProcessLine(treecmd);

            sprintf(nbinscmd,"n = h%df->GetNbinsX()",counter);
	    cout<<nbinscmd<<endl;
            gROOT->ProcessLine(nbinscmd);

            //sprintf(clonecmd," TH1F *h= (TH1F*)h%df->Clone(\"h\")",counter);
	    //cout<<clonecmd<<endl;
            //gROOT->ProcessLine(clonecmd);
	    
	    sprintf(Savecmd,"h%df->SaveAs(\"/home/pandora/ana/furuno/labr/tst/madc-chan%3d-run9132.C\")",counter,i);
            cout<<Savecmd<<endl;
            gROOT->ProcessLine(Savecmd);
	  }
      if(i>183 && i< (183+8+1))   
	  {
	    sprintf(treecmd,"tree->Draw(\"madc.adc[%3d]>>h%df2(8000,0,8000)\",\"\",\"\")",i,counter);
	    cout<<treecmd<<endl;
            gROOT->ProcessLine(treecmd);

            sprintf(nbinscmd,"n = h%df2->GetNbinsX()",counter);
	    cout<<nbinscmd<<endl;
	    gROOT->ProcessLine(nbinscmd);

            //sprintf(clonecmd," TH1F *h= (TH1F*)h%df2->Clone(\"h\")",counter);
	    //cout<<clonecmd<<endl;
            //gROOT->ProcessLine(clonecmd);
	    
            sprintf(Savecmd,"h%df2->SaveAs(\"/home/pandora/ana/furuno/labr/tst/madc-chan%3d-run9132.C\")",counter,i);
            cout<<Savecmd<<endl;
            gROOT->ProcessLine(Savecmd);
	  }

      /*
      myfile.open(filename);
      for (Int_t ii=1; ii<=n; ii++) {
         //printf("%g %g\n",h->GetBinLowEdge(ii)+h->GetBinWidth(ii)/2, h->GetBinContent(ii) );
         myfile << h->GetBinLowEdge(ii)+h->GetBinWidth(ii)/2 << " " << h->GetBinContent(ii)<<endl;
         //cout << h->GetBinLowEdge(ii)+h->GetBinWidth(ii)/2 << " " << h->GetBinContent(ii)<<endl;
      }

      myfile.close();
      h->Delete();
      */
  }
  

}
  
  

