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

#include "oc3_building.hpp"

#include "oc3_tile.hpp"
#include "oc3_scenario.hpp"
#include "oc3_walker_service.hpp"
#include "oc3_exception.hpp"
#include "oc3_building_data.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_variant.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_city.hpp"
#include "oc3_foreach.hpp"
#include "oc3_scenario_event.hpp"
#include "oc3_tilemap.hpp"

Construction::Construction( const BuildingType type, const Size& size)
: LandOverlay( type, size )
{
}

bool Construction::canBuild( const TilePos& pos ) const
{
  Tilemap& tilemap = Scenario::instance().getCity()->getTilemap();

  bool is_constructible = true;

  //return area for available tiles
  PtrTilesArea area = tilemap.getFilledRectangle( pos, getSize() );

  //on over map size
  if( (int)area.size() != getSize().getArea() )
    return false;

  foreach( Tile* tile, area )
  {
     is_constructible &= tile->getTerrain().isConstructible();
  }

  return is_constructible;
}

void Construction::build( const TilePos& pos )
{
  LandOverlay::build( pos );
  computeAccessRoads();
  _updateDesirabilityInfluence( duPositive );
}

void Construction::_updateDesirabilityInfluence( const DsbrlUpdate type )
{
  CityPtr city = Scenario::instance().getCity();
  Tilemap& tilemap = city->getTilemap();

  int dsrblRange = getDesirabilityRange();
  int step = getDesirabilityStep();
  int desInfluence = getDesirabilityInfluence();
  int mul = ( type == duPositive ? 1 : -1);

  //change desirability in selfarea
  PtrTilesArea area = tilemap.getFilledRectangle( getTilePos(), getSize() );
  foreach( Tile* tile, area )
  {
    tile->getTerrain().appendDesirability( mul * desInfluence );
  }

  //change deisirability around
  for( int curRange=1; curRange <= dsrblRange; curRange++ )
  {
    PtrTilesArea perimetr = tilemap.getRectangle( getTilePos() - TilePos( curRange, curRange ), 
                                                  getSize() + Size( 2 * curRange - 1 ) );
    foreach( Tile* tile, perimetr )
    {
      tile->getTerrain().appendDesirability( mul * desInfluence );
    }

    desInfluence += step;
  }
}


const PtrTilesList& Construction::getAccessRoads() const
{
   return _accessRoads;
}

// here the problem lays: if we remove road, it is left in _accessRoads array
// also we need to recompute _accessRoads if we place new road tile
// on next to this road tile buildings
void Construction::computeAccessRoads()
{
  _accessRoads.clear();
  if( !_getMasterTile() )
      return;

  Tilemap& tilemap = Scenario::instance().getCity()->getTilemap();

  int maxDst2road = getMaxDistance2Road();
  PtrTilesList rect = tilemap.getRectangle( _getMasterTile()->getIJ() + TilePos( -maxDst2road, -maxDst2road ),
                                            getSize() + Size( 2 * maxDst2road ), !Tilemap::checkCorners );
  foreach( Tile* tile, rect )
  {
    if( tile->getTerrain().isRoad() )
    {
      _accessRoads.push_back( tile );
    }
  }
}

int Construction::getMaxDistance2Road() const
{
  return 1;
  // it is default value
  // for houses - 2
}

void Construction::burn()
{
  deleteLater();
  DisasterEvent::create( getTile().getIJ(), DisasterEvent::fire );
}

void Construction::collapse()
{
  deleteLater();
  DisasterEvent::create( getTile().getIJ(), DisasterEvent::collapse );
}

char Construction::getDesirabilityInfluence() const
{
  return BuildingDataHolder::instance().getData( getType() ).getDesirbilityInfluence();
}

unsigned char Construction::getDesirabilityRange() const
{
  return BuildingDataHolder::instance().getData( getType() ).getDesirbilityRange();
}

