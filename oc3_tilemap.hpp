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


#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <list>

#include "oc3_serializer.hpp"
#include "oc3_picture.hpp"

class LandOverlay;
class TilePos;

class TerrainTile : public Serializable
{
public:
   TerrainTile();

   // reset all fields
   void reset();

   bool isConstructible() const;
   bool isDestructible() const;
   bool isWalkable(const bool allLands) const;
   // isBurnable

   bool isWater() const                    { return _isWater;    }
   bool isRock() const                     { return _isRock;     }
   bool isTree() const                     { return _isTree;     }
   bool isBuilding() const                 { return _isBuilding; }
   bool isRoad() const                     { return _isRoad;     }
   bool isGarden() const                   { return _isGarden;   }
   bool isAqueduct() const                 { return _isAqueduct; }
   bool isMeadow() const                   { return _isMeadow;   }

   void setTree(const bool isTree)         { _isTree     = isTree;     }
   void setRock(const bool isRock)         { _isRock     = isRock;     }
   void setWater(const bool isWater)       { _isWater    = isWater;    }
   void setBuilding(const bool isBuilding) { _isBuilding = isBuilding; }
   void setGarden(const bool isGarden)     { _isGarden   = isGarden;   }
   void setRoad(const bool isRoad)         { _isRoad     = isRoad;     }
   void setAqueduct(const bool isAqueduct) { _isAqueduct = isAqueduct; }
   void setMeadow(const bool isMeadow)     { _isMeadow   = isMeadow;   }
   
   void setOverlay(LandOverlay *overlay);
   LandOverlay *getOverlay() const;

   // encode/decode to bitset
   int encode() const;
   void decode(const int bitset);

   void setOriginalImgId( unsigned int id );
   unsigned int getOriginalImgId() const;

   void serialize(OutputSerialStream &stream);
   void unserialize(InputSerialStream &stream);
private:
   bool _isWater;
   bool _isRock;
   bool _isTree;
   bool _isBuilding;
   bool _isRoad;
   bool _isGarden;
   bool _isAqueduct;
   bool _isMeadow;
   unsigned int _imgId;
   LandOverlay *_overlay;
};

class TerrainTileHelper
{
public:
  static std::string convId2PicName( const unsigned int imgId );
};


// a Tile in the Tilemap
class Tile
{
public:
   Tile(const int i, const int j);
   Tile(const Tile& clone);

   // tile coordinates
   int getI() const;
   int getJ() const;
   TilePos getIJ() const;

   // displayed picture
   void set_picture(Picture *picture);
   Picture& get_picture() const;

   // used for multi-tile graphics: current displayed picture
   // background of constructible tiles is 1x1 => master used for foreground
   // non-constructible tiles have no foreground => master used for background
   Tile* get_master_tile() const;
   void set_master_tile(Tile* master);
   bool is_master_tile() const {return (_master_tile == this);};

   const TerrainTile& get_terrain() const;
   TerrainTile& get_terrain();
   bool is_flat() const;  // returns true if the tile is walkable/boatable (for display purpose)

private:
   int _i; // coordinates of the tile
   int _j; // coordinates of the tile
   Tile* _master_tile;  // left-most tile if multi-tile, or "this" if single-tile
   TerrainTile _terrain;    // infos about the tile (building, tree, road, water, rock...)

   Picture *_picture; // displayed picture   
};

typedef std::list< const Tile* > ConstWayOnTiles;
typedef std::list< Tile* > WayOnTiles;
typedef std::list< Tile* > PtrTilesArea;
typedef std::list< Tile* > PtrTilesList;

typedef std::vector< Tile* > PtrTilesVector;
typedef std::vector< Tile  > TilesVector;
typedef std::vector< TilesVector > TileGrid;

// Square Map of the Tiles.
class Tilemap : public Serializable
{
public:
   static const bool checkCorners = true;
   
   Tilemap();
   void init(const int size);

   bool is_inside( const TilePos& pos ) const;

   Tile& at( const int i, const int j );
   Tile& at( const TilePos& ij );
   
   const Tile& at( const int i, const int j ) const;
   const Tile& at( const TilePos& ij ) const;

   // returns all tiles on a rectangular perimeter
   // (i1, j1) : left corner of the rectangle (minI, minJ)
   // (i2, j2) : right corner of the rectangle (maxI, maxJ)
   // corners  : if false, don't return corner tiles
   PtrTilesArea getRectangle(const TilePos& start, const TilePos& stope, const bool corners = true );
   PtrTilesArea getRectangle(const TilePos& pos, const Size& size, const bool corners = true );

   // returns all tiles in a rectangular area
   // (i1, j1) : left corner of the rectangle (minI, minJ)
   // (i2, j2) : right corner of the rectangle (maxI, maxJ)
   PtrTilesArea getFilledRectangle( const TilePos& start, const TilePos& stop );
   PtrTilesArea getFilledRectangle( const TilePos& start, const Size& size );
   int getSize() const;

   void serialize(OutputSerialStream &stream);
   void unserialize(InputSerialStream &stream);

private:
   TileGrid _tile_array;
   int _size;
};


#endif
