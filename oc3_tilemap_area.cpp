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



#include "oc3_tilemap_area.hpp"

#include "oc3_positioni.hpp"
#include "oc3_size.hpp"
#include "oc3_exception.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_tile.hpp"
#include "oc3_foreach.hpp"

class TilemapArea::Impl
{
public:
  Point centerMapXZ;      //center of the view (in tiles)
  Size viewSize;    // width of the view (in tiles)  nb_tilesX = 1+2*_view_width
                    // height of the view (in tiles)  nb_tilesY = 1+2*_view_height

  PtrTilesArea tiles;  // cached list of visible tiles
};

TilemapArea::TilemapArea() : _d( new Impl )
{
  _tilemap = NULL;
  _map_size = 0;
  _d->viewSize = Size( 0 );
  _center_i = 0;
  _center_j = 0;
  _d->centerMapXZ = Point( 0, 0 );
}

TilemapArea::~TilemapArea()
{
}

void TilemapArea::init(Tilemap &tilemap)
{
  _tilemap = &tilemap;
  _map_size = tilemap.getSize();
}

void TilemapArea::setViewport(const Size& newSize )
{
  if( _d->viewSize != newSize )
  {
    _d->tiles.clear();
  }

  _d->viewSize = Size( (newSize.getWidth() + 59) / 60, ( newSize.getHeight() + 29) / 30 );
  
  StringHelper::debug( 0xff, "TilemapArea::setViewport w=%d h=%d", _d->viewSize.getWidth(), _d->viewSize.getHeight() );
}

void TilemapArea::setCenter(const TilePos& pos )
{
  _center_i = pos.getI();
  _center_j = pos.getJ();

  setCenter( Point( _center_i + _center_j, _map_size - 1 + _center_j - _center_i ) );
}

void TilemapArea::setCenter( const Point& pos )
{
  if( _d->centerMapXZ != pos  )
  {
    _d->tiles.clear();
  }
  
  _d->centerMapXZ = pos;
}

int TilemapArea::getCenterX() const  {   return _d->centerMapXZ.getX();   }
int TilemapArea::getCenterZ() const  {   return _d->centerMapXZ.getY();   }
int TilemapArea::getCenterI() const  {   return _center_i;   }
int TilemapArea::getCenterJ() const  {   return _center_j;   }


void TilemapArea::moveRight(const int amount)
{
  setCenter( Point( getCenterX() + amount, getCenterZ() ) );
}

void TilemapArea::moveLeft(const int amount)
{
  setCenter( Point( getCenterX() - amount, getCenterZ() ) );
}

void TilemapArea::moveUp(const int amount)
{
  setCenter( Point( getCenterX(), getCenterZ() + amount ) );
}

void TilemapArea::moveDown(const int amount)
{
  setCenter( Point( getCenterX(), getCenterZ() - amount ) );
}

const PtrTilesArea& TilemapArea::getTiles() const
{
  if( _d->tiles.empty() )
  {
    int zm = _map_size + 1;
    int cx = _d->centerMapXZ.getX();
    int cz = _d->centerMapXZ.getY();

    Size sizeT = _d->viewSize;  // size x

    for (int z = cz + sizeT.getHeight(); z>=cz - sizeT.getHeight(); --z)
    {
      // depth axis. from far to near.

      int xstart = cx - sizeT.getWidth();
      if ((xstart + z) % 2 == 0)
      {
	      ++xstart;
      }

      for (int x = xstart; x<=cx + sizeT.getWidth(); x+=2)
      {
	// left-right axis
        int j = (x + z - zm)/2;
        int i = x - j;

        if (i >= 0 && j >= 0 && i < _map_size && j < _map_size)
        {
          _d->tiles.push_back( &_tilemap->at( i, j ));
        }
      }
    }
  }

  return _d->tiles;
}

void TilemapArea::resetWasDrawn()
{
  foreach( Tile* tile, _d->tiles )
  {
    tile->resetWasDrawn();
  }

}
