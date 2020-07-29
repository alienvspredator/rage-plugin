#pragma once

#include <ccd/callable.h>
#include <ccd/entity.h>
// #include <ccd/vehicle.h>

namespace CCD {
class Player;
class PlayerComponent;
class PlayerEvent;
class PlayerCommand;

class PlayerManager : public SmartEntityManager,
                      public rage::IEventHandler,
                      public rage::IPlayerHandler,
                      public rage::ICheckpointHandler {
 public:
  explicit PlayerManager(Plugin *plugin);
  ~PlayerManager() override = default;

  void registerDefaultComponent(std::function<PlayerComponent *(Player *)> fn);

  virtual IPlayerHandler *GetPlayerHandler() override;
  void OnPlayerJoin(rage::IPlayer *player) override;
  void OnPlayerReady(rage::IPlayer *player) override;
  void OnPlayerQuit(rage::IPlayer *player, rage::exit_t exitType,
                    const char *reason) override;

  void OnPlayerCommand(rage::IPlayer *player,
                       const std::u16string &command) override;
  void OnPlayerChat(rage::IPlayer *player, const std::u16string &text) override;

  void OnPlayerDeath(rage::IPlayer *player, rage::hash_t reason,
                     rage::IPlayer *killer) override;
  void OnPlayerSpawn(rage::IPlayer *player) override;

  void OnPlayerRemoteEvent(rage::IPlayer *player, uint64_t eventName,
                           const rage::args_t &args) override;

  void OnPlayerStartEnterVehicle(rage::IPlayer *player, rage::IVehicle *vehicle,
                                 uint8_t seatID) override;
  void OnPlayerEnterVehicle(rage::IPlayer *player, rage::IVehicle *vehicle,
                            uint8_t seatID) override;
  void OnPlayerStartExitVehicle(rage::IPlayer *player,
                                rage::IVehicle *vehicle) override;
  void OnPlayerExitVehicle(rage::IPlayer *player,
                           rage::IVehicle *vehicle) override;

  ICheckpointHandler *GetCheckpointHandler() override;
  void OnPlayerEnterCheckpoint(rage::IPlayer *player,
                               rage::ICheckpoint *checkpoint) override;
  void OnPlayerExitCheckpoint(rage::IPlayer *player,
                              rage::ICheckpoint *checkpoint) override;

  void sendGlobalMessage(std::string const &message);

  std::vector<Player *> getPlayersInRange(Vector3 const &pos, float range);
  Player *getPlayer(
      uint16_t id) const;  // If player is not found it returns nullptr
  Player *getPlayer(std::string const &name) const;
  std::map<uint16_t, Player *> const &getPlayers() const;
};

struct ComponentVariation {
  uint16_t drawable;
  uint8_t texture;
  uint8_t pallete;

  ComponentVariation(uint16_t drawable, uint8_t texture, uint8_t pallete) {
    this->drawable = drawable;
    this->texture = texture;
    this->pallete = pallete;
  }
};

struct HeadOverlay {
  uint8_t value;
  uint8_t color;
  uint8_t secondaryColor;
  float opacity;

  HeadOverlay(uint8_t value, float opacity, uint8_t color,
              uint8_t secondaryColor) {
    this->value = value;
    this->color = color;
    this->opacity = opacity;
    this->secondaryColor = secondaryColor;
  }
};

class Player : public SmartEntity {
 public:
  friend class PlayerManager;
  // friend class Checkpoint;

  rage::IPlayer *getHandle() const;

  enum class Gender : uint8_t { GENDER_FEMALE, GENDER_MALE, GENDER_BIMALE }
  ;

  Player(PlayerManager *manager, rage::IPlayer *player);

  std::string const &getHWID() const;
  std::string const &getSocialClubName() const;

  // Appearance
  void setHeadOverlay(uint8_t id, HeadOverlay const &overlay);
  void setHeadOverlay(uint8_t id, uint8_t value, uint8_t color,
                      float opacity = 1);
  void setHeadOverlay(uint8_t id, uint8_t value, uint8_t color,
                      uint8_t secondaryColor, float opacity = 1);
  uint8_t getHeadOverlay(uint8_t id) const;
  float getHeadOverlayOpacity(uint8_t id) const;
  uint8_t getHeadOverlayColor(uint8_t id) const;
  uint8_t getHeadOverlaySecondaryColor(uint8_t id) const;

