    // Ghost association method
    // Input: array of tracks (their properties) and one jet
    // Output: array of tracks belonging to the jet
    
    // Make ghosts from tracks
    vector<fastjet::PseudoJet> ghosts;
    vector<fastjet::PseudoJet> tracks;
    double ghost_pT = 1e-100;
    TLorentzVector g, t;
    for (unsigned int j=0; j<track_pt->size(); j++){
      g.SetPtEtaPhiM(ghost_pT, track_eta->at(j), track_phi->at(j), 0.0); // We consider massless ghosts
      t.SetPtEtaPhiM(track_pt->at(j), track_eta->at(j), track_phi->at(j), 0.14); // We consider charged pions
      fastjet::PseudoJet ghost(g.Px(), g.Py(), g.Pz(), g.E());
      fastjet::PseudoJet track(t.Px(), t.Py(), t.Pz(), t.E());
      ghosts.push_back(ghost);
      tracks.push_back(track);
    }

    // Associate ghosts to jet
    vector<fastjet::PseudoJet> constituents;
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R); // R=0.4
    if (ghosts.size()>0 && jet.E()>0){
      ghosts.push_back(jet);
      fastjet::ClusterSequence cs(ghosts, jet_def);
      vector<fastjet::PseudoJet> ghost_jets = sorted_by_pt(cs.inclusive_jets());
      constituents = sorted_by_pt(ghost_jets[0].constituents());
      constituents.erase(constituents.begin()); // We erase the jet from constituents
    }

    // Match tracks and ghosts
    vector<fastjet::PseudoJet> selected_tracks;
    if (constituents.size()>0){
      for (auto constituent:constituents){
        for (auto track:tracks){
          float diff_eta = std::fabs(constituent.eta()-track.eta());
          float diff_phi = DeltaPhi(constituent.phi(),track.phi()); // change to DeltaPhi
          if (diff_eta<1e-5 && diff_phi<1e-5){
            selected_tracks.push_back(track);
            break;
          }
        }
      }
    }