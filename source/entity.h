#pragma once

#include "pluginAttachment.h"
#include "resource.h"
#include "vector3.h"

namespace CCD {
typedef rage::entityId_t entityId_t;
typedef rage::entity_t entity_t;
typedef uint32_t entityModel_t;
typedef uint32_t dimensionId_t;

class SmartEntity;
class Entity;
class EntityComponent;

class EntityManager : public PluginAttachment {
 public:
  explicit EntityManager(Plugin *plugin);
  virtual ~EntityManager() = default;

  Entity *getEntity(entityId_t id) const;
  std::map<entityId_t, Entity *> const &getEntities() const;

 protected:
  friend class Entity;

  void registerEntity(Entity *entity);
  void unregisterEntity(Entity *entity);

 private:
  std::map<entityId_t, Entity *> entities;
};

class SmartEntityManager : public EntityManager {
 public:
  explicit SmartEntityManager(Plugin *plugin);
  virtual ~SmartEntityManager() = default;

  void registerDefaultComponent(
      std::function<EntityComponent *(SmartEntity *)> const &fn);

  std::vector<std::function<EntityComponent *(SmartEntity *)>> const &
  getDefaultComponents() const;

 private:
  std::vector<std::function<EntityComponent *(SmartEntity *)>>
      defaultComponents;
};

class Entity {
 public:
  Entity(rage::Entity *handle, EntityManager *manager);
  virtual ~Entity();

  entityId_t getID() const;
  entity_t getType() const;

  void setDimension(dimensionId_t id);
  dimensionId_t getDimension() const;

  Vector3 getPosition() const;
  void setPosition(Vector3 const &pos);
  void setPosition(float x, float y, float z);

  Vector3 getRotation() const;
  void setRotation(Vector3 const &pos);
  void setRotation(float x, float y, float z);

  entityModel_t getModel() const;
  void setModel(entityModel_t model);

  uint8_t getAlpha() const;
  void setAlpha(uint8_t alpha);

  virtual EntityManager *getManager() const;

 protected:
  virtual rage::Entity *getRageMPHandle() const;

 private:
  EntityManager *manager;
  rage::Entity *handle;
};

class SmartEntity : public Entity {
 public:
  SmartEntity(rage::Entity *handle, SmartEntityManager *manager);
  virtual ~SmartEntity();

  virtual SmartEntityManager *getManager() const;

  template <typename T>
  T *getComponent(resourceType_t resourceType) const;
  std::map<resourceType_t, EntityComponent *> const &getComponents() const;

 protected:
  friend class EntityComponent;

  void registerComponent(EntityComponent *component);
  void unregisterComponent(EntityComponent *component);

 private:
  std::map<resourceType_t, EntityComponent *> components;
};

class EntityComponent {
 public:
  EntityComponent(SmartEntity *entity, Resource *resource);
  virtual ~EntityComponent();

  SmartEntity *getEntity() const;
  Resource *getResource() const;

 private:
  SmartEntity *entity;
  Resource *resource;
};
}  // namespace CCD
