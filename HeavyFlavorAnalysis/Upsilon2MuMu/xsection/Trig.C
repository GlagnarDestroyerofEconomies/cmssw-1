#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void TrigCheckVsfTrigmmb(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1SBin.default.root");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *TrigCheck_Numa_1S;
  TrigCheck_Numa_1S = (TH2D*)gFile->Get("TrigCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Numa_1S->Clone(); 
  Ratio->SetName("TrigCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Numa_1S->Clone();   
  Diff->SetName("TrigCheck - fTrigmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Numa_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Numa_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Numa_1S->GetCellContent(i,j);
      bin_contentAfter = TrigCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_contentBefore << endl;
      cout << "bin_contentNuma = " << bin_contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("Trig.root");
  TH2D *fTrigEff;
  fTrigEff = (TH2D*)gFile->Get("fTrigEff");
  
  TH1D *Diff_1D = new TH1D("Diff_1D","Ratio:#rho^{TNP}", 75,0.9,1.2);
  
  double trigcheck(-99); double trigeff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      trigcheck = Ratio->GetCellContent(i,j);
      trigeff = fTrigEff->GetCellContent(i,j);
      
      diff = trigcheck - trigeff;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << trigcheck/trigeff << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,trigcheck/trigeff);
      Diff_1D->Fill(trigcheck/trigeff);
      trigcheck=0;
      trigeff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fTrigEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
  
  gStyle->SetOptStat(00000000000);
  TCanvas *c2 = new TCanvas("c2","c2",900,600);
  fTrigEff->SetTitle("Di-muon Efficiency with Tag and Probe Method");
  fTrigEff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  fTrigEff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  fTrigEff->Draw("colz");
  c2->SaveAs("TrigTNP.pdf");
  
  TCanvas *c3 = new TCanvas("c3","c3",900,600);
  Ratio->SetTitle("Di-muon Efficiency with MC Simulation");
  Ratio->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  Ratio->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  Ratio->Draw("colz"); 
  c3->SaveAs("TrigMCSim.pdf");
  
  TCanvas *c4 = new TCanvas("c4","c4",900,600);
  Diff->SetTitle("Ratio: #rho^{TNP}");
  Diff->GetYaxis()->SetTitle("p_{T}^{#Upsilon} [GeV/c]");
  Diff->GetXaxis()->SetTitle("|y^{#Upsilon}|");
  Diff->Draw("colz"); 
  c4->SaveAs("Trigrho.pdf");
  
  TCanvas *c5 = new TCanvas("c5","c5",900,600);
  Diff_1D->SetTitle("Ratio:#rho^{TNP}");
  Diff_1D->GetXaxis()->SetTitle("Ratio: #rho^{TNP}");
  gStyle->SetOptStat(1111111111);
  Diff_1D->Draw(); 
  c5->SaveAs("Trigrho_1D.pdf"); 
  
}

void TrigCheckVsfTrig_DATA(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *TrigCheck_Numa_1S;
  TrigCheck_Numa_1S = (TH2D*)gFile->Get("TrigCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Numa_1S->Clone(); 
  Ratio->SetName("TrigCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Numa_1S->Clone();   
  Diff->SetName("TrigCheck - fTrigmmb_DATA");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Numa_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Numa_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Numa_1S->GetCellContent(i,j);
      bin_contentAfter = TrigCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_contentBefore << endl;
      cout << "bin_contentNuma = " << bin_contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("Trig_DATA.root");
  TH2D *fTrigEff;
  fTrigEff = (TH2D*)gFile->Get("fTrigEff");
  
  double trigcheck(-99); double trigeff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      trigcheck = Ratio->GetCellContent(i,j);
      trigeff = fTrigEff->GetCellContent(i,j);
      
      diff = trigcheck - trigeff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      trigcheck=0;
      trigeff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fTrigEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}
