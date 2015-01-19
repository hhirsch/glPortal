#ifndef MAPLOADER_HPP
#define MAPLOADER_HPP

#include <string>
#include <tinyxml2.h>

using namespace tinyxml2;

namespace glPortal {
class Entity;
class Mesh;
class Scene;

class MapLoader {
public:
  static Scene* getScene(std::string path);
  static Scene* scene;
  static XMLHandle rootHandle;
private:
  static Mesh getBox(Entity wall);
  static void extractSpawn();
  static void extractLights();
  static void extractDoor();
  static void extractWalls();
  static void extractTriggers();
  static void extractModels();
  static void extractButtons();
};

} /* namespace glPortal */

#endif /* MAPLOADER_HPP */
