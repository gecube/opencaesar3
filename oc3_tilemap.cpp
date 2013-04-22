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



#include "oc3_tilemap.hpp"

#include <iostream>

#include "oc3_pic_loader.hpp"
#include "oc3_building.hpp"
#include "oc3_exception.hpp"
#include "oc3_positioni.hpp"

TerrainTile::TerrainTile()
{
   reset();
}

void TerrainTile::reset()
{
  _isWater    = false;
  _isRock     = false;
  _isTree     = false;
  _isBuilding = false;
  _isRoad     = false;
  _isAqueduct = false;
  _isGarden   = false;
  _isMeadow   = false;
  _overlay    = NULL; // BUG? What will be with old overlay?
}

bool TerrainTile::isConstructible() const
{
  return !(_isWater || _isRock || _isTree || _isBuilding || _isRoad);
}

bool TerrainTile::isDestructible() const
{
  return (_isTree || _isBuilding || _isRoad);
}

bool TerrainTile::isWalkable(const bool allLand) const
{
   // TODO: test building to allow garden, gatehouse, granary, ...
   bool extendedRoad = false;

   return (_isRoad || extendedRoad || (allLand && !_isWater && !_isTree && !_isRock && !_isBuilding && !_isAqueduct));
}

void TerrainTile::setOverlay(LandOverlay *overlay)
{
  _overlay = overlay;
}

LandOverlay *TerrainTile::getOverlay() const
{
  return _overlay;
}

int TerrainTile::encode() const
{
  int res = 0;
  if (isTree())
  {
    res += 0x1;
    res += 0x10;
  }
  if (isRock())     { res +=   0x2; }
  if (isWater())    { res +=   0x4; }
  if (isBuilding()) { res +=   0x8; }
  if (isRoad())     { res +=  0x40; }
  if (isMeadow())   { res += 0x800; }
  
  return res;
}

void TerrainTile::decode(const int bitset)
{
   reset();

   if (bitset & 0x1)    {  setTree(true);     }
   if (bitset & 0x2)    {  setRock(true);     }
   if (bitset & 0x4)    {  setWater(true);    }
   if (bitset & 0x8)    {  setBuilding(true); }
   if (bitset & 0x10)   {  setTree(true);     }
   if (bitset & 0x20)   {  setGarden(true);   }
   if (bitset & 0x40)   {  setRoad(true);     }
   if (bitset & 0x100)  {  setAqueduct(true); }
//   if (bitset & 0x200)  {  setElevation(true);}
   if (bitset & 0x400)  {
     int i=0;
     setRock( true );
     //setAccessRamp(true);
   }
   if (bitset & 0x800)  {  setMeadow(true);   }
//   if (bitset & 0x4000) {  setWall(true);   }
}

void TerrainTile::serialize(OutputSerialStream &stream)
{
   int terrainBitset = encode();
   stream.write_int(terrainBitset, 2, 0, 65535);
   stream.write_objectID(_overlay);
}

void TerrainTile::unserialize(InputSerialStream &stream)
{
   int terrainBitset = stream.read_int(2, 0, 65535);
   decode(terrainBitset);
   stream.read_objectID((void**)&_overlay);
}

void TerrainTile::setOriginalImgId( unsigned int id )
{
    _imgId = id;
}

unsigned int TerrainTile::getOriginalImgId() const
{
  return _imgId;
}

Tile::Tile(const int i, const int j)
{
   _i = i;
   _j = j;
   _picture = NULL;
   _master_tile = NULL;
   _terrain.reset();
}


Tile::Tile(const Tile& clone)
{
   _i = clone._i;
   _j = clone._j;
   _picture = clone._picture;
   _master_tile = clone._master_tile;
   _terrain = clone._terrain;
}

int Tile::getI() const    {   return _i;   }

int Tile::getJ() const    {   return _j;   }


void Tile::set_picture(Picture *picture)
{
   _picture = picture;
}

Picture& Tile::get_picture() const
{
   if (_picture==NULL) THROW("error: picture is null");
   return *_picture;
}