  void setPropertyVariation(uint8_t id, uint8_t drawable, uint8_t texture);
  uint8_t getPropertyDrawable(uint8_t id) const;
  uint8_t getPropertyTexture(uint8_t id) const;

  void setComponentVariation(uint8_t id, uint16_t drawable, uint8_t texture,
                             uint8_t palette);
  void setComponentVariation(uint8_t id, ComponentVariation const &data);
  uint16_t getComponentDrawable(uint8_t id) const;
  uint8_t getComponentTexture(uint8_t id) const;
  uint8_t getComponentPalette(uint8_t id) const;
  ComponentVariation const &getComponentVariation(uint8_t id) const;
  std::array<ComponentVariation, 12> const &getComponentVariations() const;

  nlohmann::json &getCustomDataReference();
  void updateCustomData() const;

  void setGender(Gender gender);
  Gender getGender() const;

  void setFaceShape(uint8_t faceShape);
  uint8_t getFaceShape() const;

  void setSkinColor(float color);
  float getSkinColor() const;

  void setEyeColor(float color);
  float getEyeColor() const;

  void setHaircut(uint8_t id);
  uint8_t getHaircut() const;

  void setHairColor(uint8_t color);
  uint8_t setHairColor() const;

  void setHairHighlightColor(uint8_t color);
  uint8_t getHairHighlightColor() const;

  void setEyeBrows(uint8_t id);
  uint8_t getEyeBrows() const;

  void setEyeBrowsColor(uint8_t color);
  uint8_t getEyeBrowsColor() const;

  void setBeard(uint8_t id);
  uint8_t getBeard() const;

  void setBeardColor(uint8_t color);
  uint8_t getBeardColor() const;

  void setFaceBlemishes(uint8_t id);
  uint8_t getFaceBlemishes() const;

  void setFaceAge(uint8_t id);
  uint8_t getFaceAge() const;

  void setFaceComplexion(uint8_t id);
  uint8_t getFaceComplexion() const;

  void setFaceFreckles(uint8_t id);
  uint8_t getFaceFreckles() const;

  void setFaceSunDamage(uint8_t id);
  uint8_t getFaceSunDamage() const;

  void setChestHair(uint8_t id);
  uint8_t getChestHair() const;

  void setChestHairColor(uint8_t color);
  uint8_t getChestHairColor() const;

  void setBodyBlemishes(uint8_t id);
  uint8_t getBodyBlemishes() const;

  void setDenseBodyBlemishes(uint8_t id);
  uint8_t getDenseBodyBlemishes() const;

  void setFaceFeature(uint8_t id, float value);
  float getFaceFeature(uint8_t id) const;

  void kick(std::string const &reason);
  void ban(std::string const &reason);

  void sendMessage(std::string const &message);
  void sendLocalMessage(std::string const &message, float range);
  void sendGlobalMessage(std::string const &message);

  void sendNotification(std::string const &message);

  std::string const &getName() const;
  void setName(std::string const &name);

  void requestIPL(std::string const &ipl);
  void removeIPL(std::string const &ipl);

  float getHeading() const;
  void setHeading(float heading);

  Transform getTransform();
  void setTransform(Transform const &transform);
  void setTransform(float x, float y, float z, float h);

  float getHealth() const;
  void setHealth(float health);
  bool isDead() const;

  float getArmour() const;
  void setArmour(float armour) const;

  void revive(Vector3 const &pos, float h);
  void revive(float x, float y, float z, float h);

  bool isInVehicle() const;
  Vehicle *getVehicle() const;
  int8_t getSeat() const;
  bool isInDriverSeat() const;
  void setVehicle(Vehicle *vehicle, int8_t seat = -1);
  void removeFromVehicle();

  void connectVoiceTo(Player *player);
  void disconnectVoiceFrom(Player *player);
  std::vector<Player *> getConnectedPlayersByVoice() const;

  std::string getIp() const;
  int getPing() const;
  float getPacketLoss() const;

  uint32_t getActiveWeapon() const;
  void setActiveWeapon(uint32_t weapon);
  uint16_t getActiveWeaponAmmo() const;
  void setActiveWeaponAmmo(uint16_t ammo);

