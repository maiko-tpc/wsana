void hist2ascii (TH1* h1)
{
   Int_t n = h1->GetNbinsX();

   ofstream myfile;
   myfile.open("histo.txt");
  	   
   for (Int_t i=1; i<=n; i++) {
      //printf("%g %g\n",h->GetBinLowEdge(i)+h->GetBinWidth(i)/2, h->GetBinContent(i) );
      myfile << h1->GetBinLowEdge(i)+h1->GetBinWidth(i)/2 << " " << h1->GetBinContent(i)<<endl;
      //cout << h->GetBinLowEdge(i)+h->GetBinWidth(i)/2 << " " << h->GetBinContent(i)<<endl;
   }

   myfile.close();
   
}
