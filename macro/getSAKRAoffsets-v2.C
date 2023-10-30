{
  char name[128];
  char text[128];

  const int sakrachan=80;  
  Double_t fitgauslow, fitgaushi, rangelow, rangehi;
  fitgauslow=-200;  // to define range where gauss fitting is done
  fitgaushi=0;   // to define range where gauss fitting is done

  Double_t par[5];
  Double_t peakposition[sakrachan];
  Double_t sigma[sakrachan];
  Double_t strength[sakrachan];
  int offset[sakrachan];

  ofstream out,out2;
  out.open("offsets-tmp.dat");
  //out2.open("offsets-tmp2.dat");

  //for(Int_t i=0;i<sakrachan;i++)	{
  for(Int_t i=0;i<80;i++)	{
          //  h is a 2D histogram created with 
          // tree->Draw("Iteration$:sakratof>>h(5000,-40000,10000,128,0,128)",grgates && rf0peak1,"colz")
          // the 1D histogram will be defined by the size of the x axis of the 2D histo 
          h->ProjectionX("hSKRtime",i,i,"");
          hSKRtime->Draw();

          //out2<<hSKRtime->GetMean()<<endl;

  	  //TH1F *SKRtime = new TH1F("SKRtime","sakra time",130,-1000,300);
          //tree->Draw("sakratof>>SKRtime",grgates && rf0peak1 && "Iteration$==i","colz");
 
          if(i<6){ fitgauslow = -800; fitgaushi =-500; } 
          if(i>5  && i<16){ fitgauslow = -900; fitgaushi =-700; } 
          if(i>15 && i<25){ fitgauslow = -600; fitgaushi =-400; } 
          if(i>24 && i<32){ fitgauslow = -700; fitgaushi =-500; } 
          if(i>31 && i<44){ fitgauslow = -400; fitgaushi =-200; } 
          if(i>43 && i<48){ fitgauslow = -800; fitgaushi =-500; } 
          if(i>47 && i<64){ fitgauslow = -700; fitgaushi =-400; } 
          if(i>63 && i<80){ fitgauslow = -400; fitgaushi =-100; } 

          //fitgauslow = -1200; fitgaushi =-200;

	  hSKRtime->Fit("gaus","QR","",fitgauslow,fitgaushi);  		
	  gaus->GetParameters(par);
          strength[i] = par[0];
          peakposition[i] = par[1];
          sigma[i] = par[2];
	  //c1->Modified(); c1->Update();
	  //c1->WaitPrimitive();
 
          cout<<i<<"  "<< peakposition[i] <<endl;
          out<<i<<"  "<<  -700 - int(peakposition[i]) <<endl;

  }

  out.close();
  //out2.close();


}