  uint16_t getWeaponAmmo(uint32_t weapon) const;
  void setWeaponAmmo(uint32_t weapon, uint16_t ammo);

  std::map<uint32_t, uint16_t> getWeapons() const;
  void giveWeapon(uint32_t weapon, uint16_t ammo);
  void removeWeapon(uint32_t weapon);
  void removeWeapons() const;

  template <typename... Args>
  void callClientEvent(const std::string &eventName, Args &&... args) {
    const int count = sizeof...(Args);

    if constexpr (count == 0) {
      this->getHandle()->_Call(eventName);
    } else {
      rage::arg_t arguments[count] = {rage::arg_t(std::forward<Args>(args))...};
      this->getHandle()->_Call(eventName, arguments, count);
    }
  }

  std::vector<Player *> findClosestPlayers(float maxDist);
  Player *findClosestPlayer(float maxDist = 999.0f);

  PlayerManager *getManager() const;

 private:
  void OnReady();
  void OnDisconnect(rage::exit_t exitType, const char *reason);
  void OnDeath(rage::hash_t reason, Player *killer);
  void OnSpawn();
  void OnMessage(const std::string &text);
  void OnStartEnterVehicle(Vehicle *vehicle, uint8_t seat);
  void OnEnterVehicle(Vehicle *vehicle, uint8_t seat);
  void OnStartExitVehicle(Vehicle *vehicle);
  void OnExitVehicle(Vehicle *vehicle);
  void OnRemoteEvent(uint64_t eventName, std::vector<std::string> const &args);
  void OnCommand(std::string const &cmd, std::string const &args);
  // void OnEnterCheckPoint(CheckPoint *checkPoint);
  // void OnExitCheckPoint(CheckPoint *checkPoint);

  void initSyncedData();

  void setHeadBlend(uint8_t faceShape, float skinColor);

  float skinColor = 0;
  bool isConnected = true;

  std::array<ComponentVariation, 12> componentVariations = {
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0),
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0),
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0),
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0),
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0),
      ComponentVariation(0, 0, 0), ComponentVariation(0, 0, 0)};
  std::array<HeadOverlay, 13> headOverlays = {
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0), HeadOverlay(255, 1, 0, 0),
      HeadOverlay(255, 1, 0, 0)};

  nlohmann::json customData;
};

class PlayerComponent : public EntityComponent {
 public:
  friend class Player;
  friend class PlayerEvent;
  friend class PlayerCommand;

  PlayerComponent(Player *player, Resource *resource);
  virtual ~PlayerComponent() = default;

  Player *getPlayer() const;
  std::map<uint64_t, PlayerEvent *> const &getEvents() const;
  std::map<std::string, PlayerCommand *> const &getCommands() const;

 protected:
  virtual void OnInitialized(){};

  // This one is called always
  virtual void OnDisconnect(){};

  // This one is called before onDisconnect and only if player timeouts
  virtual void OnTimeout(){};

  // Same as timeout but only if player gets kicked.
  virtual void OnKicked(std::string const &reason){};

  virtual void OnDeath(rage::hash_t reason, Player *killer){};
  virtual void OnSpawn(){};
  virtual void OnMessage(std::string const &text){};
  virtual void OnStartEnterVehicle(Vehicle *vehicle, uint8_t seat){};
  virtual void OnEnterVehicle(Vehicle *vehicle, uint8_t seat){};
  virtual void OnStartExitVehicle(Vehicle *vehicle){};
  virtual void OnExitVehicle(Vehicle *vehicle){};
  // virtual void OnEnterCheckPoint(Checkpoint *checkpoint){};
  // virtual void OnExitCheckPoint(Checkpoint *checkpoint){};

 private:
  std::map<uint64_t, PlayerEvent *> events;
  std::map<std::string, PlayerCommand *> commands;

  void registerEvent(PlayerEvent *event);
  void registerCommand(PlayerCommand *command, std::string const &name);
};

class PlayerEvent : public Callable<void> {
 public:
  PlayerEvent(PlayerComponent *c, std::string const &eventName,
              std::function<void(std::vector<ArgumentValue> const &)> fn,
              std::vector<IArgument *> const &mandatoryArgs,
              std::vector<IArgument *> const &optionalArgs);
};
}  // namespace CCD