Tile* Tile::get_master_tile() const
{
   return _master_tile;
}

void Tile::set_master_tile(Tile* master)
{
   _master_tile = master;
}

const TerrainTile& Tile::get_terrain() const
{
   return _terrain;
}

TerrainTile& Tile::get_terrain()
{
   return _terrain;
}

bool Tile::is_flat() const
{
   return !(_terrain.isRock() || _terrain.isTree() || _terrain.isBuilding() || _terrain.isAqueduct());
}

TilePos Tile::getIJ() const
{
    return TilePos( _i, _j );
}

Tilemap::Tilemap()
{
   _size = 0;
}

void Tilemap::init(const int size)
{
   _size = size;

   // resize the tile array
   _tile_array.resize(_size);
   for (int i = 0; i < _size; ++i)
   {
      _tile_array[i].reserve(_size);

      for (int j = 0; j < _size; ++j)
      {
         _tile_array[i].push_back(Tile(i, j));
      }
   }
}

bool Tilemap::is_inside(const TilePos& pos ) const
{
   return ( pos.getI() >= 0 && pos.getJ()>=0 && pos.getI()<_size && pos.getJ()<_size);
}

Tile& Tilemap::at(const int i, const int j)
{
   return _tile_array.at(i).at(j);
}

const Tile& Tilemap::at(const int i, const int j) const
{
   return _tile_array.at(i).at(j);
}

Tile& Tilemap::at( const TilePos& ij )
{
    return _tile_array.at(ij.getI()).at(ij.getJ());
    // bad! No check of arrays borders!!!
}

const Tile& Tilemap::at( const TilePos& ij ) const
{
    return _tile_array.at(ij.getI()).at(ij.getJ());
}

int Tilemap::getSize() const
{
   return _size;
}

std::list<Tile*> Tilemap::getRectangle( const TilePos& start, const TilePos& stop, const bool corners /*= true*/ )
{
    std::list<Tile*> res;

    int delta_corners = 0;
    if (! corners)
    {
        delta_corners = 1;
    }

    /*Rect maxRect( 0, 0, _size, _size );
    Rect rect( start.getI()+delta_corners, start.getJ()+delta_corners, 
               stop.getI()-delta_corners, stop.getJ()-delta_corners );

    rect.constrainTo( maxRect );
    for( int i=rect.getLeft(); i < rect.getRight(); i++ )
        for( int j=rect.getTop(); j < rect.getBottom(); j++ )
            ret.push_back( &at( TilePos( i, j ) ) );
    */
    for(int i = start.getI()+delta_corners; i <= stop.getI()-delta_corners; ++i)
    {
        if (is_inside( TilePos( i, start.getJ() ) ))
        {
            res.push_back( &at(i, start.getJ() ));
        }

        if (is_inside( TilePos( i, stop.getJ() ) ))
        {
            res.push_back( &at( i, stop.getJ() ));
        }
    }

    for (int j = start.getJ()+1; j <= stop.getJ()-1; ++j)  // corners have been handled already
    {
        if (is_inside( TilePos( start.getI(), j ) ))
        {
            res.push_back(&at(start.getI(), j));
        }

        if (is_inside( TilePos( stop.getI(), j ) ))
        {
            res.push_back(&at(stop.getI(), j));
        }
    }

    return res;
}

std::list<Tile*> Tilemap::getRectangle( const TilePos& pos, const Size& size, const bool corners /*= true */ )
{
    return getRectangle( pos, pos + TilePos( size.getWidth(), size.getHeight()), corners );
}
// Get tiles inside of rectangle

std::list<Tile*> Tilemap::getFilledRectangle(const TilePos& start, const TilePos& stop )
{
   std::list<Tile*> res;

   for (int i = start.getI(); i <= stop.getI(); ++i)
   {
      for (int j = start.getJ(); j <= stop.getJ(); ++j)
      {
         if( is_inside( TilePos( i, j ) ))
         {
            res.push_back(&at( TilePos( i, j ) ) );
         }
      }
   }

   return res;
}

