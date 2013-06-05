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

#include "oc3_factoryclay.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_scenario.hpp"
#include "oc3_tile.hpp"

FactoryClay::FactoryClay() : Factory(G_NONE, G_CLAY, B_CLAY_PIT, Size(2) )
{
  _setProductRate( 9.6f );
  _picture = &Picture::load( ResourceGroup::commerce, 61 );

  _animation.load( ResourceGroup::commerce, 62, 10);
  _animation.setFrameDelay( 3 );
  _fgPictures.resize(2);

  setMaxWorkers( 10 );
  setWorkers( 0 );
}

void FactoryClay::timeStep( const unsigned long time )
{
  bool mayAnimate = getWorkers() > 0;

  if( mayAnimate && _animation.isStopped() )
  {
    _animation.start();
  }

  if( !mayAnimate && _animation.isRunning() )
  {
    _animation.stop();
  }

  Factory::timeStep( time );
}

bool FactoryClay::canBuild(const TilePos& pos ) const
{
  bool is_constructible = Construction::canBuild( pos );
  bool near_water = false;

  Tilemap& tilemap = Scenario::instance().getCity().getTilemap();
  PtrTilesList rect = tilemap.getRectangle( pos + TilePos( -1, -1), Size( _size + 2 ), Tilemap::checkCorners );
  for( PtrTilesList::iterator itTiles = rect.begin(); itTiles != rect.end(); ++itTiles )
  {
    near_water |= (*itTiles)->getTerrain().isWater();
  }

  return (is_constructible && near_water);
} 