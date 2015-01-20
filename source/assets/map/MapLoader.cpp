#include "MapLoader.hpp"

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <string>

#include <assets/map/XmlHelper.hpp>
#include <engine/env/Environment.hpp>
#include <engine/trigger/Trigger.hpp>
#include <engine/Light.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/gui/GUIButton.hpp>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>

#include "Player.hpp"

namespace glPortal {
/** \class MapLoader
 *  Load a map in GlPortal XML format.
 */

  Scene* MapLoader::scene;
  XMLHandle MapLoader::rootHandle = XMLHandle(0);

/**
 * Get a scene from a map file in XML format.
 */
Scene* MapLoader::getScene(std::string path) {
  path = Environment::getDataDir() + "maps/" + path + ".xml";
  scene = new Scene();

  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str());

  if (error == XML_NO_ERROR) {
    XMLHandle docHandle(&doc);
    XMLElement* element;

    element = docHandle.FirstChildElement().ToElement();
    rootHandle = XMLHandle(element);

    extractSpawn();
    extractDoor();
    extractModels();
    extractLights();
    extractWalls();
    extractTriggers();
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file. " << endl;
    cout << string(Environment::getDataDir()) << path << endl;
  }
  return scene;
}

/**
 * Extract a spawn element containing its rotation and position elements
 */
void MapLoader::extractSpawn() {
  XMLElement* spawnElement;
  spawnElement = rootHandle.FirstChildElement("spawn").ToElement();

  if (spawnElement) {
    XmlHelper::extractPositionAndRotation(spawnElement, scene->player);
  } else {
    throw std::runtime_error("No spawn position defined.");
  }
}

/**
 * Extract a light elements containing position (x, y, z) and colour (r, g, b) attributes
 */
void MapLoader::extractLights() {
  Vector3f lightPos;
  Vector3f lightColor;
  float distance;
  float energy;
  XMLElement* lightElement;
  lightElement = rootHandle.FirstChildElement("light").ToElement();

  do {
    XmlHelper::pushAttributeVertexToVector(lightElement, lightPos);

    lightElement->QueryFloatAttribute("r", &lightColor.x);
    lightElement->QueryFloatAttribute("g", &lightColor.y);
    lightElement->QueryFloatAttribute("b", &lightColor.z);

    lightElement->QueryFloatAttribute("distance", &distance);
    lightElement->QueryFloatAttribute("energy", &energy);

    Light light;
    light.position.set(lightPos.x, lightPos.y, lightPos.z);
    light.color.set(lightColor.x, lightColor.y, lightColor.z);
    light.distance = distance;
    light.energy = energy;
    scene->lights.push_back(light);
  } while ((lightElement = lightElement->NextSiblingElement("light")) != NULL);
}

void MapLoader::extractDoor() {
  XMLElement* endElement;
  endElement = rootHandle.FirstChildElement("end").ToElement();

  if (endElement) {
    Entity door;
    XmlHelper::extractPositionAndRotation(endElement, door);
    door.texture = TextureLoader::getTexture("Door.png");
    door.mesh = MeshLoader::getMesh("Door.obj");
    scene->end = door;
  } else {
    throw std::runtime_error("No end position defined.");
  }
}

void MapLoader::extractWalls() {
  XMLElement* textureElement = rootHandle.FirstChildElement("texture").ToElement();
  std::string texturePath("none");
  std::string surfaceType("none");

  if (textureElement) {
    do {
      if (textureElement->NoChildren()) {
        continue;
      }
      
      texturePath.assign(textureElement->Attribute("source"));
      //surfaceType.assign(textureElement->Attribute("type"));

      XMLElement* wallBoxElement = textureElement->FirstChildElement("wall");
      
      if (wallBoxElement) {
        do {
          XMLElement* boxPositionElement;
          XMLElement* boxScaleElement;

          Entity wall;
          boxPositionElement = wallBoxElement->FirstChildElement("position");
          XmlHelper::pushAttributeVertexToVector(boxPositionElement, wall.position);

          boxScaleElement = wallBoxElement->FirstChildElement("scale");
          XmlHelper::pushAttributeVertexToVector(boxScaleElement, wall.scale);

          wall.texture = TextureLoader::getTexture(texturePath);
          wall.texture.xTiling = 0.5f;
          wall.texture.yTiling = 0.5f;
          wall.mesh = MeshLoader::getPortalBox(wall);
          scene->walls.push_back(wall);
        } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != NULL);
      }

      texturePath = std::string("none");
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != NULL);
  }
}

void MapLoader::extractTriggers() {
  XMLElement* triggerElement = rootHandle.FirstChildElement("trigger").ToElement();
  string triggerType("none");

  if (triggerElement) {
    do {
      XMLElement* triggerTypeElement;

      Trigger trigger;

      if (triggerElement) {
		trigger.type.assign(triggerElement->Attribute("type"));
      }
      
      if (triggerType == "none") {
        throw std::runtime_error("Trigger must define a type attribute.");
      }

      XmlHelper::pushAttributeVertexToVector(triggerElement->FirstChildElement("position"),
                                             trigger.position);
      XmlHelper::pushAttributeVertexToVector(triggerElement->FirstChildElement("scale"),
                                             trigger.scale);
      trigger.texture = TextureLoader::getTexture("redBox.png");
      trigger.mesh = MeshLoader::getPortalBox(trigger);
      scene->triggers.push_back(trigger);

    } while ((triggerElement = triggerElement->NextSiblingElement()) != NULL);
  }
}

void MapLoader::extractModels() {
  Vector3f modelPos;
  string texture("none");
  string mesh("none");
  XMLElement* modelElement = rootHandle.FirstChildElement("model").ToElement();
  if (modelElement){
    do {
      texture.assign(modelElement->Attribute("texture"));
	  mesh.assign(modelElement->Attribute("mesh"));
      XmlHelper::pushAttributeVertexToVector(modelElement, modelPos);

      Entity model;
      XmlHelper::extractPositionAndRotation(modelElement, model);
      model.texture = TextureLoader::getTexture(texture);
      model.mesh = MeshLoader::getMesh(mesh);
      scene->models.push_back(model);
    } while ((modelElement = modelElement->NextSiblingElement("model")) != NULL);
  }
}


void MapLoader::extractButtons() {
  XMLElement* textureElement = rootHandle.FirstChildElement("texture").ToElement();
  string texturePath("none");
  string surfaceType("none");
  Vector2f position;
  Vector2f size;

  if (textureElement) {
    do {
      texturePath.assign(textureElement->Attribute("source"));
      surfaceType.assign(textureElement->Attribute("type"));
      XMLElement* buttonElement = textureElement->FirstChildElement("GUIbutton");

      if (buttonElement) {
        do {
          GUIButton button;

          buttonElement->QueryFloatAttribute("x", &position.x);
          buttonElement->QueryFloatAttribute("y", &position.y);

          buttonElement->QueryFloatAttribute("w", &size.x);
          buttonElement->QueryFloatAttribute("h", &size.y);

          button.texture = TextureLoader::getTexture(texturePath);
          button.texture.xTiling = 0.5f;
          button.texture.yTiling = 0.5f;
          scene->buttons.push_back(button);
        } while ((buttonElement = buttonElement->NextSiblingElement("GUIbutton")) != NULL);
      }

      texturePath = std::string("none");
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != NULL);
  }
}
} /* namespace glPortal */
