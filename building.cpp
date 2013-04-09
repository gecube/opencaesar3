// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com



#include "building.hpp"

#include <iostream>
#include <algorithm>

#include "scenario.hpp"
#include "walker.hpp"
#include "exception.hpp"
#include "gui_info_box.hpp"
#include "building_data.hpp"
#include "factory_building.hpp"
#include "service_building.hpp"
#include "training_building.hpp"
#include "warehouse.hpp"
#include "gettext.hpp"

namespace {
static const char* rcUtilityGroup      = "utilitya";
static const char* rcAqueductGroup     = "land2a";
static const char* rcRoadGroup         = "land2a";
static const char* rcCommerceGroup     = "commerce";
static const char* rcHousingGroup      = "housng1a";
static const char* rcSecurityGroup     = "security";
static const char* rcGovernmentGroup   = "govt";
static const char* rcEntertaimentGroup = "entertainment";
}

std::map<BuildingType, LandOverlay*> LandOverlay::_mapBuildingByID;  // key=buildingType, value=instance

LandOverlay::LandOverlay()
{
   _master_tile = NULL;
   _size = 1;
   _isDeleted = false;
   _name = "unknown";
   _picture = NULL;
}

LandOverlay::~LandOverlay()
{
  // what we shall to do here?
}


BuildingType LandOverlay::getType() const
{
   return _buildingType;
}

void LandOverlay::setType(const BuildingType buildingType)
{
   _buildingType = buildingType;
   _name = BuildingDataHolder::instance().getData(buildingType).getName();
}

void LandOverlay::timeStep(const unsigned long time) { }

void LandOverlay::setPicture(Picture &picture)
{
   Tilemap &tilemap = Scenario::instance().getCity().getTilemap();

   _picture = &picture;

   if (_master_tile != NULL)
   {
      // _master_tile == NULL is cloneable buildings
      int i = _master_tile->getI();
      int j = _master_tile->getJ();

      for (int dj = 0; dj<_size; ++dj)
      {
         for (int di = 0; di<_size; ++di)
         {
            Tile &tile = tilemap.at(i+di, j+dj);
            tile.set_picture(_picture);
         }
      }
   }
}

void LandOverlay::build(const int i, const int j)
{
   City &city = Scenario::instance().getCity();
   Tilemap &tilemap = city.getTilemap();

   _master_tile = &tilemap.at(i, j);

   for (int dj = 0; dj<_size; ++dj)
   {
      for (int di = 0; di<_size; ++di)
      {
         Tile &tile = tilemap.at(i+di, j+dj);
         tile.set_master_tile(_master_tile);
         tile.set_picture(_picture);
         TerrainTile &terrain = tile.get_terrain();
         terrain.setOverlay(this);
         setTerrain(terrain);
      }
   }
}

void LandOverlay::destroy()
{
}

Tile& LandOverlay::getTile() const
{
   return *_master_tile;
}

int LandOverlay::getSize() const
{
   return _size;
}

bool LandOverlay::isDeleted() const
{
   return _isDeleted;
}

Picture& LandOverlay::getPicture()
{
   if (_picture == NULL)
   {
      THROW("Picture is NULL!");
   }
   return *_picture;
}

std::vector<Picture*>& LandOverlay::getForegroundPictures()
{
   return _fgPictures;
}


GuiInfoBox* LandOverlay::makeInfoBox()
{
  std::cout << "LandOverlay::makeInfoBox()" << std::endl;
  return NULL;
}

std::string LandOverlay::getName()
{
   return _name;
}

void LandOverlay::serialize(OutputSerialStream &stream)
{
   stream.write_objectID(this);
   stream.write_int((int) _buildingType, 1, 0, B_MAX);
   stream.write_int(getTile().getI(), 2, 0, 1000);
   stream.write_int(getTile().getJ(), 2, 0, 1000);
}

LandOverlay& LandOverlay::unserialize_all(InputSerialStream &stream)
{
   int objectID = stream.read_objectID();
   BuildingType buildingType = (BuildingType) stream.read_int(1, 0, B_MAX);
   int i = stream.read_int(2, 0, 1000);
   int j = stream.read_int(2, 0, 1000);
   LandOverlay *instance = getInstance(buildingType);
   LandOverlay *res = instance->clone();
   res->_master_tile = &Scenario::instance().getCity().getTilemap().at(i, j);
   res->unserialize(stream);
   stream.link(objectID, res);
   return *res;
}

void LandOverlay::unserialize(InputSerialStream &stream)
{
}

