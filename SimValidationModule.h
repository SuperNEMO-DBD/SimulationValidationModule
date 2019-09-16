//! \file    SimValidationModule.h
//! \brief   Example processing module for flreconstruct
//! \details Process a things object
#ifndef SIMVALMODULE_HH
#define SIMVALMODULE_HH
// Standard Library
#include <vector>
#include <string>

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

  int calorimeter_hit_count_; // How many calorimeter hits?
  int geiger_hit_count_; // How many geiger hits?
  double vertex_x_; // Vertex distribution in x-direction.
  double vertex_y_; // Vertex distribution in y-direction.
  double vertex_z_; // Vertex distribution in z-direction.
  std::vector <double> position_start_x_; // Position start in x-direction.
  std::vector <double> position_start_y_; // Position start in y-direction.
  std::vector <double> position_start_z_; // Position start in z-direction.
  std::vector <double> position_stop_x_; // Position stop in x-direction.
  std::vector <double> position_stop_y_; // Position stop in y-direction.
  std::vector <double> position_stop_z_; // Position stop in z-direction.
  std::vector <double> time_start_; // Time Start per entry.
  std::vector <double> time_stop_; // Time Stop per entry.
  std::vector <double> energy_deposit_; // Energy Deposit

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

  // geometry service - placeholder for a future service to request
  // information from the simulation code.
  //  const geomtools::manager* geometry_manager_; //!< The geometry manager

  // Macro which automatically creates the interface needed
  // to enable the module to be loaded at runtime
  DPP_MODULE_REGISTRATION_INTERFACE(SimValidationModule);
};

#endif // SIMVALMODULE_HH


