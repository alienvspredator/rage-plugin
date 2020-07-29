#pragma once

#include <ccd/entity.h>

namespace CCD {
class Vehicle;
class VehicleComponent;

class VehicleManager : public SmartEntityManager,
                       public rage::IEventHandler,
                       public rage::IVehicleHandler {
 public:
  friend class Vehicle;

  explicit VehicleManager(Plugin *plugin);
  virtual ~VehicleManager() = default;

  void registerDefaultComponent(
      std::function<VehicleComponent *(Vehicle *)> fn);

  Vehicle *getVehicle(uint16_t id) const;
  Vehicle *getVehicle(std::string const &id) const;
  std::map<uint16_t, Vehicle *> const &getVehicles() const;

 protected:
  IVehicleHandler *GetVehicleHandler() override;

  void OnVehicleDeath(rage::IVehicle *vehicle, rage::hash_t hash,
                      rage::IPlayer *killer) override;
  void OnVehicleSirenToggle(rage::IVehicle *vehicle, bool toggle) override;
  void OnVehicleHornToggle(rage::IVehicle *vehicle, bool toggle) override;
  void OnTrailerAttached(rage::IVehicle *vehicle,
                         rage::IVehicle *trailer) override;
  void OnVehicleDamage(rage::IVehicle *vehicle, float bodyHealthLoss,
                       float engineHealthLoss) override;
};

class Vehicle {};
class VehicleComponent {};
}  // namespace CCD
