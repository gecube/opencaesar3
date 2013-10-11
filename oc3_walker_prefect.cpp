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

#include "oc3_walker_prefect.hpp"
#include "oc3_positioni.hpp"
#include "oc3_building_prefecture.hpp"
#include "oc3_building_house.hpp"
#include "oc3_astarpathfinding.hpp"
#include "oc3_path_finding.hpp"
#include "oc3_tile.hpp"
#include "oc3_city.hpp"
#include "oc3_variant.hpp"
#include "oc3_name_generator.hpp"
#include "oc3_game_event_mgr.hpp"
#include "oc3_stringhelper.hpp"

class WalkerPrefect::Impl
{
public:
  typedef enum { patrol=0, back2Prefecture, gotoFire, fightFire, doNothing } PrefectAction; 
    
  int water;
  PrefectAction action;
};

WalkerPrefect::WalkerPrefect( CityPtr city )
: ServiceWalker( city, Service::prefect ), _d( new Impl )
{
  _setType( WT_PREFECT );
  _d->water = 0;
  _d->action = Impl::patrol;
  _setGraphic( WG_PREFECT );

  setName( NameGenerator::rand( NameGenerator::male ) );
}

void WalkerPrefect::onNewTile()
{
  Walker::onNewTile();
}

bool WalkerPrefect::_looks4Fire( ServiceWalker::ReachedBuildings& buildings, TilePos& pos )
{
  buildings = getReachedBuildings( getIJ() );

  foreach( BuildingPtr building, buildings )
  {
    if( building->getType() == B_BURNING_RUINS )
    {
      pos = building->getTilePos();
      return true;
    }
  }

  return false;
}

void WalkerPrefect::_checkPath2NearestFire( const ReachedBuildings& buildings )
{
  PathWay bestPath;
  int minLength = 9999;
  foreach( BuildingPtr building, buildings )
  {
    if( building->getType() != B_BURNING_RUINS )
      continue;

    PathWay tmp;
    bool foundPath = Pathfinder::getInstance().getPath( getIJ(), building->getTile().getIJ(), tmp,
                                                        false, Size( 0 ) ); 
    if( foundPath && tmp.getLength() < minLength )
    {
      bestPath = tmp;
      minLength = tmp.getLength();
      
      if( tmp.getLength() == 1 )
        break;
    }
  }

  if( bestPath.getLength() > 0 )
  {
    setPathWay( bestPath );
    //_pathWay.begin();
  }
}

void WalkerPrefect::onDestination()
{ 
}

void WalkerPrefect::_back2Prefecture()
{
  bool pathFound = Pathfinder::getInstance().getPath( getIJ(), getBase()->getTile().getIJ(),
                                                      _getPathway(), false, Size( 0 ) );

  if( !pathFound )
  {
    deleteLater();
    _d->action = Impl::doNothing;
  }
  else
  {
    _getPathway().begin();
  }

  _setGraphic( WG_PREFECT );
  _d->action = Impl::back2Prefecture;
}

void WalkerPrefect::onMidTile()
{
  ReachedBuildings reachedBuildings;
  TilePos firePos;
  bool haveBurningRuinsNear = _looks4Fire( reachedBuildings, firePos );  
  bool isDestination = _getPathway().isDestination();

  switch( _d->action )
  {
  case Impl::doNothing:
  break;

  case Impl::patrol:
  {
    if( haveBurningRuinsNear )
    {
      //tell our prefecture that need send prefect with water to fight with fire
      //on next deliverService

      //found fire, no water, go prefecture
      getBase().as<Prefecture>()->fireDetect( firePos );
      _back2Prefecture();

      Walker::onNewDirection();
    }
    else
    {
      foreach( BuildingPtr building, reachedBuildings )
      {
        building->applyService( ServiceWalkerPtr( this ) );

        HousePtr house = building.as<House>();
        if( house.isValid() && house->getHealthLevel() < 1 )
        {
          house->deleteLater();

          GameEventMgr::append( DisasterEvent::create( house->getTilePos(), DisasterEvent::plague ) );
        }
      }
    }

    if( isDestination )
    {
      _back2Prefecture();
    }

    Walker::onMidTile();
  }
  break;

  case Impl::back2Prefecture:
  {
    if( haveBurningRuinsNear )
    {
      //tell our prefecture that need send prefect with water to fight with fire
      //on next deliverService
      getBase().as<Prefecture>()->fireDetect( firePos );
    }

    if( isDestination )
    {
      deleteLater();
      _d->action = Impl::doNothing;
    }

    Walker::onMidTile();
  }
  break;

  case Impl::gotoFire:
  {
    if( _getPathway().getDestination().getIJ().distanceFrom( getIJ() ) < 1.5f )
    {
      LandOverlayPtr overlay = _getPathway().getDestination().getOverlay();
      if( overlay.isValid() && overlay->getType() == B_BURNING_RUINS )
      {
        _d->action = Impl::fightFire;
        _setGraphic( WG_PREFECT_FIGHTS_FIRE );
        Walker::onNewDirection();
        isDestination = false;
      }
    }

    if( isDestination )
    {
      if( !haveBurningRuinsNear || _d->water == 0 )
      {
        _back2Prefecture();
      }
      else
      {
        _setGraphic( WG_PREFECT_DRAG_WATER );
        _d->action = Impl::gotoFire;

        _checkPath2NearestFire( reachedBuildings );
        Walker::onNewDirection();
      }
    }

    Walker::onMidTile();
  }
  break;

  case Impl::fightFire:
  break;
  }
}

