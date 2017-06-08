#include "level.h"
#include "graphics.h"
#include "globals.h"
#include <stdio.h>
#include "tinyxml2.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
    _mapName(mapName),
    _spawnPoint(spawnPoint),
    _size(Vector2(0,0)) 
{
        this->loadMap(mapName, graphics);
}

Level::~Level() {

}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());
	XMLElement* mapNode = doc.FirstChildElement("map");

	int width, height;

	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if(pTileset != NULL) {
		while(pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			char* path;
			std::stringstream ss;
			printf(source);
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if(pLayer != NULL) {
		while(pLayer) {
			XMLElement* pData = pLayer->FirstChildElement("data");
			if(pData != NULL) {
				while(pData) {
					XMLElement* pTile = pData->FirstChildElement("tile");
					if(pTile != NULL) {
						int tileCounter = 0;
						while(pTile) {
							if(pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if(pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								} else {
									break;
								}
							}

							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for(int i = 0; i < this->_tilesets.size(); i++) {
								if(this->_tilesets[i].FirstGid <= gid) {
									tls = this->_tilesets.at(i);
									break;
								}
							}

							if(tls.FirstGid == -1) {
								tileCounter++;
								if(pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								} else {
									break;
								}
							}

							int xx = 0, yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;

							yy += tileHeight * 	(tileCounter / width);

							Vector2 finalTilePosition = Vector2(xx, yy);

							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) -1;

							tsxx *= tileWidth;
							int tsyy = 0;
							int amount = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amount;

							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if(pObjectGroup != NULL) {
		while(pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if(ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if(pObject != NULL) {
					while(pObject) {
						float x,y,width,height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");

						this->_collisionRects.push_back(Rectangle(
								std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE,
								std::ceil(width) * globals::SPRITE_SCALE,
								std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			} else if (ss.str() == "spawnPoints") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if(pObject != NULL) {
					while(pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if(ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}

			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}


}

void Level::update(int elapsedTime) {

}

void Level::draw(Graphics &graphics) {
    for(int i = 0; i < this->_tileList.size(); i++) {
    	this->_tileList.at(i).draw(graphics);
    }
}


std::vector<Rectangle> Level::checkTileCollision(const Rectangle &other) {
	std::vector<Rectangle> others;

	for(int i = 0; i < this->_collisionRects.size(); i++) {
		if(this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}


