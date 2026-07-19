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

   // discriminant variables
   const double f_c = 0.2;
   const double f_tau = 0.01;

   // Jets as reconstructed by the detector in the MC simulation
   TH1F *h_jet_pt = new TH1F("h_jet_pt", "Reconstructed Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_jet_eta = new TH1F("h_jet_eta", "Reconstructed Jet #eta;#eta;Jets", 100, -5.0, 5.0); // Jets as reconstructed by the detector in the MC simulation
   TH1F *h_jet_phi = new TH1F("h_jet_phi", "Reconstructed Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // Jets as reconstructed by the detector in the MC simulation

   // MC simulation jets (all)
   TH1F *h_truth_jet_pt = new TH1F("h_truth_jet_pT", "Truth Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_truth_jet_eta = new TH1F("h_truth_jet_eta", "Truth Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation jets (all)
   TH1F *h_truth_jet_phi = new TH1F("h_truth_jet_phi", "Truth Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation jets (all)

   // MC simulation bjets
   TH1F *h_truth_bjet_pt = new TH1F("h_truth_bjet_pT", "Truth b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_truth_bjet_eta = new TH1F("h_truth_bjet_eta", "Truth b-Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation bjets
   TH1F *h_truth_bjet_phi = new TH1F("h_truth_bjet_phi", "Truth b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation bjets

   // MC simulation jets - bjets
   // TODO: figure out if they're removable?
   TH1F *h_truth_not_bjet_pt = new TH1F("h_truth_not_bjet_pT", "Truth non-b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0); 
   TH1F *h_truth_not_bjet_eta = new TH1F("h_truth_not_bjet_eta", "Truth non-b-Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation jets - bjets
   TH1F *h_truth_not_bjet_phi = new TH1F("h_truth_not_bjet_phi", "Truth non-b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation jets - bjets

   // delta R's 
   TH1F *h_delta_r = new TH1F("h_delta_r", "#DeltaR (reco, truth);#DeltaR;Pairs", 100, 0.0, 10.0);
   // delta R's for matched jets only
   TH1F *h_delta_r_matched_pairs = new TH1F("h_delta_r_matched_pairs", "#DeltaR matched pairs;#DeltaR;Pairs", 100, 0.0, 10.0);


   // pT for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_matched_bjet_pt = new TH1F("h_matched_bjet_pT", "Matched reco b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // η for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_matched_bjet_eta = new TH1F("h_matched_bjet_eta", "Matched reco b-Jet #eta;#eta;Jets", 100, -5.0, 5.0);
   // φ for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_matched_bjet_phi = new TH1F("h_matched_bjet_phi", "Matched reco b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_matched_not_bjet_pt = new TH1F("h_matched_not_bjet_pT", "Matched reco non-b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // η for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_matched_not_bjet_eta = new TH1F("h_matched_not_bjet_eta", "Matched reco non-b-Jet #eta;#eta;Jets", 100, -5.0, 5.0);
   // φ for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_matched_not_bjet_phi = new TH1F("h_matched_not_bjet_phi", "Matched reco non-b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // D_b for reconstructed jets whose true jet flavour (jet_HadronConeExclTruthLabelID == 5) was bottom quark
   TH1F *h_discriminant_b_bjet = new TH1F("h_discriminant_b_bjet", "GN discriminant D_{b} b-jets",100, -10.0, 20.0);
   // D_b for reconstructed jets whose true jet flavour (jet_HadronConeExclTruthLabelID != 5) was NOT bottom quark
   TH1F *h_discriminant_b_not_bjet = new TH1F("h_discriminant_b_not_bjet", "GN discriminant D_{b} non b-jets",100, -10.0, 20.0);

   // pT for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_matched_reco_passing_Db_cut_pt = new TH1F("h_matched_reco_passing_Db_cut_pt", "Matched reco jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_matched_reco_passing_Db_cut_eta = new TH1F("h_matched_reco_passing_Db_cut_eta", "Matched reco jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_matched_reco_passing_Db_cut_phi = new TH1F("h_matched_reco_passing_Db_cut_phi", "Matched reco jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_matched_reco_true_b_pt = new TH1F("h_matched_reco_true_b_pt", "Matched reco jets (true b);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_matched_reco_true_b_eta = new TH1F("h_matched_reco_true_b_eta", "Matched reco jets (true b);#eta;Jets", 100, -5.0, 5.0);
   // phi for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_matched_reco_true_b_phi = new TH1F("h_matched_reco_true_b_phi", "Matched reco jets (true b);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_matched_reco_true_b_passing_Db_cut_pt = new TH1F("h_matched_reco_true_b_passing_Db_cut_pt", "True b-jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_matched_reco_true_b_passing_Db_cut_eta = new TH1F("h_matched_reco_true_b_passing_Db_cut_eta", "True b-jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_matched_reco_true_b_passing_Db_cut_phi = new TH1F("h_matched_reco_true_b_passing_Db_cut_phi", "True b-jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());
   // pT for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_matched_reco_true_nonb_passing_Db_cut_pt = new TH1F("h_matched_reco_true_nonb_passing_Db_cut_pt", "True non-b-jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_matched_reco_true_nonb_passing_Db_cut_eta = new TH1F("h_matched_reco_true_nonb_passing_Db_cut_eta", "True non-b-jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_matched_reco_true_nonb_passing_Db_cut_phi = new TH1F("h_matched_reco_true_nonb_passing_Db_cut_phi", "True non-b-jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());


   /* set histogram option*/
   h_jet_pt->SetOption("HIST");
   h_jet_eta->SetOption("HIST");
   h_jet_phi->SetOption("HIST");

   h_truth_jet_pt->SetOption("HIST");
   h_truth_jet_eta->SetOption("HIST");
   h_truth_jet_phi->SetOption("HIST");

   h_truth_bjet_pt->SetOption("HIST");
   h_truth_bjet_eta->SetOption("HIST");
   h_truth_bjet_phi->SetOption("HIST");

   h_truth_not_bjet_pt->SetOption("HIST");
   h_truth_not_bjet_eta->SetOption("HIST");
   h_truth_not_bjet_phi->SetOption("HIST");

   h_delta_r->SetOption("HIST");
   h_delta_r_matched_pairs->SetOption("HIST");

   h_matched_bjet_pt->SetOption("HIST");
   h_matched_bjet_eta->SetOption("HIST");
   h_matched_bjet_phi->SetOption("HIST");

   h_matched_not_bjet_pt->SetOption("HIST");
   h_matched_not_bjet_eta->SetOption("HIST");
   h_matched_not_bjet_phi->SetOption("HIST");

   h_discriminant_b_bjet->SetOption("HIST");
   h_discriminant_b_not_bjet->SetOption("HIST");

   h_matched_reco_passing_Db_cut_pt->SetOption("HIST");
   h_matched_reco_passing_Db_cut_eta->SetOption("HIST");
   h_matched_reco_passing_Db_cut_phi->SetOption("HIST");

   h_matched_reco_true_b_pt->SetOption("HIST");
   h_matched_reco_true_b_eta->SetOption("HIST");
   h_matched_reco_true_b_phi->SetOption("HIST");


   h_matched_reco_true_b_passing_Db_cut_pt->SetOption("HIST");
   h_matched_reco_true_b_passing_Db_cut_eta->SetOption("HIST");
   h_matched_reco_true_b_passing_Db_cut_phi->SetOption("HIST");
   h_matched_reco_true_nonb_passing_Db_cut_pt->SetOption("HIST");
   h_matched_reco_true_nonb_passing_Db_cut_eta->SetOption("HIST");
   h_matched_reco_true_nonb_passing_Db_cut_phi->SetOption("HIST");
   /* end set histogram option*/

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
            double dphi = fabs(TVector2::Phi_mpi_pi(jet_phi->at(i) - truth_jet_phi->at(j)));
            double dr = sqrt(deta*deta + dphi*dphi);
            h_delta_r->Fill(dr, MC_weight);
            // TODO: check if there is more than one reco jet matched to truth
            if (dr < 0.3)
            {
               // fill inclusive matched reco jet kinematics
               h_jet_pt->Fill(jet_pt->at(i), MC_weight);
               h_jet_eta->Fill(jet_eta->at(i), MC_weight);
               h_jet_phi->Fill(jet_phi->at(i), MC_weight);
               h_delta_r_matched_pairs->Fill(dr, MC_weight);

               // split matched reco jets by truth jet flavor (from truth_jet_flavor)
               if (truth_jet_flavor->at(j) == 5) {
                  h_matched_bjet_pt->Fill(jet_pt->at(i), MC_weight);
                  h_matched_bjet_eta->Fill(jet_eta->at(i), MC_weight);
                  h_matched_bjet_phi->Fill(jet_phi->at(i), MC_weight);
               } else {
                  h_matched_not_bjet_pt->Fill(jet_pt->at(i), MC_weight);
                  h_matched_not_bjet_eta->Fill(jet_eta->at(i), MC_weight);
                  h_matched_not_bjet_phi->Fill(jet_phi->at(i), MC_weight);
               }

               // compute D_b discriminant for this reco jet
               double D_b = log(jet_GN2v01_pb->at(i) / (f_c * jet_GN2v01_pc->at(i) + f_tau * jet_GN2v01_ptau->at(i) + (1 - f_c - f_tau) * jet_GN2v01_pu->at(i)));

               // fill all matched reco jets passing D_b cut (any flavor)
               if (D_b > 0)
               {
                  h_matched_reco_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                  h_matched_reco_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                  h_matched_reco_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
               }

               // split by reco-level truth label (HadronConeExcl) for D_b studies
               if (jet_HadronConeExclTruthLabelID->at(i) == 5)
               {
                  h_matched_reco_true_b_pt->Fill(jet_pt->at(i), MC_weight);
                  h_matched_reco_true_b_eta->Fill(jet_eta->at(i), MC_weight);
                  h_matched_reco_true_b_phi->Fill(jet_phi->at(i), MC_weight);
                  h_discriminant_b_bjet->Fill(D_b, MC_weight);
                  // we propose Db cut on Db==0
                  if (D_b > 0)
                  {
                     h_matched_reco_true_b_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                     h_matched_reco_true_b_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                     h_matched_reco_true_b_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                  }
               }
               else
               {
                  h_discriminant_b_not_bjet->Fill(D_b, MC_weight);
                  if (D_b > 0)
                  {
                     h_matched_reco_true_nonb_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                     h_matched_reco_true_nonb_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                     h_matched_reco_true_nonb_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                  }
               }
            }
         }
         h_truth_jet_pt->Fill(truth_jet_pt->at(j), MC_weight);
         h_truth_jet_eta->Fill(truth_jet_eta->at(j), MC_weight);
         h_truth_jet_phi->Fill(truth_jet_phi->at(j), MC_weight);

         if (truth_jet_flavor->at(j) == 5) {
            h_truth_bjet_pt->Fill(truth_jet_pt->at(j), MC_weight);
            h_truth_bjet_eta->Fill(truth_jet_eta->at(j), MC_weight);
            h_truth_bjet_phi->Fill(truth_jet_phi->at(j), MC_weight);
         } else {
            h_truth_not_bjet_pt->Fill(truth_jet_pt->at(j), MC_weight);
            h_truth_not_bjet_eta->Fill(truth_jet_eta->at(j), MC_weight);
            h_truth_not_bjet_phi->Fill(truth_jet_phi->at(j), MC_weight);
         }
      }
   }

   double efficiency = h_matched_reco_true_b_passing_Db_cut_pt->Integral() / h_matched_bjet_pt->Integral();
   double purity = h_matched_reco_true_b_passing_Db_cut_pt->Integral() / (h_matched_reco_true_b_passing_Db_cut_pt->Integral() + h_matched_reco_true_nonb_passing_Db_cut_pt->Integral());
   std::cout << "Efficiency (b-jet acceptance): " << efficiency << std::endl;
   std::cout << "Purity (b-jet fraction after cut): " << purity << std::endl;

   TH1F *h_efficiency_pt = new TH1F("h_efficiency_pt", "Efficiency vs p_{T};p_{T} [GeV];Efficiency", 100, 0.0, 200.0);
   TH1F *h_purity_pt = new TH1F("h_purity_pt", "Purity vs p_{T};p_{T} [GeV];Purity", 100, 0.0, 200.0);

   h_efficiency_pt->Divide(h_matched_reco_true_b_passing_Db_cut_pt, h_matched_reco_true_b_pt, 1.0, 1.0, "B");
   h_purity_pt->Divide(h_matched_reco_true_b_passing_Db_cut_pt, h_matched_reco_passing_Db_cut_pt, 1.0, 1.0, "B");

   h_matched_reco_passing_Db_cut_pt->Scale(purity / efficiency);
   h_matched_reco_passing_Db_cut_eta->Scale(purity / efficiency);
   h_matched_reco_passing_Db_cut_phi->Scale(purity / efficiency);

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

   bjetEventLoop analysis(chain);
   analysis.Loop();

   return 0;
   // todo: plot output.root 
}