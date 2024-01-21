{
  double a1=tree->GetEntries("grpidcut && good_fit_plane[1]==1 && good_fit_plane[2]==1 && good_fit_plane[3]==1");
  double a2=tree->GetEntries("grpidcut && good_fit_plane[0]==1 && good_fit_plane[1]==1 && good_fit_plane[2]==1 && good_fit_plane[3]==1");  

  double b1=tree->GetEntries("grpidcut && good_fit_plane[0]==1 && good_fit_plane[2]==1 && good_fit_plane[3]==1");
  double b2=tree->GetEntries("grpidcut && good_fit_plane[1]==1 && good_fit_plane[0]==1 && good_fit_plane[2]==1 && good_fit_plane[3]==1");  

  double c1=tree->GetEntries("grpidcut && good_fit_plane[0]==1 && good_fit_plane[1]==1 && good_fit_plane[3]==1");
  double c2=tree->GetEntries("grpidcut && good_fit_plane[2]==1 && good_fit_plane[0]==1 && good_fit_plane[1]==1 && good_fit_plane[3]==1");  

  double d1=tree->GetEntries("grpidcut && good_fit_plane[0]==1 && good_fit_plane[1]==1 && good_fit_plane[2]==1");
  double d2=tree->GetEntries("grpidcut && good_fit_plane[3]==1 && good_fit_plane[0]==1 && good_fit_plane[1]==1 && good_fit_plane[2]==1");  
  
  double eff0 = a2/a1;
  double eff1 = b2/b1;
  double eff2 = c2/c1;
  double eff3 = d2/d1;  

  double eff_multi = eff0*eff1*eff2*eff3;

  printf("X1 eff=%f\n", eff0);
  printf("U1 eff=%f\n", eff1);
  printf("X2 eff=%f\n", eff2);
  printf("U2 eff=%f\n", eff3);  

  printf("total eff: %f\n", eff_multi);
  
}