void WalkerPrefect::timeStep(const unsigned long time)
{
  ServiceWalker::timeStep( time );

  if( _d->action == Impl::fightFire )
  {    
    setSpeed( 0 );
    BuildingPtr building = _getPathway().getDestination().getOverlay().as<Building>();
    bool inFire = (building.isValid() && building->getType() == B_BURNING_RUINS);

    if( inFire )
    {
      ServiceWalkerPtr ptr( this );
      const float beforeFight = building->evaluateService( ptr );
      building->applyService( ptr );
      const float afterFight = building->evaluateService( ptr );
      _d->water -= math::clamp( (int)(beforeFight - afterFight), 0, 100 );

      if( afterFight == 0)
      {
        inFire = false;
      }
    }

    if( !inFire || 0 == _d->water )
    {
      _setGraphic( WG_PREFECT_DRAG_WATER );
      _d->action = Impl::gotoFire;  
      setSpeed( 1 );
    }      

  }
}

WalkerPrefect::~WalkerPrefect()
{
}

float WalkerPrefect::getServiceValue() const
{
  return 5;
}

WalkerPrefectPtr WalkerPrefect::create( CityPtr city )
{
  WalkerPrefectPtr ret( new WalkerPrefect( city ) );
  ret->drop();

  return ret;
}

void WalkerPrefect::send2City(PrefecturePtr prefecture, int water/*=0 */ )
{
  _d->action = water > 0 ? Impl::gotoFire : Impl::patrol;
  _d->water = water;
  _setGraphic( water > 0 ? WG_PREFECT_DRAG_WATER : WG_PREFECT );

  if( water > 0 )
  {
    setBase( prefecture.as<Building>() );

    _getCity()->addWalker( WalkerPtr( this ));
  }
  else
  {
    ServiceWalker::send2City( prefecture.as<Building>() );
  }
}

void WalkerPrefect::load( const VariantMap& stream )
{
  ServiceWalker::load( stream );
 
  _d->action = (Impl::PrefectAction)stream.get( "prefectAction" ).toInt();
  _d->water = stream.get( "water" ).toInt();
  _setGraphic( _d->water > 0 ? WG_PREFECT_DRAG_WATER : WG_PREFECT );

  PrefecturePtr prefecture = getBase().as<Prefecture>();
  if( prefecture.isValid() )
  {
    prefecture->addWalker( WalkerPtr( this ) );
    _getCity()->addWalker( WalkerPtr( this ) );
  }
  
  if( prefecture.isNull() )
  {
    StringHelper::debug( 0xff, "Not found prefecture on loading" );
    deleteLater();
  }
}

void WalkerPrefect::save( VariantMap& stream ) const
{
  ServiceWalker::save( stream );

  stream[ "type" ] = (int)WT_PREFECT;
  stream[ "water" ] = _d->water;
  stream[ "prefectAction" ] = (int)_d->action;
  stream[ "__debug_typeName" ] = Variant( std::string( OC3_STR_EXT(WT_PREFECT) ) );
}