LandOverlay* LandOverlay::getInstance(const BuildingType buildingType)
{
   if (_mapBuildingByID.empty())
   {
      // first call to this method

      // entertainment
      _mapBuildingByID[B_THEATER]      = new BuildingTheater();
      _mapBuildingByID[B_AMPHITHEATER] = new BuildingAmphiTheater();
      _mapBuildingByID[B_COLLOSSEUM]   = new BuildingCollosseum();
      _mapBuildingByID[B_ACTOR]        = new BuildingActor();
      _mapBuildingByID[B_GLADIATOR]    = new BuildingGladiator();
      _mapBuildingByID[B_LION]         = new BuildingLion();
      _mapBuildingByID[B_CHARIOT]      = new BuildingChariot();
      _mapBuildingByID[B_HIPPODROME]   = new BuildingHippodrome();
      // road&house
      _mapBuildingByID[B_HOUSE] = new House( House::smallHovel );
      _mapBuildingByID[B_ROAD]  = new Road();
      // administration
      _mapBuildingByID[B_FORUM]  = new Forum();
      _mapBuildingByID[B_SENATE] = new Senate();
      _mapBuildingByID[B_GOVERNOR_HOUSE]  = new GovernorsHouse();
      _mapBuildingByID[B_GOVERNOR_VILLA]  = new GovernorsVilla();
      _mapBuildingByID[B_GOVERNOR_PALACE] = new GovernorsPalace(); 
      _mapBuildingByID[B_STATUE1] = new SmallStatue(); 
      _mapBuildingByID[B_STATUE2] = new MediumStatue(); 
      _mapBuildingByID[B_STATUE3] = new BigStatue();
      _mapBuildingByID[B_GARDEN]  = new Garden();
      _mapBuildingByID[B_PLAZA]   = new Plaza();
      // water
      _mapBuildingByID[B_WELL]      = new BuildingWell();
      _mapBuildingByID[B_FOUNTAIN]  = new BuildingFountain();
      _mapBuildingByID[B_AQUEDUCT]  = new Aqueduct();
      _mapBuildingByID[B_RESERVOIR] = new Reservoir();
      // security
      _mapBuildingByID[B_PREFECT]   = new BuildingPrefect();
      _mapBuildingByID[B_FORT_LEGIONNAIRE] = new FortLegionnaire();
      _mapBuildingByID[B_FORT_JAVELIN]     = new FortJaveline();
      _mapBuildingByID[B_FORT_MOUNTED]     = new FortMounted();
      _mapBuildingByID[B_MILITARY_ACADEMY] = new Academy();
      _mapBuildingByID[B_BARRACKS]         = new Barracks();
      // commerce
      _mapBuildingByID[B_MARKET]    = new Market();
      _mapBuildingByID[B_WAREHOUSE] = new Warehouse();
      _mapBuildingByID[B_GRANARY]   = new Granary();
      // farms
      _mapBuildingByID[B_WHEAT]     = new FarmWheat();
      _mapBuildingByID[B_OLIVE]     = new FarmOlive();
      _mapBuildingByID[B_GRAPE]     = new FarmGrape();
      _mapBuildingByID[B_MEAT]      = new FarmMeat();
      _mapBuildingByID[B_FRUIT]     = new FarmFruit();
      _mapBuildingByID[B_VEGETABLE] = new FarmVegetable();
      // raw materials
      _mapBuildingByID[B_IRON]   = new FactoryIron();
      _mapBuildingByID[B_TIMBER] = new FactoryTimber();
      _mapBuildingByID[B_CLAY]   = new FactoryClay();
      _mapBuildingByID[B_MARBLE] = new FactoryMarble();
      // factories
      _mapBuildingByID[B_WEAPON]    = new FactoryWeapon();
      _mapBuildingByID[B_FURNITURE] = new FactoryFurniture();
      _mapBuildingByID[B_WINE]      = new FactoryWine();
      _mapBuildingByID[B_OIL]       = new FactoryOil();
      _mapBuildingByID[B_POTTERY]   = new FactoryPottery();
      // utility
      _mapBuildingByID[B_ENGINEER] = new BuildingEngineer();
      _mapBuildingByID[B_DOCK]     = new Dock();
      _mapBuildingByID[B_SHIPYARD] = new Shipyard();
      _mapBuildingByID[B_WHARF]    = new Wharf();
      _mapBuildingByID[B_TRIUMPHAL_ARCH]  = new TriumphalArch();
      // religion
      _mapBuildingByID[B_TEMPLE_CERES]   = new TempleCeres();
      _mapBuildingByID[B_TEMPLE_NEPTUNE] = new TempleNeptune();
      _mapBuildingByID[B_TEMPLE_MARS]    = new TempleMars();
      _mapBuildingByID[B_TEMPLE_VENUS]   = new TempleVenus();
      _mapBuildingByID[B_TEMPLE_MERCURE] = new TempleMercure();
      _mapBuildingByID[B_BIG_TEMPLE_CERES]   = new BigTempleCeres();
      _mapBuildingByID[B_BIG_TEMPLE_NEPTUNE] = new BigTempleNeptune();
      _mapBuildingByID[B_BIG_TEMPLE_MARS]    = new BigTempleMars();
      _mapBuildingByID[B_BIG_TEMPLE_VENUS]   = new BigTempleVenus();
      _mapBuildingByID[B_BIG_TEMPLE_MERCURE] = new BigTempleMercure();
      _mapBuildingByID[B_TEMPLE_ORACLE]  = new TempleOracle();
      // health
      _mapBuildingByID[B_BATHS]    = new Baths();
      _mapBuildingByID[B_BARBER]   = new Barber();
      _mapBuildingByID[B_DOCTOR]   = new Doctor();
      _mapBuildingByID[B_HOSPITAL] = new Hospital();
      // education
      _mapBuildingByID[B_SCHOOL]  = new School();
      _mapBuildingByID[B_LIBRARY] = new Library();
      _mapBuildingByID[B_COLLEGE] = new College();
      _mapBuildingByID[B_MISSION_POST] = new MissionPost();
      // natives
      _mapBuildingByID[B_NATIVE_HUT]    = new NativeHut();
      _mapBuildingByID[B_NATIVE_CENTER] = new NativeCenter();
      _mapBuildingByID[B_NATIVE_FIELD]  = new NativeField();
   }

   std::map<BuildingType, LandOverlay*>::iterator mapIt;
   mapIt = _mapBuildingByID.find(buildingType);
   LandOverlay *res;

   if (mapIt == _mapBuildingByID.end())
   {
      // THROW("Unknown building type:" << buildingType);
      res = NULL;
   }
   else
   {
      res = mapIt->second;
   }
   return res;
}



