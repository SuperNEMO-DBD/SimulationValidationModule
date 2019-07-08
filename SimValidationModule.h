//! \file    SimValidationModule.h
//! \brief   Example processing module for flreconstruct
//! \details Process a things object
#ifndef SIMVALMODULE_HH
#define SIMVALMODULE_HH
// Standard Library
// Third Party
#include "TFile.h"
#include "TTree.h"

// - Bayeux
#include "bayeux/dpp/base_module.h"
#include "bayeux/mctools/simulated_data.h"
#include "bayeux/datatools/service_manager.h"
#include "bayeux/geomtools/manager.h"
#include "bayeux/geomtools/geometry_service.h"

// - Falaise


typedef struct ValidationEventStorage{
  // Quantities to histogram (h_)
  int h_calorimeter_hit_count_; // How many calorimeter hits?
  int h_geiger_hit_count_; // How many geiger hits?

}Validationeventstorage;


// This Project
class SimValidationModule : public dpp::base_module {

 public:
  //! Construct module
  SimValidationModule();
  //! Destructor
  virtual ~SimValidationModule();
  //! Configure the module
  virtual void initialize(const datatools::properties& myConfig,
                          datatools::service_manager& flServices,
                          dpp::module_handle_dict_type& moduleDict);
  //! Process supplied data record
  virtual dpp::base_module::process_status process(datatools::things& workItem);
  //! Reset the module
  virtual void reset();
 private:
  TFile* hfile_;
  TTree* tree_;
  ValidationEventStorage validation_;

  // configurable data member
  std::string filename_output_;

  // geometry service
  const geomtools::manager* geometry_manager_; //!< The geometry manager

  // Macro which automatically creates the interface needed
  // to enable the module to be loaded at runtime
  DPP_MODULE_REGISTRATION_INTERFACE(SimValidationModule);
};

#endif // SIMVALMODULE_HH

