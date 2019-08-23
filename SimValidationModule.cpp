#include "SimValidationModule.h"

//Standard Library
#include <iostream>

// Bayeux:
#include "bayeux/mctools/utils.h"

int mainWallHitType=1302;
int xWallHitType=1232;
int gammaVetoHitType=1252;


DPP_MODULE_REGISTRATION_IMPLEMENT(SimValidationModule,"SimValidationModule");

SimValidationModule::SimValidationModule() : dpp::base_module()
{
  filename_output_="SimValidation.root";
}

SimValidationModule::~SimValidationModule() {
  if (is_initialized()) this->reset();
}

void SimValidationModule::initialize(const datatools::properties& myConfig,
                                   datatools::service_manager& flServices,
                                   dpp::module_handle_dict_type& /*moduleDict*/){

  // Look for services
  if (flServices.has("geometry")) {
    const geomtools::geometry_service& GS = flServices.get<geomtools::geometry_service> ("geometry");
    geometry_manager_ = &GS.get_geom_manager();
    DT_THROW_IF(!geometry_manager_,
                std::runtime_error,
                "Null pointer to geometry manager return by geometry_service");
  }
  // Extract the filename_out key from the supplied config, if
  // the key exists. datatools::properties throws an exception if
  // the key isn't in the config, so catch this if thrown and don't do
  // anything
  try {
    myConfig.fetch("filename_out",this->filename_output_);
  } catch (std::logic_error& e) {
  }

  // Use the method of PTD2ROOT to create a root file with just the branches we need for the Validation analysis


  hfile_ = new TFile(filename_output_.c_str(),"RECREATE","Output file of Simulation data");
  hfile_->cd();
  tree_ = new TTree("SimValidation","Simulation Validation");
  tree_->SetDirectory(hfile_);
 
  // Some basic counts
  tree_->Branch("calorimeter_hit_count",&validation_.calorimeter_hit_count_);
  tree_->Branch("geiger_hit_count",&validation_.geiger_hit_count_);
  tree_->Branch("vertex_x", &validation_.vertex_x_);
  tree_->Branch("vertex_y", &validation_.vertex_y_);
  tree_->Branch("vertex_z", &validation_.vertex_z_);
  tree_->Branch("energy_deposit", &validation_.energy_deposit_);
  tree_->Branch("position_start_x", &validation_.position_start_x_);
  tree_->Branch("position_start_y", &validation_.position_start_y_);
  tree_->Branch("position_start_z", &validation_.position_start_z_);
  tree_->Branch("position_stop_x", &validation_.position_stop_x_);
  tree_->Branch("position_stop_y", &validation_.position_stop_y_);
  tree_->Branch("position_stop_z", &validation_.position_stop_z_);
  tree_->Branch("time_start", &validation_.time_start_);
  tree_->Branch("time_stop", &validation_.time_stop_);

  this->_set_initialized(true);
}
//! [SimValidationModule::Process]
dpp::base_module::process_status
SimValidationModule::process(datatools::things& workItem) {
  
  
  // declare internal variables to mimic the ntuple variables, names are same but in camel case
  validation_.position_start_x_.clear();
  validation_.position_start_y_.clear();
  validation_.position_start_z_.clear();
  validation_.position_stop_x_.clear();
  validation_.position_stop_y_.clear();
  validation_.position_stop_z_.clear();
  validation_.time_start_.clear();
  validation_.time_stop_.clear();
  validation_.energy_deposit_.clear();
  validation_.calorimeter_hit_count_ = 0;
  validation_.geiger_hit_count_ = 0;
  validation_.vertex_x_ = 0;
  validation_.vertex_y_ = 0;
  validation_.vertex_z_ = 0;

  // Grab simulated data bank
  try {
    const mctools::simulated_data& SD = workItem.get<mctools::simulated_data>("SD");

    validation_.vertex_x_ = SD.get_vertex().x();
    validation_.vertex_y_ = SD.get_vertex().y();
    validation_.vertex_z_ = SD.get_vertex().z();

    if (SD.has_step_hits("gg")) 
      validation_.geiger_hit_count_ = SD.get_number_of_step_hits("gg");;

    
    if (SD.has_step_hits("calorimeter")) 
      validation_.calorimeter_hit_count_ += SD.get_number_of_step_hits("calorimeter");

    if (SD.has_step_hits("xcalo")) 
      validation_.calorimeter_hit_count_ += SD.get_number_of_step_hits("xcalo");

    if (SD.has_step_hits("gveto")) 
      validation_.calorimeter_hit_count_ += SD.get_number_of_step_hits("gveto");

    if (SD.has_step_hits("__visu.tracks")) {
      const auto &rawSteps = SD.get_step_hits("__visu.tracks");
      for (const datatools::handle<mctools::base_step_hit> &hit : rawSteps) {
        validation_.energy_deposit_.push_back((hit.get()).get_energy_deposit() / CLHEP::keV);
        validation_.time_start_.push_back((hit.get()).get_time_start());
        validation_.time_stop_.push_back((hit.get()).get_time_stop());
        validation_.position_start_x_.push_back((hit.get()).get_position_start().x());
        validation_.position_start_y_.push_back((hit.get()).get_position_start().y()); 
        validation_.position_start_z_.push_back((hit.get()).get_position_start().z()); 
        validation_.position_stop_x_.push_back((hit.get()).get_position_stop().x()); 
        validation_.position_stop_y_.push_back((hit.get()).get_position_stop().y()); 
        validation_.position_stop_z_.push_back((hit.get()).get_position_stop().z()); 
      }
    }
  }
  catch (std::logic_error& e) {
    std::cerr << "failed to grab SD bank : " << e.what() << std::endl;
    return dpp::base_module::PROCESS_INVALID;
  }// end try on SD bank

  tree_->Fill();

  // MUST return a status, see ref dpp::processing_status_flags_type
  return dpp::base_module::PROCESS_OK;
}


//! [SimValidationModule::reset]
void SimValidationModule::reset() {
  hfile_->cd();
  tree_->Write();
  hfile_->Close(); //
  std::cout << "In reset: finished conversion, file closed " << std::endl;

  // clean up
  delete hfile_;
  filename_output_ = "SimValidation.root";
  this->_set_initialized(false);

}