std::list<Tile*> Tilemap::getFilledRectangle( const TilePos& start, const Size& size )
{
    return getFilledRectangle( start, start + TilePos( size.getWidth(), size.getHeight() ) );
}

void Tilemap::serialize(OutputSerialStream &stream)
{
   stream.write_int(_size, 2, 0, 1000);

   // saves the graphics map
   PicLoader &picLoader = PicLoader::instance();
   std::string pic_name;
   short int imgId;   // 16bits
   for (int i=0; i<_size; ++i)
   {
      for (int j=0; j<_size; ++j)
      {
         Tile &tile = at(i, j);
         TerrainTile &terrain = tile.get_terrain();

         imgId = 0;

         if (&tile == tile.get_master_tile() || tile.get_master_tile() == NULL)
         {
            // this is a master tile
            if ( !terrain.isBuilding() )
            {
               // this is not a building
               pic_name = tile.get_picture().get_name();
               imgId = picLoader.get_pic_id_by_name(pic_name);
            }
         }

         stream.write_int(imgId, 2, 0, 65535);
         terrain.serialize(stream);
      }
   }
}

void Tilemap::unserialize(InputSerialStream &stream)
{
   _size = stream.read_int(2, 0, 1000);
   init(_size);

   // loads the graphics map
   PicLoader &picLoader = PicLoader::instance();
   short int imgId;   // 16bits
   for (int i=0; i<_size; ++i)
   {
      for (int j=0; j<_size; ++j)
      {
         Tile &tile = at(i, j);
         TerrainTile &terrain = tile.get_terrain();

         imgId = (short)stream.read_int(2, 0, 65535);

         // terrain
         terrain.unserialize(stream);
         // cannot access the overlay at this stage (it has not be unserialized yet)

         if (imgId != 0 && !terrain.isBuilding())
         {
            // master landscape tile!
            Picture& pic = picLoader.get_pic_by_id(imgId);

            int tile_size = (pic.get_width()+2)/60;  // size of the multi-tile. the multi-tile is a square.

            // master is the left-most subtile
            Tile* master;
            if (tile_size == 1)
            {
               master = NULL;
            }
            else
            {
               master = &at(i, j);
            }

            for (int di=0; di<tile_size; ++di)
            {
               // for each subrow of the multi-tile
               for (int dj=0; dj<tile_size; ++dj)
               {
                  // for each subcol of the multi-tile
                  Tile &sub_tile = at(i+di, j+dj);
                  sub_tile.set_master_tile(master);
                  sub_tile.set_picture(&pic);
               }
            }
         }

      }
   }
}

std::string TerrainTileHelper::convId2PicName( const unsigned int imgId )
{
  // example: for land1a_00004.png, pfx=land1a and id=4
  std::string res_pfx;  // resource name prefix
  int res_id = imgId;   // id of resource

  if (201<=imgId && imgId < 245)
  {
    res_pfx = "plateau";
    res_id = imgId - 200;
  }
  else if (245<=imgId && imgId < 548)
  {
    res_pfx = "land1a";
    res_id = imgId - 244;
  }
  else if (548<=imgId && imgId < 779)
  {
    res_pfx = "land2a";
    res_id = imgId - 547;
  }
  else if (779<=imgId && imgId < 871)
  {
    res_pfx = "land3a";
    res_id = imgId - 778;      
  }
  else
  {
    res_pfx = "land1a";
    res_id = 1;

    // std::cout.setf(std::ios::hex, std::ios::basefield);
    // std::cout << "Unknown image Id " << imgId << std::endl;
    // std::cout.unsetf(std::ios::hex);

    if (imgId == 0xb10 || imgId == 0xb0d) {res_pfx = "housng1a", res_id = 51;} // TERRIBLE HACK!

    // THROW("Unknown image Id " << imgId);
  }

  char ret_str[128];
  snprintf( ret_str, 127, "%s_%05d.png", res_pfx.c_str(), res_id );
  return std::string( ret_str );
}