Construction::Construction()
{
}

bool Construction::canBuild(const int i, const int j) const
{
   Tilemap& tilemap = Scenario::instance().getCity().getTilemap();

   bool is_constructible = true;

   std::list<Tile*> rect = tilemap.getFilledRectangle(i, j, i+_size-1, j+_size-1);
   for (std::list<Tile*>::iterator itTiles = rect.begin(); itTiles != rect.end(); ++itTiles)
   {
      Tile &tile = **itTiles;

      is_constructible &= tile.get_terrain().isConstructible();
   }

   return is_constructible;
}

void Construction::build(const int i, const int j)
{
   LandOverlay::build(i, j);
   computeAccessRoads();
}

const std::list<Tile*>& Construction::getAccessRoads() const
{
   return _accessRoads;
}

// here the problem lays: if we remove road, it is left in _accessRoads array
// also we need to recompute _accessRoads if we place new road tile
// on next to this road tile buildings

void Construction::computeAccessRoads()
{
	_accessRoads.clear();

	Tilemap& tilemap = Scenario::instance().getCity().getTilemap();
	int mi = _master_tile->getI();
	int mj = _master_tile->getJ();

	Uint8 maxDst2road = getMaxDistance2Road();
	std::list<Tile*> rect = tilemap.getRectangle( mi-maxDst2road, mj-maxDst2road, mi+_size+maxDst2road-1, mj+_size+maxDst2road-1, false);
	for (std::list<Tile*>::iterator itTiles = rect.begin(); itTiles != rect.end(); ++itTiles)
	{
		Tile* tile = *itTiles;

		if( tile->get_terrain().isRoad() )
		{
			_accessRoads.push_back( tile );
		}
	}
}

Uint8 Construction::getMaxDistance2Road() const
{
	return 1;
}

Aqueduct::Aqueduct()
{
  setType(B_AQUEDUCT);
  setPicture( PicLoader::instance().get_picture( rcAqueductGroup, 133) );
  _size = 1;
  // land2a 119 120         - aqueduct over road
  // land2a 121 122         - aqueduct over plain ground
  // land2a 123 124 125 126 - aqueduct corner
  // land2a 127 128         - aqueduct over dirty roads
  // land2a 129 130 131 132 - aqueduct T-shape crossing
  // land2a 133             - aqueduct crossing
  // land2a 134 - 148       - aqueduct without water
}

Aqueduct* Aqueduct::clone() const
{
  return new Aqueduct(*this);
}

void Aqueduct::build(const int i, const int j)
{
  Construction::build(i, j);  
}

void Aqueduct::setTerrain(TerrainTile &terrain)
{
  terrain.reset();
  terrain.setOverlay(this);
  terrain.setBuilding(true);
}

// I didn't decide what is the best approach: make Plaza as constructions or as upgrade to roads

Plaza::Plaza()
{
  setType(B_PLAZA);
  setPicture( PicLoader::instance().get_picture( rcEntertaimentGroup, 102) ); // 102 ~ 107
  _size = 1;
}

Plaza* Plaza::clone() const
{
  return new Plaza(*this);
}

