	//Get cambridge jets and substructure
	if (selected_tracks.size()>0){
		JetDefinition cambridge_def(fastjet::cambridge_algorithm, 0.4); // Cambridge algorithm with R=0.4
		cambridge_cs_ptr = std::make_unique<ClusterSequence>(selected_tracks, cambridge_def);
	    //exclusive jets - exclusive (only 1 jet after clustering)
		vector<PseudoJet> cambridge_jets = sorted_by_pt(cambridge_cs_ptr->exclusive_jets(1));

		// Process the resulting Cambridge jets with Soft Drop

		if (!cambridge_jets.empty()){
    		PseudoJet cambridge_jet = cambridge_jets[0];
			// Create Soft Drop groomer
			contrib::SoftDrop soft_drop(beta, zcut, R0);

			ca_constituents = cambridge_jet.constituents();

			// Apply Soft Drop to find first hard splitting
			PseudoJet groomed_jet = soft_drop(cambridge_jet);
			vector<fastjet::PseudoJet> groomed_constituents;
			float dR=0;
			if (groomed_jet != 0){ 
				groomed_constituents = groomed_jet.constituents();
				dR = groomed_jet.structure_of<contrib::SoftDrop>().delta_R();
			}
		}
	}