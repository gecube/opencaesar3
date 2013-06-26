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

#include "oc3_burningruins.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_positioni.hpp"
#include "oc3_servicewalker.hpp"
#include "oc3_tile.hpp"
#include "oc3_scenario.hpp"

BurningRuins::BurningRuins() : ServiceBuilding(S_BURNING_RUINS, B_BURNING_RUINS, Size(1) )
{
    _fireLevel = 99;

    setPicture( Picture::load( ResourceGroup::land2a, 187) );
    _animation.load( ResourceGroup::land2a, 188, 8 );
    _animation.setOffset( Point( 14, 26 ) );
    _fgPictures.resize(1);           
}

void BurningRuins::timeStep(const unsigned long time)
{
    ServiceBuilding::timeStep(time);

    if (time % 16 == 0 )
    {
      if( _fireLevel > 0 )
      {
        _fireLevel -= 1;
        if( _fireLevel == 50 )
        {
          setPicture(Picture::load( ResourceGroup::land2a, 214));
          _animation.clear();
          _animation.load( ResourceGroup::land2a, 215, 8);
          _animation.setOffset( Point( 14, 26 ) );
        }
        else if( _fireLevel == 25 )
        {
          setPicture( Picture::load( ResourceGroup::land2a, 223));
          _animation.clear();
          _animation.load(ResourceGroup::land2a, 224, 8);
          _animation.setOffset( Point( 14, 18 ) );
        }
      }
      else
      {
        deleteLater();
        _animation.clear();
        _fgPictures.clear();
      }           
    }
}

void BurningRuins::destroy()
{
  ServiceBuilding::destroy();

  Scenario::instance().getCity().build( B_BURNED_RUINS, getTilePos());
}

void BurningRuins::deliverService()
{
  /*ServiceWalker walker(getService());
  walker.setServiceBuilding(*this);
  std::set<Building*> reachedBuildings = walker.getReachedBuildings(getTile().getI(), getTile().getJ());
  for (std::set<Building*>::iterator itBuilding = reachedBuildings.begin(); itBuilding != reachedBuildings.end(); ++itBuilding)
  {
    Building &building = **itBuilding;
    building.applyService(walker);
  }*/
}

void BurningRuins::burn()
{

}

void BurningRuins::build( const TilePos& pos )
{
  ServiceBuilding::build( pos );
  //while burning can't remove it
  getTile().getTerrain().setTree( false );
  getTile().getTerrain().setBuilding( false );
  getTile().getTerrain().setRoad( false );
  getTile().getTerrain().setRock( true );
}   

bool BurningRuins::isWalkable() const
{
  return (_fireLevel == 0);
}

float BurningRuins::evaluateService( ServiceWalkerPtr walker )
{
  return _fireLevel;
}

void BurningRuins::applyService(ServiceWalkerPtr walker)
{
  if ( S_PREFECT == walker->getService() )
  {
    _fireLevel = math::clamp<float>( _fireLevel - walker->getServiceValue(), 0.f, 100.f );
  }
}

bool BurningRuins::isNeedRoadAccess() const
{
  return false;
}

void BurnedRuins::timeStep( const unsigned long time )
{

}

BurnedRuins::BurnedRuins() : Building( B_BURNED_RUINS, Size(1) )
{
  setPicture( Picture::load( ResourceGroup::land2a, 111 + rand() % 8 ));
}

void BurnedRuins::build( const TilePos& pos )
{
  Building::build(pos);

  getTile().getTerrain().setBuilding( true );
  getTile().getTerrain().setRock( false );
}

bool BurnedRuins::isWalkable() const
{
  return true;
}

bool BurnedRuins::isNeedRoadAccess() const
{
  return false;
}

void BurnedRuins::destroy()
{
  Building::destroy();
}