void Plaza::setTerrain(TerrainTile &terrain)
{
  terrain.reset();
  terrain.setOverlay(this);
  terrain.setRoad(true);
}

// Plazas can be built ONLY on top of existing roads
// Also in original game there was a bug:
// gamer could place any number of plazas on one road tile (!!!)

bool Plaza::canBuild(const int i, const int j) const
{
   Tilemap& tilemap = Scenario::instance().getCity().getTilemap();

   bool is_constructible = true;

   std::list<Tile*> rect = tilemap.getFilledRectangle(i, j, i+_size-1, j+_size-1);
   for (std::list<Tile*>::iterator itTiles = rect.begin(); itTiles != rect.end(); ++itTiles)
   {
      Tile &tile = **itTiles;

      is_constructible &= tile.get_terrain().isRoad();
   }

   return is_constructible;
}

Garden::Garden()
{
  setType(B_GARDEN);
  setPicture( PicLoader::instance().get_picture( rcEntertaimentGroup, 110) ); // 110 111 112 113
  _size = 1;
}

Garden* Garden::clone() const
{
  return new Garden(*this);
}

void Garden::setTerrain(TerrainTile &terrain)
{
  terrain.reset();
  terrain.setOverlay(this);
  terrain.setBuilding(true);
  terrain.setGarden(true);
}

Reservoir::Reservoir()
{
  setType(B_RESERVOIR);
  setPicture( PicLoader::instance().get_picture( rcUtilityGroup, 34) );
  _size = 3;
  
  // utilitya 34      - emptry reservoir
  // utilitya 35 ~ 42 - full reservoir animation
 
  AnimLoader animLoader(PicLoader::instance());
  animLoader.fill_animation(_animation, rcUtilityGroup, 35, 8);
  animLoader.fill_animation_reverse(_animation, rcUtilityGroup, 42, 7);
  animLoader.change_offset(_animation, 47, 63);
  _fgPictures.resize(1);
  //_fgPictures[0]=;
}

Reservoir* Reservoir::clone() const
{
  return new Reservoir(*this);
}

void Reservoir::build(const int i, const int j)
{
  Construction::build(i, j);  
}

void Reservoir::setTerrain(TerrainTile &terrain)
{
  terrain.reset();
  terrain.setOverlay(this);
  terrain.setBuilding(true);
}

void Reservoir::timeStep(const unsigned long time)
{
  _animation.nextFrame();
  
  // takes current animation frame and put it into foreground
  _fgPictures.at(0) = _animation.get_current_picture(); 
}

Road::Road()
{
  setType(B_ROAD);
  setPicture(PicLoader::instance().get_picture( rcRoadGroup, 44));  // default picture for build tool
}

Road* Road::clone() const
{
  return new Road(*this);
}

void Road::build(const int i, const int j)
{
  Construction::build(i, j);
  setPicture(computePicture());

  // update adjacent roads
  for (std::list<Tile*>::iterator itTile = _accessRoads.begin(); itTile != _accessRoads.end(); ++itTile)
  {
    Tile &tile = **itTile;
    Road &road = (Road&) *tile.get_terrain().getOverlay();
    road.computeAccessRoads();
    road.setPicture(road.computePicture());
  }
  // NOTE: also we need to update accessRoads for adjacent building
  // how to detect them if MaxDistance2Road can be any
  // so let's recompute accessRoads for every _building_
  std::list<LandOverlay*> list = Scenario::instance().getCity().getOverlayList(); // it looks terrible!!!!
  for (std::list<LandOverlay*>::iterator itOverlay = list.begin(); itOverlay!=list.end(); ++itOverlay)
  {
    LandOverlay *overlay = *itOverlay;
    Building *construction = dynamic_cast<Building*>(overlay);
    if (construction != NULL) // if NULL then it ISN'T building
    {
      construction->computeAccessRoads();
    }    
  }
}

void Road::setTerrain(TerrainTile &terrain)
{
   terrain.reset();
   terrain.setOverlay(this);
   terrain.setRoad(true);
}

