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

#include "oc3_cityservice_roads.hpp"
#include "oc3_city.hpp"
#include "oc3_gamedate.hpp"
#include "oc3_path_finding.hpp"
#include "oc3_tilemap.hpp"
#include "oc3_road.hpp"

class CityServiceRoads::Impl
{
public:
  CityPtr city;
  int maxDistance;
  int defaultIncreasePaved;
  int defaultDecreasePaved;

  DateTime lastTimeUpdate;
  ScopedPtr< Propagator > propagator;

  void updateRoadsAround( BuildingPtr building );
};

CityServicePtr CityServiceRoads::create( CityPtr city )
{
  CityServiceRoads* ret = new CityServiceRoads( city );

  return CityServicePtr( ret );
}

CityServiceRoads::CityServiceRoads( CityPtr city )
: CityService( "roads" ), _d( new Impl )
{
  _d->city = city;
  _d->maxDistance = 10;
  _d->defaultIncreasePaved = 4;
  _d->defaultDecreasePaved = -1;
  _d->lastTimeUpdate = GameDate::current();
  _d->propagator.reset( new Propagator( city ) );
}

void CityServiceRoads::update( const unsigned int time )
{
  if( _d->lastTimeUpdate.getMonth() == GameDate::current().getMonth() )
    return;

  _d->lastTimeUpdate = GameDate::current();

  std::vector< BuildingType > btypes;
  btypes.push_back( B_SENATE );

  CityHelper helper( _d->city );

  foreach( BuildingType type, btypes )
  {
    BuildingList buildings = helper.getBuildings<Building>( type );

    foreach( BuildingPtr building, buildings )
    {
      _d->updateRoadsAround( building );
    }
  }

  if( _d->lastTimeUpdate.getMonth() % 3 == 1 )
  {
    RoadList roads = helper.getBuildings<Road>( B_ROAD );
    foreach( RoadPtr road, roads )
    {
      road->appendPaved( _d->defaultDecreasePaved );
    }
  }
}

CityServiceRoads::~CityServiceRoads()
{

}

void CityServiceRoads::Impl::updateRoadsAround(BuildingPtr building)
{
  std::list<PathWay> pathWayList;

  propagator->init( building.as<Construction>() );
  propagator->getWays( maxDistance, pathWayList );

  Tilemap& tmap = city->getTilemap();

  foreach( PathWay& current, pathWayList )
  {
    ConstTilemapTiles tiles = current.getAllTiles();
    foreach( const Tile* tile, tiles )
    {
      Tile& currentTile = tmap.at( tile->getIJ() );
      RoadPtr road = currentTile.getOverlay().as<Road>();
      if( road.isValid() )
      {
        road->appendPaved( defaultIncreasePaved );
      }
    }
  }
}
