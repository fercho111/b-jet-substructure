#define bjetEventLoop_cxx
#include "bjetEventLoop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TVector2.h>
#include <iostream>
#include <fstream>
#include <string>

void bjetEventLoop::Loop()
{
//   In a ROOT session, you can do:
//      root> .L bjetEventLoop.C
//      root> bjetEventLoop t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

   if (fChain == 0) return;

   TFile *output = new TFile("output.root", "RECREATE");

   // histograms
   // TODO add axis titles
   TH1F *h_jet_pt = new TH1F("h_jet_pt", "Jet transverse momentum", 100, 0.0, 200.0);
   TH1F *h_jet_eta = new TH1F("h_jet_eta", "Jet pseudorapidity", 100, -5.0, 5.0);
   TH1F *h_jet_phi = new TH1F("h_jet_phi", "Jet phi", 100, -TMath::Pi(), TMath::Pi());
   
   TH1F *h_true_jet_pt = new TH1F("h_true_jet_pT", "True Jet transverse momentum", 100, 0.0, 200.0);
   TH1F *h_true_jet_eta = new TH1F("h_true_jet_eta", "True Jet pseudorapidity", 100, -5.0, 5.0);
   TH1F *h_true_jet_phi = new TH1F("h_true_jet_phi", "True Jet phi", 100, -TMath::Pi(), TMath::Pi());

   TH1F *h_delta_r = new TH1F("h_delta_r", "Delta R", 100, 0.0, 10.0);
   TH1F *h_delta_r_matched_pairs = new TH1F("h_delta_r_matched_pairs", "Delta R matched pairs", 100, 0.0, 50.0);

   h_jet_pt->SetOption("HIST");
   h_jet_eta->SetOption("HIST");
   h_jet_phi->SetOption("HIST");

   h_true_jet_pt->SetOption("HIST");
   h_true_jet_eta->SetOption("HIST");
   h_true_jet_phi->SetOption("HIST");

   h_delta_r->SetOption("HIST");
   h_delta_r_matched_pairs->SetOption("HIST");

   // Get the total number of events in the tree/chain
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;

      for (int j = 0; j < truth_jet_pt->size(); j++)
      {
         for (int i = 0; i < jet_pt->size(); i++)
         {
            double deta = jet_eta->at(i) - truth_jet_eta->at(j);
            // double check ?
            double dphi = TVector2::Phi_mpi_pi(jet_phi->at(i) - truth_jet_phi->at(j));
            double dr = sqrt(deta*deta + dphi*dphi);
            h_delta_r->Fill(dr, MC_weight);
            if (dr < 0.3)
            {
               // if jet is matched to b flavour
               h_jet_pt->Fill(jet_pt->at(i), MC_weight);
               h_jet_eta->Fill(jet_eta->at(i), MC_weight);
               h_jet_phi->Fill(jet_phi->at(i), MC_weight);
               h_delta_r_matched_pairs->Fill(dr,MC_weight);
            }
         }
         h_true_jet_pt->Fill(truth_jet_pt->at(j), MC_weight);
         h_true_jet_eta->Fill(truth_jet_eta->at(j), MC_weight);
         h_true_jet_phi->Fill(truth_jet_phi->at(j), MC_weight);
      }
   }
   output->Write();
   output->Close();
}

int main(int argc, char** argv)
{
   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " file_list.txt" << std::endl;
      return 1;
   }

   std::ifstream fileList(argv[1]);
   if (!fileList.is_open())
   {
      std:: cerr << "File could not be opened" << std::endl;
      return 1;
   }

   // we will need to use TChain
   TChain *chain = new TChain("AntiKt4HI");

   std::string filename;
   while(std::getline(fileList, filename))
   {
      chain->Add(filename.c_str());
   }

   fileList.close();

   bjetEventLoop analysis(chain);   // pass chain to the constructor
   analysis.Loop();

   return 0;
   // todo: plot output.root 
}