Picture& Road::computePicture()
{
   int i = getTile().getI();
   int j = getTile().getJ();

   std::list<Tile*> roads = getAccessRoads();
   int directionFlags = 0;  // bit field, N=1, E=2, S=4, W=8
   for (std::list<Tile*>::iterator itRoads = roads.begin(); itRoads!=roads.end(); ++itRoads)
   {
      Tile &tile = **itRoads;
      if (tile.getJ() > j)
      {
         // road to the north
         directionFlags += 1;
      }
      else if (tile.getJ() < j)
      {
         // road to the south
         directionFlags += 4;
      }
      else if (tile.getI() > i)
      {
         // road to the east
         directionFlags += 2;
      }
      else if (tile.getI() < i)
      {
         // road to the west
         directionFlags += 8;
      }
   }

   std::cout << "direction flags=" << directionFlags << std::endl;

   int index;
   switch (directionFlags)
   {
   case 0:  // no road!
      index = 101;
      break;
   case 1:  // North
      index = 101;
      break;
   case 2:  // East
      index = 102;
      break;
   case 4:  // South
      index = 103;
      break;
   case 8:  // West
      index = 104;
      break;
   case 3:  // North+East
      index = 97;
      break;
   case 5:  // North+South
      index = 93+2*(rand()%2);
      break;  // 93/95
   case 6:  // East+South
      index = 98;
      break;
   case 7:  // North+East+South
      index = 106;
      break;
   case 9:  // North+West
      index = 100;
      break;
   case 10:  // East+West
      index = 94+2*(rand()%2);
      break;  // 94/96
   case 11:  // North+East+West
      index = 109;
      break;
   case 12:  // South+West
      index = 99;
      break;
   case 13:  // North+South+West
      index = 108;
      break;
   case 14:  // East+South+West
      index = 107;
      break;
   case 15:  // North+East+South+West
      index = 110;
      break;
   }

   Picture *picture = &PicLoader::instance().get_picture( rcRoadGroup, index);
   return *picture;
}


Building::Building()
{
   _damageLevel = 0.0;
   _fireLevel = 0.0;
   _damageIncrement = 1;
   _fireIncrement = 1;
}

void Building::setTerrain(TerrainTile &terrain)
{
   terrain.reset();
   terrain.setOverlay(this);
   terrain.setBuilding(true);
}

void Building::timeStep(const unsigned long time)
{
   Construction::timeStep(time);

   if (time % 64 == 0)
   {
      _damageLevel += _damageIncrement;
      _fireLevel += _fireIncrement;
      if (_damageLevel >= 100)
      {
         std::cout << "Building destroyed!" << std::cout;
      }
      if (_fireLevel >= 100)
      {
         std::cout << "Building catch fire!" << std::cout;
      }
   }
}

float Building::getDamageLevel()
{
   return _damageLevel;
}

void Building::setDamageLevel(const float value)
{
   _damageLevel = value;
}

float Building::getFireLevel()
{
   return _fireLevel;
}

void Building::setFireLevel(const float value)
{
   _fireLevel = value;
}


void Building::storeGoods(GoodStock &stock, const int amount)
{
   THROW("This building should not store any goods");
}

float Building::evaluateService(ServiceWalker &walker)
{
   float res = 0.0;
   ServiceType service = walker.getService();
   if (_reservedServices.count(service) == 1)
   {
      // service is already reserved
      return 0.0;
   }

   if (service == S_ENGINEER)
   {
      res = _damageLevel;
   }
   else if (service == S_PREFECT)
   {
      res = _fireLevel;
   }
   return res;
}

void Building::reserveService(const ServiceType service)
{
   // std::cout << "reserved service" << std::endl;
   _reservedServices.insert(service);
}

void Building::cancelService(const ServiceType service)
{
   // std::cout << "cancel service" << std::endl;
   _reservedServices.erase(service);
}

void Building::applyService(ServiceWalker &walker)
{
   // std::cout << "apply service" << std::endl;
   // remove service reservation
   ServiceType service = walker.getService();
   _reservedServices.erase(service);

   if (service == S_ENGINEER)
   {
      _damageLevel = 0;
   }
   else if (service == S_PREFECT)
   {
      _fireLevel = 0;
   }
}

float Building::evaluateTrainee(const WalkerTraineeType traineeType)
{
   float res = 0.0;

   if (_reservedTrainees.count(traineeType) == 1)
   {
      // don't allow two reservations of the same type
      return 0.0;
   }

   if (_traineeMap.count(traineeType) == 1)
   {
      int currentLevel = _traineeMap[traineeType];
      res = (float)( 101 - currentLevel );
   }

   return res;
}

void Building::reserveTrainee(const WalkerTraineeType traineeType)
{
   _reservedTrainees.insert(traineeType);
}

void Building::cancelTrainee(const WalkerTraineeType traineeType)
{
   _reservedTrainees.erase(traineeType);
}

void Building::applyTrainee(const WalkerTraineeType traineeType)
{
   _reservedTrainees.erase(traineeType);
   _traineeMap[traineeType] += 100;
}

