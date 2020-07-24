#include "entity.h"

CCD::EntityManager::EntityManager(Plugin *plugin) : PluginAttachment(plugin) {}

CCD::Entity *CCD::EntityManager::getEntity(entityId_t id) const {
  try {
    return this->entities.at(id);
  } catch (std::out_of_range const &) {
    return nullptr;
  }
}

std::map<CCD::entityId_t, CCD::Entity *> const &
CCD::EntityManager::getEntities() const {
  return this->entities;
}

void CCD::EntityManager::registerEntity(Entity *entity) {
  this->entities[entity->getID()] = entity;
}

void CCD::EntityManager::unregisterEntity(Entity *entity) {
  this->entities.erase(entity->getID());
}

CCD::SmartEntityManager::SmartEntityManager(Plugin *plugin)
    : EntityManager(plugin) {}

void CCD::SmartEntityManager::registerDefaultComponent(
    std::function<EntityComponent *(SmartEntity *)> const &fn) {
  this->defaultComponents.push_back(fn);
}

std::vector<std::function<CCD::EntityComponent *(CCD::SmartEntity *)>> const &
CCD::SmartEntityManager::getDefaultComponents() const {
  return this->defaultComponents;
}

CCD::Entity::Entity(rage::Entity *handle, EntityManager *manager) {
  this->handle = handle;
  this->manager = manager;

  this->manager->registerEntity(this);
}

CCD::Entity::~Entity() {
  this->manager->unregisterEntity(this);

  switch (this->getType()) {
    case entity_t::Player:
      return;

    default:
      this->getRageMPHandle()->Destroy();
  }
}

CCD::entityId_t CCD::Entity::getID() const {
  return this->getRageMPHandle()->GetId();
}

CCD::entity_t CCD::Entity::getType() const {
  return this->getRageMPHandle()->GetType();
}

void CCD::Entity::setDimension(dimensionId_t id) {
  this->getRageMPHandle()->SetDimension(id);
}

CCD::dimensionId_t CCD::Entity::getDimension() const {
  return this->getRageMPHandle()->GetDimension();
}

CCD::Vector3 CCD::Entity::getPosition() const {
  return Vector3(this->getRageMPHandle()->GetPosition());
}

void CCD::Entity::setPosition(Vector3 const &pos) {
  this->setPosition(pos.x, pos.y, pos.z);
}

void CCD::Entity::setPosition(float x, float z, float y) {
  this->getRageMPHandle()->SetPosition({x, y, z});
}

CCD::Vector3 CCD::Entity::getRotation() const {
  return Vector3(this->getRageMPHandle()->GetRotation());
}

void CCD::Entity::setRotation(Vector3 const &pos) {
  this->setRotation(pos.x, pos.y, pos.z);
}

void CCD::Entity::setRotation(float x, float z, float y) {
  this->getRageMPHandle()->SetRotation({x, y, z});
}

CCD::entityModel_t CCD::Entity::getModel() const {
  return entityModel_t(this->getRageMPHandle()->GetModel());
}

void CCD::Entity::setModel(entityModel_t model) {
  this->getRageMPHandle()->SetModel(uint32_t(model));
}

uint8_t CCD::Entity::getAlpha() const {
  return this->getRageMPHandle()->GetAlpha();
}

void CCD::Entity::setAlpha(uint8_t alpha) {
  this->getRageMPHandle()->SetAlpha(alpha);
}

CCD::EntityManager *CCD::Entity::getManager() const { return this->manager; }

rage::Entity *CCD::Entity::getRageMPHandle() const { return this->handle; }

CCD::SmartEntity::SmartEntity(rage::Entity *handle, SmartEntityManager *manager)
    : Entity(handle, manager) {
  for (auto fn : manager->getDefaultComponents()) fn(this);
}

CCD::SmartEntity::~SmartEntity() {
  for (auto pair : this->components) delete pair.second;
  this->components.clear();
}

CCD::SmartEntityManager *CCD::SmartEntity::getManager() const {
  return (SmartEntityManager *)Entity::getManager();
}

template <typename T>
T *CCD::SmartEntity::getComponent(resourceType_t resourceType) const {
  try {
    return (T *)this->components.at(resourceType);
  } catch (std::out_of_range) {
    return nullptr;
  }
}

std::map<CCD::resourceType_t, CCD::EntityComponent *> const &
CCD::SmartEntity::getComponents() const {
  return this->components;
}

void CCD::SmartEntity::registerComponent(EntityComponent *component) {
  this->components[component->getResource()->getType()] = component;
}

void CCD::SmartEntity::unregisterComponent(EntityComponent *component) {
  this->components.erase(component->getResource()->getType());
}

CCD::EntityComponent::EntityComponent(SmartEntity *entity, Resource *resource) {
  this->entity = entity;
  this->resource = resource;

  this->entity->registerComponent(this);
}

CCD::EntityComponent::~EntityComponent() {
  this->entity->unregisterComponent(this);
}

CCD::SmartEntity *CCD::EntityComponent::getEntity() const {
  return this->entity;
}

CCD::Resource *CCD::EntityComponent::getResource() const {
  return this->resource;
}
