#include "SimValidationModule.h"

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
  tree_->Branch("h_calorimeter_hit_count",&validation_.h_calorimeter_hit_count_);
  tree_->Branch("h_geiger_hit_count",&validation_.h_geiger_hit_count_);
  
  this->_set_initialized(true);
}
//! [SimValidationModule::Process]
dpp::base_module::process_status
SimValidationModule::process(datatools::things& workItem) {
  
  
  // declare internal variables to mimic the ntuple variables, names are same but in camel case
  int caloHitCount = 0;
  int geigerHitCount = 0;


  // Grab simulated data bank
  try {
    const mctools::simulated_data& SD = workItem.get<mctools::simulated_data>("SD");

    if (SD.has_step_hits("gg")) 
      geigerHitCount = SD.get_number_of_step_hits("gg");;

    
    if (SD.has_step_hits("calorimeter")) 
      caloHitCount += SD.get_number_of_step_hits("calorimeter");

    if (SD.has_step_hits("xcalo")) 
      caloHitCount += SD.get_number_of_step_hits("xcalo");

    if (SD.has_step_hits("gveto")) 
      caloHitCount += SD.get_number_of_step_hits("gveto");

  }
  catch (std::logic_error& e) {
    std::cerr << "failed to grab SD bank : " << e.what() << std::endl;
    return dpp::base_module::PROCESS_INVALID;
  }// end try on SD bank

  // Counts
  validation_.h_calorimeter_hit_count_=caloHitCount;
  validation_.h_geiger_hit_count_=geigerHitCount;
  
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