void Building::serialize(OutputSerialStream &stream)
{
   Construction::serialize(stream);
   stream.write_int((int) _damageLevel, 1, 0, 100);  // approximation...
   stream.write_int((int) _fireLevel, 1, 0, 100);  // approximation...

   stream.write_int(_traineeMap.size(), 1, 0, WTT_MAX);
   for (std::map<WalkerTraineeType, int>::iterator itLevel = _traineeMap.begin(); itLevel != _traineeMap.end(); ++itLevel)
   {
      WalkerTraineeType traineeType = itLevel->first;
      int traineeLevel = itLevel->second;
      stream.write_int((int)traineeType, 1, 0, WTT_MAX);
      stream.write_int(traineeLevel, 1, 0, 200);
   }

   stream.write_int(_reservedTrainees.size(), 1, 0, WTT_MAX);
   for (std::set<WalkerTraineeType>::iterator itReservation = _reservedTrainees.begin(); itReservation != _reservedTrainees.end(); ++itReservation)
   {
      WalkerTraineeType traineeType = *itReservation;
      stream.write_int((int)traineeType, 1, 0, WTT_MAX);
   }
}

void Building::unserialize(InputSerialStream &stream)
{
   Construction::unserialize(stream);
   _damageLevel = (float)stream.read_int(1, 0, 100);
   _fireLevel = (float)stream.read_int(1, 0, 100);

   int size = stream.read_int(1, 0, WTT_MAX);
   for (int i=0; i<size; ++i)
   {
      WalkerTraineeType traineeType = (WalkerTraineeType) stream.read_int(1, 0, WTT_MAX);
      int traineeLevel = stream.read_int(1, 0, 200);
      _traineeMap[traineeType] = traineeLevel;
   }

   size = stream.read_int(1, 0, WTT_MAX);
   for (int i=0; i<size; ++i)
   {
      WalkerTraineeType traineeType = (WalkerTraineeType) stream.read_int(1, 0, WTT_MAX);
      _reservedTrainees.insert(traineeType);
   }
}

WorkingBuilding::WorkingBuilding()
{
   _maxWorkers = 0;
   _currentWorkers = 0;
   _isActive = true;
}

void WorkingBuilding::setMaxWorkers(const int &maxWorkers)
{
   _maxWorkers = maxWorkers;
}

int WorkingBuilding::getMaxWorkers()
{
   return _maxWorkers;
}

void WorkingBuilding::setWorkers(const int &currentWorkers)
{
   _currentWorkers = currentWorkers;
}

int WorkingBuilding::getWorkers()
{
   return _currentWorkers;
}

void WorkingBuilding::setActive(const bool &value)
{
   _isActive = value;
}

bool WorkingBuilding::isActive()
{
   return _isActive;
}

void WorkingBuilding::serialize(OutputSerialStream &stream)
{
   Building::serialize(stream);
   stream.write_int((int) _currentWorkers, 1, 0, 100);
}

void WorkingBuilding::unserialize(InputSerialStream &stream)
{
   Building::unserialize(stream);
   _currentWorkers = stream.read_int(1, 0, 100);
}


Granary::Granary()
{
   setType(B_GRANARY);
   setMaxWorkers(5);
   setWorkers(0);

   _size = 3;
   setPicture(PicLoader::instance().get_picture( rcCommerceGroup, 140));
   _fgPictures.resize(6);  // 1 upper level + 4 windows + animation
   int maxQty = 2400;
   _goodStore.setMaxQty(maxQty);
   _goodStore.setMaxQty(G_WHEAT, maxQty);
   _goodStore.setMaxQty(G_MEAT, maxQty);
   _goodStore.setMaxQty(G_FISH, maxQty);
   _goodStore.setMaxQty(G_FRUIT, maxQty);
   _goodStore.setMaxQty(G_VEGETABLE, maxQty);

   _goodStore.setCurrentQty(G_WHEAT, 300);

   AnimLoader animLoader(PicLoader::instance());
   animLoader.fill_animation(_animation, rcCommerceGroup, 146, 7);
   // do the animation in reverse
   animLoader.fill_animation_reverse(_animation, rcCommerceGroup, 151, 6);
   PicLoader& ldr = PicLoader::instance();

   _fgPictures[0] = &ldr.get_picture( rcCommerceGroup, 141);
   _fgPictures[5] = _animation.get_current_picture();
   computePictures();
}

Granary* Granary::clone() const
{
   return new Granary(*this);
}


void Granary::timeStep(const unsigned long time)
{
   _animation.nextFrame();

   _fgPictures[5] = _animation.get_current_picture();
}

SimpleGoodStore& Granary::getGoodStore()
{
   return _goodStore;
}

void Granary::computePictures()
{
   int allQty = _goodStore.getCurrentQty();
   int maxQty = _goodStore.getMaxQty();

   for (int n = 0; n < 4; ++n)
   {
      // reset all window pictures
      _fgPictures[n+1] = NULL;
   }

   PicLoader& ldr = PicLoader::instance();
   if (allQty > 0)
   {
      _fgPictures[1] = &ldr.get_picture( rcCommerceGroup, 142);
   }
   if (allQty > maxQty * 0.25)
   {
      _fgPictures[2] = &ldr.get_picture( rcCommerceGroup, 143);
   }
   if (allQty > maxQty * 0.5)
   {
      _fgPictures[3] = &ldr.get_picture( rcCommerceGroup, 144);
   }
   if (allQty > maxQty * 0.9)
   {
      _fgPictures[4] = &ldr.get_picture( rcCommerceGroup, 145);
   }
}


