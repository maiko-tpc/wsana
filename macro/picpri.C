{
  c1->SaveAs("macro/pic/tmp.png");
  system("lpr -Prcc4000a macro/pic/tmp.png -o scaling=50");
}