void Construction::destroy()
{
  LandOverlay::destroy();
  _updateDesirabilityInfluence( duNegative );
}

bool Construction::isNeedRoadAccess() const
{
  return true;
}

char Construction::getDesirabilityStep() const
{
  return BuildingDataHolder::instance().getData( getType() ).getDesirabilityStep();
}

Building::Building(const BuildingType type, const Size& size )
: Construction( type, size )
{
   _damageLevel = 0.0;
   _fireLevel = 0.0;
   _damageIncrement = 1;
   _fireIncrement = 1;
}

void Building::setTerrain(TerrainTile &terrain)
{
  // here goes the problem
  // when we reset tile, we delete information
  // about it's original information
  // try to fix
  bool saveMeadow = terrain.isMeadow();
  terrain.clearFlags();
  terrain.setOverlay(this);
  terrain.setBuilding(true);
  terrain.setMeadow(saveMeadow);
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
        StringHelper::debug( 0xff, "Building destroyed!" );
        collapse();
      }
      if (_fireLevel >= 100)
      {
        StringHelper::debug( 0xff, "Building catch fire!" );
        burn();
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
   _OC3_DEBUG_BREAK_IF("This building should not store any goods");
}

float Building::evaluateService(ServiceWalkerPtr walker)
{
   float res = 0.0;
   Service::Type service = walker->getService();
   if (_reservedServices.count(service) == 1)
   {
      // service is already reserved
      return 0.0;
   }

   switch(service)
   {
   case Service::S_ENGINEER:
      res = _damageLevel;
   break;

   case Service::S_PREFECT:
      res = _fireLevel;
   break;

   default: break;
   }
   return res;
}

void Building::reserveService(const Service::Type service)
{
   // std::cout << "reserved service" << std::endl;
   _reservedServices.insert(service);
}

void Building::cancelService(const Service::Type service)
{
   // std::cout << "cancel service" << std::endl;
   _reservedServices.erase(service);
}

void Building::applyService( ServiceWalkerPtr walker)
{
   // std::cout << "apply service" << std::endl;
   // remove service reservation
   Service::Type service = walker->getService();
   _reservedServices.erase(service);

   switch( service )
   {
   case Service::S_ENGINEER:
     {
       _damageLevel = 0;
     }
   break;
   case Service::S_PREFECT:
    {
      _fireLevel = 0;
    }
   break;

   default: break;
   }
}

float Building::evaluateTrainee(const WalkerType traineeType)
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

void Building::reserveTrainee(const WalkerType traineeType)
{
   _reservedTrainees.insert(traineeType);
}

void Building::cancelTrainee(const WalkerType traineeType)
{
   _reservedTrainees.erase(traineeType);
}

void Building::applyTrainee(const WalkerType traineeType)
{
   _reservedTrainees.erase(traineeType);
   _traineeMap[traineeType] += 100;
}

void Building::save( VariantMap& stream) const
{
    Construction::save( stream );
    stream[ Serializable::damageLevel ] = _damageLevel;  
    stream[ Serializable::fireLevel ] = _fireLevel;  

//    stream.write_int(_traineeMap.size(), 1, 0, WTT_MAX);
//    for (std::map<WalkerTraineeType, int>::iterator itLevel = _traineeMap.begin(); itLevel != _traineeMap.end(); ++itLevel)
//    {
//       WalkerTraineeType traineeType = itLevel->first;
//       int traineeLevel = itLevel->second;
//       stream.write_int((int)traineeType, 1, 0, WTT_MAX);
//       stream.write_int(traineeLevel, 1, 0, 200);
//    }
// 
//    stream.write_int(_reservedTrainees.size(), 1, 0, WTT_MAX);
//    for (std::set<WalkerTraineeType>::iterator itReservation = _reservedTrainees.begin(); itReservation != _reservedTrainees.end(); ++itReservation)
//    {
//       WalkerTraineeType traineeType = *itReservation;
//       stream.write_int((int)traineeType, 1, 0, WTT_MAX);
//    }
}