GuiInfoBox* Granary::makeInfoBox()
{
   GuiInfoGranary* box = new GuiInfoGranary(*this);
   return box;
}

void Granary::serialize(OutputSerialStream &stream)
{
   WorkingBuilding::serialize(stream);
   _goodStore.serialize(stream);
}

void Granary::unserialize(InputSerialStream &stream)
{
   WorkingBuilding::unserialize(stream);
   _goodStore.unserialize(stream);
}

// housng1a 46 - governor's house    3 x 3
// housng1a 47 - governor's villa    4 x 4
// housng1a 48 - governor's palace   5 x 5

// govt     1  - small statue        1 x 1
// govt     2  - medium statue       2 x 2
// govt     3  - big statue          3 x 3

// land3a 43 44 - triumphal arch
// land3a 45 46 - triumphal arch

// transport 93 - missionaire post   2 x 2
// circus    1 ~ 18 hippodrome    5x(5 x 5)

MissionPost::MissionPost()
{
 setType(B_MISSION_POST);
  _size = 2;
  setMaxWorkers(20);
  setWorkers(0);  
  setPicture(PicLoader::instance().get_picture("transport", 93));
}


SmallStatue::SmallStatue()
{
 setType(B_STATUE1);
  _size = 1;
  setPicture(PicLoader::instance().get_picture(rcGovernmentGroup, 1));
}

MediumStatue::MediumStatue()
{
 setType(B_STATUE2);
  _size = 2;
  setPicture(PicLoader::instance().get_picture(rcGovernmentGroup, 2));
}

BigStatue::BigStatue()
{
 setType(B_STATUE3);
  _size = 3;
  setPicture(PicLoader::instance().get_picture(rcGovernmentGroup, 3));
}

MissionPost*  MissionPost::clone()  const { return new MissionPost(*this);  }
SmallStatue*  SmallStatue::clone()  const { return new SmallStatue(*this);  }
MediumStatue* MediumStatue::clone() const { return new MediumStatue(*this); }
BigStatue*    BigStatue::clone()    const { return new BigStatue(*this);    }

GovernorsHouse::GovernorsHouse()
{
  setType(B_GOVERNOR_HOUSE);
  _size = 3;
  setMaxWorkers(5);
  setWorkers(0);    
  setPicture(PicLoader::instance().get_picture(rcHousingGroup, 46));
}

GovernorsVilla::GovernorsVilla()
{
  setType(B_GOVERNOR_VILLA);
  _size = 4;
  setMaxWorkers(10);
  setWorkers(0);    
  setPicture(PicLoader::instance().get_picture(rcHousingGroup, 47));
}

GovernorsPalace::GovernorsPalace()
{
  setType(B_GOVERNOR_PALACE);
  _size = 5;
  setMaxWorkers(15);
  setWorkers(0);  
  setPicture(PicLoader::instance().get_picture(rcHousingGroup, 48));
}

GovernorsHouse*  GovernorsHouse::clone()  const { return new GovernorsHouse(*this);  }
GovernorsVilla*  GovernorsVilla::clone()  const { return new GovernorsVilla(*this);  }
GovernorsPalace* GovernorsPalace::clone() const { return new GovernorsPalace(*this); }

Academy::Academy()
{
  setType(B_MILITARY_ACADEMY);
  _size = 3;
  setMaxWorkers(20);
  setWorkers(0);
  setPicture(PicLoader::instance().get_picture(rcSecurityGroup, 18));
}

Barracks::Barracks()
{
  setType(B_BARRACKS);
  _size = 3;
  setMaxWorkers(5);
  setWorkers(0);  
  setPicture(PicLoader::instance().get_picture(rcSecurityGroup, 17));
}

Academy*  Academy::clone()  const { return new Academy(*this);  }
Barracks* Barracks::clone() const { return new Barracks(*this); }

NativeBuilding::NativeBuilding() {}

void NativeBuilding::serialize(OutputSerialStream &stream) {Building::serialize(stream);}

void NativeBuilding::unserialize(InputSerialStream &stream) {Building::unserialize(stream);}

GuiInfoBox* NativeBuilding::makeInfoBox()
{
  return new GuiBuilding(*this);
}

NativeHut* NativeHut::clone() const
{
  return new NativeHut(*this);
}

NativeHut::NativeHut()
{
  setType(B_NATIVE_HUT);
  _size = 1;
  setPicture(PicLoader::instance().get_picture(rcHousingGroup, 49));
  //setPicture(PicLoader::instance().get_picture("housng1a", 50));
}

