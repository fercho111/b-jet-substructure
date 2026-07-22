#define bjetEventLoop_cxx
#include "bjetEventLoop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TVector2.h>
#include <TLorentzVector.h>
#include <iostream>
#include <fstream>
#include <string>
#include <fastjet/PseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/ClusterSequence.hh>

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
   TH1F *h_1_jet_pt = new TH1F("h_1_jet_pt", "Reconstructed Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_1_jet_eta = new TH1F("h_1_jet_eta", "Reconstructed Jet #eta;#eta;Jets", 100, -5.0, 5.0); // Jets as reconstructed by the detector in the MC simulation
   TH1F *h_1_jet_phi = new TH1F("h_1_jet_phi", "Reconstructed Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // Jets as reconstructed by the detector in the MC simulation

   // MC simulation jets (all)
   TH1F *h_1_truth_jet_pt = new TH1F("h_truth_jet_pT", "Truth Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_1_truth_jet_eta = new TH1F("h_1_truth_jet_eta", "Truth Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation jets (all)
   TH1F *h_1_truth_jet_phi = new TH1F("h_1_truth_jet_phi", "Truth Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation jets (all)

   // MC simulation bjets
   TH1F *h_1_truth_bjet_pt = new TH1F("h_truth_bjet_pT", "Truth b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_1_truth_bjet_eta = new TH1F("h_1_truth_bjet_eta", "Truth b-Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation bjets
   TH1F *h_1_truth_bjet_phi = new TH1F("h_1_truth_bjet_phi", "Truth b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation bjets

   // MC simulation jets - bjets
   // TODO: figure out if they're removable?
   TH1F *h_1_truth_not_bjet_pt = new TH1F("h_truth_not_bjet_pT", "Truth non-b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0); 
   TH1F *h_1_truth_not_bjet_eta = new TH1F("h_1_truth_not_bjet_eta", "Truth non-b-Jet #eta;#eta;Jets", 100, -5.0, 5.0); // MC simulation jets - bjets
   TH1F *h_1_truth_not_bjet_phi = new TH1F("h_1_truth_not_bjet_phi", "Truth non-b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi()); // MC simulation jets - bjets

   // delta R's 
   TH1F *h_1_delta_r = new TH1F("h_1_delta_r", "#DeltaR (reco, truth);#DeltaR;Pairs", 100, 0.0, 10.0);
   // delta R's for matched jets only
   TH1F *h_1_delta_r_matched_pairs = new TH1F("h_1_delta_r_matched_pairs", "#DeltaR matched pairs;#DeltaR;Pairs", 100, 0.0, 10.0);


   // pT for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_2_matched_bjet_pt = new TH1F("h_matched_bjet_pT", "Matched reco b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // η for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_2_matched_bjet_eta = new TH1F("h_2_matched_bjet_eta", "Matched reco b-Jet #eta;#eta;Jets", 100, -5.0, 5.0);
   // φ for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was bottom quark
   TH1F *h_2_matched_bjet_phi = new TH1F("h_2_matched_bjet_phi", "Matched reco b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_2_matched_not_bjet_pt = new TH1F("h_matched_not_bjet_pT", "Matched reco non-b-Jet p_{T};p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // η for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_2_matched_not_bjet_eta = new TH1F("h_2_matched_not_bjet_eta", "Matched reco non-b-Jet #eta;#eta;Jets", 100, -5.0, 5.0);
   // φ for reconstructed/matched jets (ΔR < 0.3) whose true jet flavour was NOT bottom quark
   TH1F *h_2_matched_not_bjet_phi = new TH1F("h_2_matched_not_bjet_phi", "Matched reco non-b-Jet #phi;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // D_b for reconstructed jets whose true jet flavour (jet_HadronConeExclTruthLabelID == 5) was bottom quark
   TH1F *h_2_discriminant_b_bjet = new TH1F("h_2_discriminant_b_bjet", "GN discriminant D_{b} b-jets",100, -10.0, 20.0);
   // D_b for reconstructed jets whose true jet flavour (jet_HadronConeExclTruthLabelID != 5) was NOT bottom quark
   TH1F *h_2_discriminant_b_not_bjet = new TH1F("h_2_discriminant_b_not_bjet", "GN discriminant D_{b} non b-jets",100, -10.0, 20.0);

   // pT for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_2_matched_reco_passing_Db_cut_pt = new TH1F("h_2_matched_reco_passing_Db_cut_pt", "Matched reco jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_2_matched_reco_passing_Db_cut_eta = new TH1F("h_2_matched_reco_passing_Db_cut_eta", "Matched reco jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for all matched reco jets (any flavor) passing D_b > 0 -- will be scaled by purity/efficiency
   TH1F *h_2_matched_reco_passing_Db_cut_phi = new TH1F("h_2_matched_reco_passing_Db_cut_phi", "Matched reco jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_2_matched_reco_true_b_pt = new TH1F("h_2_matched_reco_true_b_pt", "Matched reco jets (true b);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_2_matched_reco_true_b_eta = new TH1F("h_2_matched_reco_true_b_eta", "Matched reco jets (true b);#eta;Jets", 100, -5.0, 5.0);
   // phi for matched reco jets with HadronConeExcl == 5 -- truth reference for comparison
   TH1F *h_2_matched_reco_true_b_phi = new TH1F("h_2_matched_reco_true_b_phi", "Matched reco jets (true b);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // pT for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_2_matched_reco_true_b_passing_Db_cut_pt = new TH1F("h_2_matched_reco_true_b_passing_Db_cut_pt", "True b-jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_2_matched_reco_true_b_passing_Db_cut_eta = new TH1F("h_2_matched_reco_true_b_passing_Db_cut_eta", "True b-jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for true b-jets passing D_b > 0 -- numerator for efficiency and purity
   TH1F *h_2_matched_reco_true_b_passing_Db_cut_phi = new TH1F("h_2_matched_reco_true_b_passing_Db_cut_phi", "True b-jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());
   // pT for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_2_matched_reco_true_nonb_passing_Db_cut_pt = new TH1F("h_2_matched_reco_true_nonb_passing_Db_cut_pt", "True non-b-jets passing D_{b} cut;p_{T} [GeV];Jets", 100, 0.0, 200.0);
   // eta for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_2_matched_reco_true_nonb_passing_Db_cut_eta = new TH1F("h_2_matched_reco_true_nonb_passing_Db_cut_eta", "True non-b-jets passing D_{b} cut;#eta;Jets", 100, -5.0, 5.0);
   // phi for true non-b-jets passing D_b > 0 -- for purity denominator
   TH1F *h_2_matched_reco_true_nonb_passing_Db_cut_phi = new TH1F("h_2_matched_reco_true_nonb_passing_Db_cut_phi", "True non-b-jets passing D_{b} cut;#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   
   // Matched reco jets passing D_b cut with |eta| < 2.1 (to be scaled by purity/efficiency)
   TH1F *h_2_etacut_matched_reco_passing_Db_cut_pt = new TH1F("h_2_etacut_matched_reco_passing_Db_cut_pt", "Matched reco jets passing D_{b} cut (|#eta|<2.1);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_2_etacut_matched_reco_passing_Db_cut_eta = new TH1F("h_2_etacut_matched_reco_passing_Db_cut_eta", "Matched reco jets passing D_{b} cut (|#eta|<2.1);#eta;Jets", 100, -5.0, 5.0);
   TH1F *h_2_etacut_matched_reco_passing_Db_cut_phi = new TH1F("h_2_etacut_matched_reco_passing_Db_cut_phi", "Matched reco jets passing D_{b} cut (|#eta|<2.1);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // Matched reco jets with HadronConeExcl == 5 with |eta| < 2.1 (truth reference)
   TH1F *h_2_etacut_matched_reco_true_b_pt = new TH1F("h_2_etacut_matched_reco_true_b_pt", "Matched reco jets (true b, |#eta|<2.1);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_2_etacut_matched_reco_true_b_eta = new TH1F("h_2_etacut_matched_reco_true_b_eta", "Matched reco jets (true b, |#eta|<2.1);#eta;Jets", 100, -5.0, 5.0);
   TH1F *h_2_etacut_matched_reco_true_b_phi = new TH1F("h_2_etacut_matched_reco_true_b_phi", "Matched reco jets (true b, |#eta|<2.1);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // True b-jets passing D_b cut with |eta| < 2.1 (for eta-cut efficiency)
   TH1F *h_2_etacut_matched_reco_true_b_passing_Db_cut_pt = new TH1F("h_2_etacut_matched_reco_true_b_passing_Db_cut_pt", "True b-jets passing D_{b} cut (|#eta|<2.1);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_2_etacut_matched_reco_true_b_passing_Db_cut_eta = new TH1F("h_2_etacut_matched_reco_true_b_passing_Db_cut_eta", "True b-jets passing D_{b} cut (|#eta|<2.1);#eta;Jets", 100, -5.0, 5.0);
   TH1F *h_2_etacut_matched_reco_true_b_passing_Db_cut_phi = new TH1F("h_2_etacut_matched_reco_true_b_passing_Db_cut_phi", "True b-jets passing D_{b} cut (|#eta|<2.1);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // True non-b-jets passing D_b cut with |eta| < 2.1 (for eta-cut purity)
   TH1F *h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt = new TH1F("h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt", "True non-b-jets passing D_{b} cut (|#eta|<2.1);p_{T} [GeV];Jets", 100, 0.0, 200.0);
   TH1F *h_2_etacut_matched_reco_true_nonb_passing_Db_cut_eta = new TH1F("h_2_etacut_matched_reco_true_nonb_passing_Db_cut_eta", "True non-b-jets passing D_{b} cut (|#eta|<2.1);#eta;Jets", 100, -5.0, 5.0);
   TH1F *h_2_etacut_matched_reco_true_nonb_passing_Db_cut_phi = new TH1F("h_2_etacut_matched_reco_true_nonb_passing_Db_cut_phi", "True non-b-jets passing D_{b} cut (|#eta|<2.1);#phi [rad];Jets", 100, -TMath::Pi(), TMath::Pi());

   // ======================================================================
   // TRACK SUBSTRUCTURE HISTOGRAMS
   // ======================================================================

   // track multiplicity per jet -- inclusive matched reco jets (any D_b)
   TH1F *h_3_track_multiplicity_inclusive = new TH1F("h_3_track_multiplicity_inclusive", "Track multiplicity (inclusive);N_{tracks};Jets", 50, 0, 50);
   // track pT -- inclusive matched reco jets
   TH1F *h_3_track_pt_inclusive = new TH1F("h_3_track_pt_inclusive", "Track p_{T} (inclusive);p_{T} [GeV];Tracks", 100, 0.0, 50.0);
   // track d0 -- inclusive matched reco jets
   TH1F *h_3_track_d0_inclusive = new TH1F("h_3_track_d0_inclusive", "Track d_{0} (inclusive);d_{0} [mm];Tracks", 100, -2.0, 2.0);
   // track origin label -- inclusive matched reco jets
   TH1F *h_3_track_origin_inclusive = new TH1F("h_3_track_origin_inclusive", "Track origin (inclusive);Origin label;Tracks", 20, 0, 20);

   // track multiplicity per jet -- D_b selected matched reco jets
   TH1F *h_3_track_multiplicity_Db_selected = new TH1F("h_3_track_multiplicity_Db_selected", "Track multiplicity (D_{b} selected);N_{tracks};Jets", 50, 0, 50);
   // track pT -- D_b selected matched reco jets
   TH1F *h_3_track_pt_Db_selected = new TH1F("h_3_track_pt_Db_selected", "Track p_{T} (D_{b} selected);p_{T} [GeV];Tracks", 100, 0.0, 50.0);
   // track d0 -- D_b selected matched reco jets
   TH1F *h_3_track_d0_Db_selected = new TH1F("h_3_track_d0_Db_selected", "Track d_{0} (D_{b} selected);d_{0} [mm];Tracks", 100, -2.0, 2.0);
   // track origin label -- D_b selected matched reco jets
   TH1F *h_3_track_origin_Db_selected = new TH1F("h_3_track_origin_Db_selected", "Track origin (D_{b} selected);Origin label;Tracks", 20, 0, 20);

   // particle multiplicity per truth jet -- inclusive
   TH1F *h_3_particle_multiplicity_inclusive = new TH1F("h_3_particle_multiplicity_inclusive", "Particle multiplicity (inclusive);N_{particles};Jets", 50, 0, 50);
   // particle pT -- inclusive truth jets
   TH1F *h_3_particle_pt_inclusive = new TH1F("h_3_particle_pt_inclusive", "Particle p_{T} (inclusive);p_{T} [GeV];Particles", 100, 0.0, 50.0);
   // particle origin label -- inclusive truth jets
   TH1F *h_3_particle_origin_inclusive = new TH1F("h_3_particle_origin_inclusive", "Particle origin (inclusive);Origin label;Particles", 20, 0, 20);

   // particle multiplicity per truth jet -- b-jets only
   TH1F *h_3_particle_multiplicity_bjet = new TH1F("h_3_particle_multiplicity_bjet", "Particle multiplicity (b-jets);N_{particles};Jets", 50, 0, 50);
   // particle pT -- truth b-jets only
   TH1F *h_3_particle_pt_bjet = new TH1F("h_3_particle_pt_bjet", "Particle p_{T} (b-jets);p_{T} [GeV];Particles", 100, 0.0, 50.0);
   // particle origin label -- truth b-jets only
   TH1F *h_3_particle_origin_bjet = new TH1F("h_3_particle_origin_bjet", "Particle origin (b-jets);Origin label;Particles", 20, 0, 20);


   /* set histogram option*/
   h_1_jet_pt->SetOption("HIST");
   h_1_jet_eta->SetOption("HIST");
   h_1_jet_phi->SetOption("HIST");

   h_1_truth_jet_pt->SetOption("HIST");
   h_1_truth_jet_eta->SetOption("HIST");
   h_1_truth_jet_phi->SetOption("HIST");

   h_1_truth_bjet_pt->SetOption("HIST");
   h_1_truth_bjet_eta->SetOption("HIST");
   h_1_truth_bjet_phi->SetOption("HIST");

   h_1_truth_not_bjet_pt->SetOption("HIST");
   h_1_truth_not_bjet_eta->SetOption("HIST");
   h_1_truth_not_bjet_phi->SetOption("HIST");

   h_1_delta_r->SetOption("HIST");
   h_1_delta_r_matched_pairs->SetOption("HIST");

   h_2_matched_bjet_pt->SetOption("HIST");
   h_2_matched_bjet_eta->SetOption("HIST");
   h_2_matched_bjet_phi->SetOption("HIST");

   h_2_matched_not_bjet_pt->SetOption("HIST");
   h_2_matched_not_bjet_eta->SetOption("HIST");
   h_2_matched_not_bjet_phi->SetOption("HIST");

   h_2_discriminant_b_bjet->SetOption("HIST");
   h_2_discriminant_b_not_bjet->SetOption("HIST");

   h_2_matched_reco_passing_Db_cut_pt->SetOption("HIST");
   h_2_matched_reco_passing_Db_cut_eta->SetOption("HIST");
   h_2_matched_reco_passing_Db_cut_phi->SetOption("HIST");

   h_2_matched_reco_true_b_pt->SetOption("HIST");
   h_2_matched_reco_true_b_eta->SetOption("HIST");
   h_2_matched_reco_true_b_phi->SetOption("HIST");


   h_2_matched_reco_true_b_passing_Db_cut_pt->SetOption("HIST");
   h_2_matched_reco_true_b_passing_Db_cut_eta->SetOption("HIST");
   h_2_matched_reco_true_b_passing_Db_cut_phi->SetOption("HIST");
   h_2_matched_reco_true_nonb_passing_Db_cut_pt->SetOption("HIST");
   h_2_matched_reco_true_nonb_passing_Db_cut_eta->SetOption("HIST");
   h_2_matched_reco_true_nonb_passing_Db_cut_phi->SetOption("HIST");

   h_2_etacut_matched_reco_passing_Db_cut_pt->SetOption("HIST");
   h_2_etacut_matched_reco_passing_Db_cut_eta->SetOption("HIST");
   h_2_etacut_matched_reco_passing_Db_cut_phi->SetOption("HIST");

   h_2_etacut_matched_reco_true_b_pt->SetOption("HIST");
   h_2_etacut_matched_reco_true_b_eta->SetOption("HIST");
   h_2_etacut_matched_reco_true_b_phi->SetOption("HIST");

   h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->SetOption("HIST");
   h_2_etacut_matched_reco_true_b_passing_Db_cut_eta->SetOption("HIST");
   h_2_etacut_matched_reco_true_b_passing_Db_cut_phi->SetOption("HIST");

   h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt->SetOption("HIST");
   h_2_etacut_matched_reco_true_nonb_passing_Db_cut_eta->SetOption("HIST");
   h_2_etacut_matched_reco_true_nonb_passing_Db_cut_phi->SetOption("HIST");

   h_3_track_multiplicity_inclusive->SetOption("HIST");
   h_3_track_pt_inclusive->SetOption("HIST");
   h_3_track_d0_inclusive->SetOption("HIST");
   h_3_track_origin_inclusive->SetOption("HIST");
   h_3_track_multiplicity_Db_selected->SetOption("HIST");
   h_3_track_pt_Db_selected->SetOption("HIST");
   h_3_track_d0_Db_selected->SetOption("HIST");
   h_3_track_origin_Db_selected->SetOption("HIST");

   h_3_particle_multiplicity_inclusive->SetOption("HIST");
   h_3_particle_pt_inclusive->SetOption("HIST");
   h_3_particle_origin_inclusive->SetOption("HIST");
   h_3_particle_multiplicity_bjet->SetOption("HIST");
   h_3_particle_pt_bjet->SetOption("HIST");
   h_3_particle_origin_bjet->SetOption("HIST");
   /* end set histogram option */

   // Get the total number of events in the tree/chain
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;

      for (unsigned int l = 0; l < truth_jet_pt->size(); l++)
      {
         for (unsigned int i = 0; i < jet_pt->size(); i++)
         {
            double deta = jet_eta->at(i) - truth_jet_eta->at(l);
            double dphi = fabs(TVector2::Phi_mpi_pi(jet_phi->at(i) - truth_jet_phi->at(l)));
            double dr = sqrt(deta*deta + dphi*dphi);
            h_1_delta_r->Fill(dr, MC_weight);
            // TODO: check if there is more than one reco jet matched to truth
            if (dr < 0.3)
            {
               // fill inclusive matched reco jet kinematics
               h_1_jet_pt->Fill(jet_pt->at(i), MC_weight);
               h_1_jet_eta->Fill(jet_eta->at(i), MC_weight);
               h_1_jet_phi->Fill(jet_phi->at(i), MC_weight);
               h_1_delta_r_matched_pairs->Fill(dr, MC_weight);

               // split matched reco jets by truth jet flavor (from truth_jet_flavor)
               if (truth_jet_flavor->at(l) == 5) {
                  h_2_matched_bjet_pt->Fill(jet_pt->at(i), MC_weight);
                  h_2_matched_bjet_eta->Fill(jet_eta->at(i), MC_weight);
                  h_2_matched_bjet_phi->Fill(jet_phi->at(i), MC_weight);
               } else {
                  h_2_matched_not_bjet_pt->Fill(jet_pt->at(i), MC_weight);
                  h_2_matched_not_bjet_eta->Fill(jet_eta->at(i), MC_weight);
                  h_2_matched_not_bjet_phi->Fill(jet_phi->at(i), MC_weight);
               }

               // compute D_b discriminant for this reco jet
               double D_b = log(jet_GN2v01_pb->at(i) / (f_c * jet_GN2v01_pc->at(i) + f_tau * jet_GN2v01_ptau->at(i) + (1 - f_c - f_tau) * jet_GN2v01_pu->at(i)));

               // fill all matched reco jets passing D_b cut (any flavor)
               if (D_b > 0)
               {
                  h_2_matched_reco_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                  h_2_matched_reco_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                  h_2_matched_reco_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
               }

               // split by reco-level truth label (HadronConeExcl) for D_b studies
               if (jet_HadronConeExclTruthLabelID->at(i) == 5)
               {
                  h_2_matched_reco_true_b_pt->Fill(jet_pt->at(i), MC_weight);
                  h_2_matched_reco_true_b_eta->Fill(jet_eta->at(i), MC_weight);
                  h_2_matched_reco_true_b_phi->Fill(jet_phi->at(i), MC_weight);
                  h_2_discriminant_b_bjet->Fill(D_b, MC_weight);
                  // we propose Db cut on Db==0
                  if (D_b > 0)
                  {
                     h_2_matched_reco_true_b_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                     h_2_matched_reco_true_b_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                     h_2_matched_reco_true_b_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                  }
               }
               else
               {
                  h_2_discriminant_b_not_bjet->Fill(D_b, MC_weight);
                  if (D_b > 0)
                  {
                     h_2_matched_reco_true_nonb_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                     h_2_matched_reco_true_nonb_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                     h_2_matched_reco_true_nonb_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                  }
               }

               // |eta| < 2.1 cut on both reco and truth jets
               if (fabs(jet_eta->at(i)) < 2.1 && fabs(truth_jet_eta->at(l)) < 2.1)
               {
                  if (D_b > 0)
                  {
                     h_2_etacut_matched_reco_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                     h_2_etacut_matched_reco_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                     h_2_etacut_matched_reco_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                  }

                  if (jet_HadronConeExclTruthLabelID->at(i) == 5)
                  {
                     h_2_etacut_matched_reco_true_b_pt->Fill(jet_pt->at(i), MC_weight);
                     h_2_etacut_matched_reco_true_b_eta->Fill(jet_eta->at(i), MC_weight);
                     h_2_etacut_matched_reco_true_b_phi->Fill(jet_phi->at(i), MC_weight);
                     if (D_b > 0)
                     {
                        h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                        h_2_etacut_matched_reco_true_b_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                        h_2_etacut_matched_reco_true_b_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                     }
                  }
                  else
                  {
                     if (D_b > 0)
                     {
                        h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt->Fill(jet_pt->at(i), MC_weight);
                        h_2_etacut_matched_reco_true_nonb_passing_Db_cut_eta->Fill(jet_eta->at(i), MC_weight);
                        h_2_etacut_matched_reco_true_nonb_passing_Db_cut_phi->Fill(jet_phi->at(i), MC_weight);
                     }
                  }
               }



               /* Ghost Association */
   
               // Ghost association method
               // Input: array of tracks (their properties) and one jet
               // Output: array of tracks belonging to the jet
   
               // Make ghosts from tracks
               vector<fastjet::PseudoJet> ghosts;
               vector<fastjet::PseudoJet> tracks;
               double ghost_pT = 1e-100;
               TLorentzVector g, t, j;
               for (unsigned int k=0; k<track_pt->size(); k++)
               {
                  g.SetPtEtaPhiM(ghost_pT, track_eta->at(k), track_phi->at(k), 0.0); // We consider massless ghosts
                  t.SetPtEtaPhiM(track_pt->at(k), track_eta->at(k), track_phi->at(k), 0.14); // We consider charged pions
                  fastjet::PseudoJet ghost(g.Px(), g.Py(), g.Pz(), g.E());
                  fastjet::PseudoJet track(t.Px(), t.Py(), t.Pz(), t.E());
                  ghosts.push_back(ghost);
                  tracks.push_back(track);
               }
   
               // Associate ghosts to jet
               double R = 0.4;
               j.SetPtEtaPhiM(jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), jet_m->at(i));
               fastjet::PseudoJet jet(j.Px(), j.Py(), j.Pz(), j.E());
               vector<fastjet::PseudoJet> constituents;
               fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R); // R = 0.4
               if (ghosts.size()>0 && jet.E()>0)
               {
                  ghosts.push_back(jet);
                  fastjet::ClusterSequence cs(ghosts, jet_def);
                  vector<fastjet::PseudoJet> ghost_jets = sorted_by_pt(cs.inclusive_jets());
                  constituents = sorted_by_pt(ghost_jets[0].constituents());
                  constituents.erase(constituents.begin()); // We erase the jet from constituents
               }
   
               // Match tracks and ghosts
               vector<fastjet::PseudoJet> selected_tracks;
               // need this to plot particle origin label for jets
               vector<int> selected_track_indices;
               if (constituents.size()>0)
               {
                  for (auto constituent:constituents)
                  {
                     for (unsigned int k=0; k<tracks.size(); k++)
                     {
                        float diff_eta = std::fabs(constituent.eta()-tracks[k].eta());
                        float diff_phi = std::fabs(TVector2::Phi_mpi_pi(constituent.phi()-tracks[k].phi()));
                        if (diff_eta<1e-5 && diff_phi<1e-5)
                        {
                           selected_tracks.push_back(tracks[k]);
                           selected_track_indices.push_back(k);
                           break;
                        }
                     }
                  }
               }

               // fill track substructure histograms for inclusive matched jets
               h_3_track_multiplicity_inclusive->Fill(selected_track_indices.size(), MC_weight);
               for (unsigned int t=0; t<selected_track_indices.size(); t++)
               {
                  int idx = selected_track_indices[t];
                  h_3_track_pt_inclusive->Fill(track_pt->at(idx), MC_weight);
                  h_3_track_d0_inclusive->Fill(track_d0->at(idx), MC_weight);
                  h_3_track_origin_inclusive->Fill(track_origin->at(idx), MC_weight);
               }

               // fill track substructure histograms for D_b selected jets
               if (D_b > 0)
               {
                  h_3_track_multiplicity_Db_selected->Fill(selected_track_indices.size(), MC_weight);
                  for (unsigned int t=0; t<selected_track_indices.size(); t++)
                  {
                     int idx = selected_track_indices[t];
                     h_3_track_pt_Db_selected->Fill(track_pt->at(idx), MC_weight);
                     h_3_track_d0_Db_selected->Fill(track_d0->at(idx), MC_weight);
                     h_3_track_origin_Db_selected->Fill(track_origin->at(idx), MC_weight);
                  }
               }
            }
         }
         h_1_truth_jet_pt->Fill(truth_jet_pt->at(l), MC_weight);
         h_1_truth_jet_eta->Fill(truth_jet_eta->at(l), MC_weight);
         h_1_truth_jet_phi->Fill(truth_jet_phi->at(l), MC_weight);

         if (truth_jet_flavor->at(l) == 5)
         {
            h_1_truth_bjet_pt->Fill(truth_jet_pt->at(l), MC_weight);
            h_1_truth_bjet_eta->Fill(truth_jet_eta->at(l), MC_weight);
            h_1_truth_bjet_phi->Fill(truth_jet_phi->at(l), MC_weight);
         }
         else
         {
            h_1_truth_not_bjet_pt->Fill(truth_jet_pt->at(l), MC_weight);
            h_1_truth_not_bjet_eta->Fill(truth_jet_eta->at(l), MC_weight);
            h_1_truth_not_bjet_phi->Fill(truth_jet_phi->at(l), MC_weight);
         }

         /* Ghost association: truth particles to truth jet */
         vector<fastjet::PseudoJet> particle_ghosts;
         vector<fastjet::PseudoJet> particles;
         double particle_ghost_pT = 1e-100;
         TLorentzVector pg, pp, tj;
         for (unsigned int k=0; k<particle_pt->size(); k++)
         {
            pg.SetPtEtaPhiM(particle_ghost_pT, particle_eta->at(k), particle_phi->at(k), 0.0);
            pp.SetPtEtaPhiM(particle_pt->at(k), particle_eta->at(k), particle_phi->at(k), 0.14);
            fastjet::PseudoJet pghost(pg.Px(), pg.Py(), pg.Pz(), pg.E());
            fastjet::PseudoJet ptrack(pp.Px(), pp.Py(), pp.Pz(), pp.E());
            particle_ghosts.push_back(pghost);
            particles.push_back(ptrack);
         }

         // build truth jet PseudoJet
         double R_truth = 0.4;
         tj.SetPtEtaPhiM(truth_jet_pt->at(l), truth_jet_eta->at(l), truth_jet_phi->at(l), 0.0);
         fastjet::PseudoJet truth_jet_pj(tj.Px(), tj.Py(), tj.Pz(), tj.E());
         vector<fastjet::PseudoJet> particle_constituents;
         fastjet::JetDefinition truth_jet_def(fastjet::antikt_algorithm, R_truth);
         if (particle_ghosts.size()>0 && truth_jet_pj.E()>0)
         {
            particle_ghosts.push_back(truth_jet_pj);
            fastjet::ClusterSequence pcs(particle_ghosts, truth_jet_def);
            vector<fastjet::PseudoJet> truth_ghost_jets = sorted_by_pt(pcs.inclusive_jets());
            particle_constituents = sorted_by_pt(truth_ghost_jets[0].constituents());
            particle_constituents.erase(particle_constituents.begin());
         }

         // match particle ghosts back to original indices
         vector<int> selected_particle_indices;
         if (particle_constituents.size()>0)
         {
            for (auto constituent:particle_constituents)
            {
               for (unsigned int k=0; k<particles.size(); k++)
               {
                  float diff_eta = std::fabs(constituent.eta()-particles[k].eta());
                  float diff_phi = std::fabs(TVector2::Phi_mpi_pi(constituent.phi()-particles[k].phi()));
                  if (diff_eta<1e-5 && diff_phi<1e-5)
                  {
                     selected_particle_indices.push_back(k);
                     break;
                  }
               }
            }
         }
         

         // fill truth particle histograms -- inclusive (all truth jets)
         h_3_particle_multiplicity_inclusive->Fill(selected_particle_indices.size(), MC_weight);
         for (unsigned int p=0; p<selected_particle_indices.size(); p++)
         {
            int idx = selected_particle_indices[p];
            h_3_particle_pt_inclusive->Fill(particle_pt->at(idx), MC_weight);
            h_3_particle_origin_inclusive->Fill(particle_origin->at(idx), MC_weight);
         }

         // fill truth particle histograms -- b-jets only
         if (truth_jet_flavor->at(l) == 5)
         {
            h_3_particle_multiplicity_bjet->Fill(selected_particle_indices.size(), MC_weight);
            for (unsigned int p=0; p<selected_particle_indices.size(); p++)
            {
               int idx = selected_particle_indices[p];
               h_3_particle_pt_bjet->Fill(particle_pt->at(idx), MC_weight);
               h_3_particle_origin_bjet->Fill(particle_origin->at(idx), MC_weight);
            }
         }

       }
    }

   // ======================================================================
   // CORRECTION FACTORS (inclusive, no eta cut)
   // ======================================================================
   double efficiency = h_2_matched_reco_true_b_passing_Db_cut_pt->Integral() / h_2_matched_reco_true_b_pt->Integral();
   double purity = h_2_matched_reco_true_b_passing_Db_cut_pt->Integral() / (h_2_matched_reco_true_b_passing_Db_cut_pt->Integral() + h_2_matched_reco_true_nonb_passing_Db_cut_pt->Integral());
   std::cout << "=== No eta cut ===" << std::endl;
   std::cout << "Efficiency (b-jet acceptance): " << efficiency << std::endl;
   std::cout << "Purity (b-jet fraction after cut): " << purity << std::endl;
   std::cout << "Correction factor (purity/efficiency): " << purity / efficiency << std::endl;

   // scale the D_b-passing reco jets by purity/efficiency
   h_2_matched_reco_passing_Db_cut_pt->Scale(purity / efficiency);
   h_2_matched_reco_passing_Db_cut_eta->Scale(purity / efficiency);
   h_2_matched_reco_passing_Db_cut_phi->Scale(purity / efficiency);

   TH1F *h_2_efficiency_pt = new TH1F("h_2_efficiency_pt", "Efficiency vs p_{T};p_{T} [GeV];Efficiency", 100, 0.0, 200.0);
   TH1F *h_2_purity_pt = new TH1F("h_2_purity_pt", "Purity vs p_{T};p_{T} [GeV];Purity", 100, 0.0, 200.0);

   TH1F *h_2_all_passing_Db_cut_pt = (TH1F*)h_2_matched_reco_true_b_passing_Db_cut_pt->Clone("h_2_all_passing_Db_cut_pt");
   h_2_all_passing_Db_cut_pt->Add(h_2_matched_reco_true_nonb_passing_Db_cut_pt);

   h_2_efficiency_pt->Divide(h_2_matched_reco_true_b_passing_Db_cut_pt, h_2_matched_reco_true_b_pt, 1.0, 1.0, "B");
   h_2_purity_pt->Divide(h_2_matched_reco_true_b_passing_Db_cut_pt, h_2_all_passing_Db_cut_pt, 1.0, 1.0, "B");

   TH1F *h_2_matched_reco_passing_Db_cut_pt_ptcorr = (TH1F*)h_2_matched_reco_passing_Db_cut_pt->Clone("h_2_matched_reco_passing_Db_cut_pt_ptcorr");
   h_2_matched_reco_passing_Db_cut_pt_ptcorr->SetTitle("Matched reco jets passing D_{b} cut (p_{T}-dep. corrected);p_{T} [GeV];Jets");

   h_2_matched_reco_passing_Db_cut_pt_ptcorr->Scale(efficiency / purity);
   for (int bin = 1; bin <= h_2_matched_reco_passing_Db_cut_pt_ptcorr->GetNbinsX(); bin++) {
      double eff_bin = h_2_efficiency_pt->GetBinContent(bin);
      double pur_bin = h_2_purity_pt->GetBinContent(bin);
      if (eff_bin > 0) {
         h_2_matched_reco_passing_Db_cut_pt_ptcorr->SetBinContent(bin,
            h_2_matched_reco_passing_Db_cut_pt_ptcorr->GetBinContent(bin) * pur_bin / eff_bin);
      } else {
         h_2_matched_reco_passing_Db_cut_pt_ptcorr->SetBinContent(bin, 0);
      }
   }

   // ======================================================================
   // CORRECTION FACTORS (with |eta| < 2.1 cut)
   // ======================================================================
   double efficiency_etacut = h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->Integral() / h_2_etacut_matched_reco_true_b_pt->Integral();
   double purity_etacut = h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->Integral() / (h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->Integral() + h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt->Integral());
   std::cout << "\n=== With |eta| < 2.1 cut ===" << std::endl;
   std::cout << "Efficiency (b-jet acceptance): " << efficiency_etacut << std::endl;
   std::cout << "Purity (b-jet fraction after cut): " << purity_etacut << std::endl;
   std::cout << "Correction factor (purity/efficiency): " << purity_etacut / efficiency_etacut << std::endl;

   // Scale eta-cut D_b-passing histograms by purity/efficiency
   h_2_etacut_matched_reco_passing_Db_cut_pt->Scale(purity_etacut / efficiency_etacut);
   h_2_etacut_matched_reco_passing_Db_cut_eta->Scale(purity_etacut / efficiency_etacut);
   h_2_etacut_matched_reco_passing_Db_cut_phi->Scale(purity_etacut / efficiency_etacut);

   // pT-dependent efficiency and purity with eta cut
   TH1F *h_2_etacut_efficiency_pt = new TH1F("h_2_etacut_efficiency_pt", "Efficiency vs p_{T} (|#eta|<2.1);p_{T} [GeV];Efficiency", 100, 0.0, 200.0);
   TH1F *h_2_etacut_purity_pt = new TH1F("h_2_etacut_purity_pt", "Purity vs p_{T} (|#eta|<2.1);p_{T} [GeV];Purity", 100, 0.0, 200.0);

   TH1F *h_2_etacut_all_passing_Db_cut_pt = (TH1F*)h_2_etacut_matched_reco_true_b_passing_Db_cut_pt->Clone("h_2_etacut_all_passing_Db_cut_pt");
   h_2_etacut_all_passing_Db_cut_pt->Add(h_2_etacut_matched_reco_true_nonb_passing_Db_cut_pt);

   h_2_etacut_efficiency_pt->Divide(h_2_etacut_matched_reco_true_b_passing_Db_cut_pt, h_2_etacut_matched_reco_true_b_pt, 1.0, 1.0, "B");
   h_2_etacut_purity_pt->Divide(h_2_etacut_matched_reco_true_b_passing_Db_cut_pt, h_2_etacut_all_passing_Db_cut_pt, 1.0, 1.0, "B");



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