void Building::load( const VariantMap& stream )
{
  Construction::load( stream );
  _damageLevel = stream.get( Serializable::damageLevel ).toFloat();
  _fireLevel = stream.get( Serializable::fireLevel ).toFloat();
//    Construction::unserialize(stream);
//    _damageLevel = (float)stream.read_int(1, 0, 100);
//    _fireLevel = (float)stream.read_int(1, 0, 100);
// 
//    int size = stream.read_int(1, 0, WTT_MAX);
//    for (int i=0; i<size; ++i)
//    {
//       WalkerTraineeType traineeType = (WalkerTraineeType) stream.read_int(1, 0, WTT_MAX);
//       int traineeLevel = stream.read_int(1, 0, 200);
//       _traineeMap[traineeType] = traineeLevel;
//    }
// 
//    size = stream.read_int(1, 0, WTT_MAX);
//    for (int i=0; i<size; ++i)
//    {
//       WalkerTraineeType traineeType = (WalkerTraineeType) stream.read_int(1, 0, WTT_MAX);
//       _reservedTrainees.insert(traineeType);
//    }
}

// govt     1  - small statue        1 x 1
// govt     2  - medium statue       2 x 2
// govt     3  - big statue          3 x 3

// land3a 43 44 - triumphal arch
// land3a 45 46 - triumphal arch

// transport 93 - missionaire post   2 x 2
// circus    1 ~ 18 hippodrome    5x(5 x 5)

SmallStatue::SmallStatue() : Building( B_STATUE1, Size(1) )
{
  _fireIncrement = 0;
  _damageIncrement = 0;
  setPicture( Picture::load( ResourceGroup::govt, 1));
}

bool SmallStatue::isNeedRoadAccess() const
{
  return false;
}

MediumStatue::MediumStatue() : Building( B_STATUE2, Size(2) )
{
  _fireIncrement = 0;
  _damageIncrement = 0;
  setPicture( Picture::load( ResourceGroup::govt, 2));
}

bool MediumStatue::isNeedRoadAccess() const
{
  return false;
}

BigStatue::BigStatue() : Building( B_STATUE3, Size(3))
{
  _fireIncrement = 0;
  _damageIncrement = 0;
  setPicture( Picture::load( ResourceGroup::govt, 3));
}

bool BigStatue::isNeedRoadAccess() const
{
  return false;
}

Shipyard::Shipyard() : Building( B_SHIPYARD, Size(2) )
{
  setPicture( Picture::load( ResourceGroup::transport, 1));
  // also transport 2 3 4 check position of river on map
}

// dock pictures
// transport 5        animation = 6~16
// transport 17       animation = 18~28
// transport 29       animation = 30~40
// transport 41       animation = 42~51

Dock::Dock() : Building( B_DOCK, Size(2) )
{
  setPicture( Picture::load( ResourceGroup::transport, 5));  

  _getAnimation().load( ResourceGroup::transport, 6, 11);
  // now fill in reverse order
  _getAnimation().load( ResourceGroup::transport, 15, 10, Animation::reverse );
  
  _getAnimation().setOffset( Point( 107, 61 ) );
  _fgPictures.resize(1);  
}

void Dock::timeStep(const unsigned long time)
{
  _getAnimation().update( time );
  
  // takes current animation frame and put it into foreground
  _fgPictures[ 0 ] = _getAnimation().getCurrentPicture();
}

// second arch pictures is land3a 45 + 46	

TriumphalArch::TriumphalArch() : Building( B_TRIUMPHAL_ARCH, Size(3) )
{
  setPicture( Picture::load( "land3a", 43 ) );
  _getAnimation().load("land3a", 44, 1);
  _getAnimation().setOffset( Point( 63, 97 ) );
  _fgPictures.resize(1);
  _fgPictures.at(0) = _getAnimation().getCurrentPicture();
}