void NativeHut::serialize(OutputSerialStream &stream)  {Building::serialize(stream);}

void NativeHut::unserialize(InputSerialStream &stream) {Building::unserialize(stream);}



NativeCenter::NativeCenter()
{
  setType(B_NATIVE_CENTER);
  _size = 2;
  setPicture(PicLoader::instance().get_picture(rcHousingGroup, 51));
}

void NativeCenter::serialize(OutputSerialStream &stream)  {Building::serialize(stream);}

void NativeCenter::unserialize(InputSerialStream &stream) {Building::unserialize(stream);}

NativeCenter* NativeCenter::clone() const
{
  return new NativeCenter(*this);
}

NativeField::NativeField()
{
  setType(B_NATIVE_FIELD);
  _size = 1;
  setPicture(PicLoader::instance().get_picture(rcCommerceGroup, 13));  
}

void NativeField::serialize(OutputSerialStream &stream) {Building::serialize(stream);}

void NativeField::unserialize(InputSerialStream &stream) {Building::unserialize(stream);}

NativeField* NativeField::clone() const
{
  return new NativeField(*this);
}

Shipyard::Shipyard()
{
  setType(B_SHIPYARD);
  _size = 2;
  setPicture(PicLoader::instance().get_picture("transport", 1));
  // also transport 2 3 4 check position of river on map
}

Shipyard* Shipyard::clone() const
{
   return new Shipyard(*this);
}

// dock pictures
// transport 5        animation = 6~16
// transport 17       animation = 18~28
// transport 29       animation = 30~40
// transport 41       animation = 42~51

Dock::Dock()
{
  setType(B_DOCK);
  _size = 2;
  setPicture(PicLoader::instance().get_picture("transport", 5));  

  AnimLoader animLoader(PicLoader::instance());
  animLoader.fill_animation(_animation, "transport", 6, 11);
  // now fill in reverse order
  animLoader.fill_animation_reverse(_animation, "transport", 15, 10);
  
  animLoader.change_offset(_animation, 107, 61);
  _fgPictures.resize(1);  
}

void Dock::timeStep(const unsigned long time)
{
  _animation.nextFrame();
  
  // takes current animation frame and put it into foreground
  _fgPictures.at(0) = _animation.get_current_picture(); 
}


Dock* Dock::clone() const
{
   return new Dock(*this);
}

// second arch pictures is land3a 45 + 46	

TriumphalArch::TriumphalArch()
{
  setType(B_TRIUMPHAL_ARCH);
  _size = 3;
  setPicture(PicLoader::instance().get_picture("land3a", 43));
  getPicture().set_offset(0,116);
  AnimLoader animLoader(PicLoader::instance());
  animLoader.fill_animation(_animation, "land3a", 44, 1);
  animLoader.change_offset(_animation, 63, 97);
  _fgPictures.resize(1);
  _fgPictures.at(0) = _animation.get_current_picture(); 
}

TriumphalArch* TriumphalArch::clone() const
{
   return new TriumphalArch(*this);
}



FortLegionnaire::FortLegionnaire()
{
  setType(B_FORT_LEGIONNAIRE);
  _size = 3;
  setPicture(PicLoader::instance().get_picture(rcSecurityGroup, 12));

  Picture* logo = &PicLoader::instance().get_picture(rcSecurityGroup, 16);
  logo -> set_offset(80,10);
  _fgPictures.resize(1);
  _fgPictures.at(0) = logo;  
}

FortLegionnaire* FortLegionnaire::clone() const
{
   return new FortLegionnaire(*this);
}

FortMounted::FortMounted()
{
  setType(B_FORT_MOUNTED);
  _size = 3;
  setPicture(PicLoader::instance().get_picture(rcSecurityGroup, 12));

  Picture* logo = &PicLoader::instance().get_picture(rcSecurityGroup, 15);
  logo -> set_offset(80,10);
  _fgPictures.resize(1);
  _fgPictures.at(0) = logo;
}

FortMounted* FortMounted::clone() const
{
   return new FortMounted(*this);
}

FortJaveline::FortJaveline()
{
  setType(B_FORT_JAVELIN);
  _size = 3;
  setPicture(PicLoader::instance().get_picture(rcSecurityGroup, 12));

  Picture* logo = &PicLoader::instance().get_picture(rcSecurityGroup, 14);
  //std::cout << logo->get_xoffset() << " " << logo->get_yoffset() << " " << logo->get_width() << " " << logo->get_height() << std::endl;
  logo -> set_offset(80,10);
  _fgPictures.resize(1);
  _fgPictures.at(0) = logo;  
}

FortJaveline* FortJaveline::clone() const
{
   return new FortJaveline(*this);
}
