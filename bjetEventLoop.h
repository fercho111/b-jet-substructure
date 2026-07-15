//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 13 12:19:51 2026 by ROOT version 6.40.02
// from TTree AntiKt4HI/skimmed tree
// found on file: MC_JZ1_part1.root
//////////////////////////////////////////////////////////

#ifndef bjetEventLoop_h
#define bjetEventLoop_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
using std::vector;

class bjetEventLoop {
public :
   TTree          *fChain;   ///<!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; ///<!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        MC_weight;
   vector<float>   *jet_pt;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_GN2v01_pb;
   vector<float>   *jet_GN2v01_pc;
   vector<float>   *jet_GN2v01_pu;
   vector<float>   *jet_GN2v01_ptau;
   vector<int>     *jet_HadronConeExclTruthLabelID;
   vector<int>     *jet_PartonTruthLabelID;
   vector<float>   *truth_jet_pt;
   vector<float>   *truth_jet_eta;
   vector<float>   *truth_jet_phi;
   vector<int>     *truth_jet_flavor;
   vector<float>   *track_pt;
   vector<float>   *track_eta;
   vector<float>   *track_phi;
   vector<float>   *track_d0;
   vector<float>   *track_z0sinTheta;
   vector<int>     *track_origin;
   vector<float>   *particle_pt;
   vector<float>   *particle_eta;
   vector<float>   *particle_phi;
   vector<int>     *particle_pdg;
   vector<int>     *particle_origin;

   // List of branches
   TBranch        *b_MC_weight;   ///<!
   TBranch        *b_jet_pt;   ///<!
   TBranch        *b_jet_eta;   ///<!
   TBranch        *b_jet_phi;   ///<!
   TBranch        *b_jet_GN2v01_pb;   ///<!
   TBranch        *b_jet_GN2v01_pc;   ///<!
   TBranch        *b_jet_GN2v01_pu;   ///<!
   TBranch        *b_jet_GN2v01_ptau;   ///<!
   TBranch        *b_jet_HadronConeExclTruthLabelID;   ///<!
   TBranch        *b_jet_PartonTruthLabelID;   ///<!
   TBranch        *b_truth_jet_pt;   ///<!
   TBranch        *b_truth_jet_eta;   ///<!
   TBranch        *b_truth_jet_phi;   ///<!
   TBranch        *b_truth_jet_flavor;   ///<!
   TBranch        *b_track_pt;   ///<!
   TBranch        *b_track_eta;   ///<!
   TBranch        *b_track_phi;   ///<!
   TBranch        *b_track_d0;   ///<!
   TBranch        *b_track_z0sinTheta;   ///<!
   TBranch        *b_track_origin;   ///<!
   TBranch        *b_particle_pt;   ///<!
   TBranch        *b_particle_eta;   ///<!
   TBranch        *b_particle_phi;   ///<!
   TBranch        *b_particle_pdg;   ///<!
   TBranch        *b_particle_origin;   ///<!

   bjetEventLoop(TTree *tree=0);
   virtual ~bjetEventLoop();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef bjetEventLoop_cxx
bjetEventLoop::bjetEventLoop(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("MC_JZ1_part1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("MC_JZ1_part1.root");
      }
      f->GetObject("AntiKt4HI",tree);

   }
   Init(tree);
}

bjetEventLoop::~bjetEventLoop()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t bjetEventLoop::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t bjetEventLoop::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void bjetEventLoop::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.

   // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_GN2v01_pb = 0;
   jet_GN2v01_pc = 0;
   jet_GN2v01_pu = 0;
   jet_GN2v01_ptau = 0;
   jet_HadronConeExclTruthLabelID = 0;
   jet_PartonTruthLabelID = 0;
   truth_jet_pt = 0;
   truth_jet_eta = 0;
   truth_jet_phi = 0;
   truth_jet_flavor = 0;
   track_pt = 0;
   track_eta = 0;
   track_phi = 0;
   track_d0 = 0;
   track_z0sinTheta = 0;
   track_origin = 0;
   particle_pt = 0;
   particle_eta = 0;
   particle_phi = 0;
   particle_pdg = 0;
   particle_origin = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("MC_weight", &MC_weight, &b_MC_weight);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_GN2v01_pb", &jet_GN2v01_pb, &b_jet_GN2v01_pb);
   fChain->SetBranchAddress("jet_GN2v01_pc", &jet_GN2v01_pc, &b_jet_GN2v01_pc);
   fChain->SetBranchAddress("jet_GN2v01_pu", &jet_GN2v01_pu, &b_jet_GN2v01_pu);
   fChain->SetBranchAddress("jet_GN2v01_ptau", &jet_GN2v01_ptau, &b_jet_GN2v01_ptau);
   fChain->SetBranchAddress("jet_HadronConeExclTruthLabelID", &jet_HadronConeExclTruthLabelID, &b_jet_HadronConeExclTruthLabelID);
   fChain->SetBranchAddress("jet_PartonTruthLabelID", &jet_PartonTruthLabelID, &b_jet_PartonTruthLabelID);
   fChain->SetBranchAddress("truth_jet_pt", &truth_jet_pt, &b_truth_jet_pt);
   fChain->SetBranchAddress("truth_jet_eta", &truth_jet_eta, &b_truth_jet_eta);
   fChain->SetBranchAddress("truth_jet_phi", &truth_jet_phi, &b_truth_jet_phi);
   fChain->SetBranchAddress("truth_jet_flavor", &truth_jet_flavor, &b_truth_jet_flavor);
   fChain->SetBranchAddress("track_pt", &track_pt, &b_track_pt);
   fChain->SetBranchAddress("track_eta", &track_eta, &b_track_eta);
   fChain->SetBranchAddress("track_phi", &track_phi, &b_track_phi);
   fChain->SetBranchAddress("track_d0", &track_d0, &b_track_d0);
   fChain->SetBranchAddress("track_z0sinTheta", &track_z0sinTheta, &b_track_z0sinTheta);
   fChain->SetBranchAddress("track_origin", &track_origin, &b_track_origin);
   fChain->SetBranchAddress("particle_pt", &particle_pt, &b_particle_pt);
   fChain->SetBranchAddress("particle_eta", &particle_eta, &b_particle_eta);
   fChain->SetBranchAddress("particle_phi", &particle_phi, &b_particle_phi);
   fChain->SetBranchAddress("particle_pdg", &particle_pdg, &b_particle_pdg);
   fChain->SetBranchAddress("particle_origin", &particle_origin, &b_particle_origin);
   Notify();
}

bool bjetEventLoop::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be for a new TTree in a TChain. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void bjetEventLoop::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t bjetEventLoop::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef bjetEventLoop_cxx
