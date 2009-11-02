
#include "scene.h"

#include "viewport.h"
#include "rect.h"

Scene::Scene() : Registrable("Scene"), background(0) {
  area = new Rect(
      VirtualPosition(0, 0),
      Game::getInstance().getViewport().getVirtualSize()
      );
}

Scene::~Scene() {
  delete area;
}


