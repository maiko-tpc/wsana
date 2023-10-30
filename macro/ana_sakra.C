void ana_sakra(){
  int eve;
  tree->SetBranchAddress("eve", &eve);
  int nEntry = tree->GetEntries();

  for(int i=0; i<nEntry; i++){
    tree->GetEntry(i);    
  }
  
}
