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

#include "oc3_tile.hpp"
#include "oc3_exception.hpp"
#include "oc3_building.hpp"
#include "oc3_landoverlay.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_stringhelper.hpp"

void Tile::Terrain::reset()
{
  clearFlags();
  desirability = 0;
  watersrvc = 0;
}

void Tile::Terrain::clearFlags()
{
  water      = false;
  rock       = false;
  tree       = false;
  building   = false;
  road       = false;
  aqueduct   = false;
  garden     = false;
  meadow     = false;
  wall       = false;
  gatehouse  = false;
}

Tile::Tile( const TilePos& pos) //: _terrain( 0, 0, 0, 0, 0, 0 )
{
  _pos = pos;
  _picture = NULL;
  _wasDrawn = false;
  _master = NULL;
  _overlay = NULL;
  _terrain.reset();
  _terrain.imgid = 0;
}

int Tile::getI() const    {   return _pos.getI();   }

int Tile::getJ() const    {   return _pos.getJ();   }


void Tile::setPicture(const Picture *picture)
{
  _picture = picture;
}

void Tile::setPicture(const char* rc, const int index)
{
  setPicture( &Picture::load( rc, index ) );
}

void Tile::setPicture(const std::string& name)
{
  setPicture( &Picture::load( name ) );
}

const Picture& Tile::getPicture() const
{
  _OC3_DEBUG_BREAK_IF( !_picture && "error: picture is null");

  return *_picture;
}

Tile* Tile::getMasterTile() const
{
  return _master;
}

void Tile::setMasterTile(Tile* master)
{
  _master = master;
}

bool Tile::isFlat() const
{
  return !(_terrain.rock || _terrain.tree || _terrain.building || _terrain.aqueduct);
}

TilePos Tile::getIJ() const
{
  return _pos;
}

bool Tile::isMasterTile() const
{
  return (_master == this);
}

Point Tile::getXY() const
{
  return Point( 30 * ( getI() + getJ()), 15 * (getI() - getJ()) );
}

void Tile::animate(unsigned int time)
{
  if( _overlay.isNull() && _animation.isValid() )
  {
    _animation.update( time );
  }
}

const Animation&Tile::getAnimation() const
{
  return _animation;
}

void Tile::setAnimation(const Animation& animation)
{
  _animation = animation;
}

bool Tile::isWalkable(bool alllands ) const
{
  // TODO: test building to allow garden, gatehouse, granary, ...
  bool walkable = (_terrain.road || (alllands && !_terrain.water && !_terrain.tree && !_terrain.rock));
  if( _overlay.isValid() )
  {
    walkable &= _overlay->isWalkable();
  }

  return walkable;
}

bool Tile::getFlag(Tile::Type type) const
{
  switch( type )
  {
  case tlRoad: return _terrain.road;
  case tlWater: return _terrain.water;
  case tlTree: return _terrain.tree;
  case isConstructible:
  {
    return !(_terrain.water || _terrain.rock || _terrain.tree || _terrain.building || _terrain.road);
  }
  case tlMeadow: return _terrain.meadow;
  case tlRock: return _terrain.rock;
  case tlBuilding: return _terrain.building;
  case tlAqueduct: return _terrain.aqueduct;
  case isDestructible:
  {
    return (_terrain.tree || _terrain.building || _terrain.road);
  }
  case tlGarden: return _terrain.garden;
  case tlElevation: return _terrain.elevation;
  case tlWall: return _terrain.wall;
  case tlGateHouse: return _terrain.gatehouse;
  case wasDrawn: return _wasDrawn;
  default: break;
  }

  return false;
}

void Tile::setFlag(Tile::Type type, bool value)
{
  switch( type )
  {
  case tlRoad: _terrain.road = value; break;
  case tlWater: _terrain.water = value; break;
  case tlTree: _terrain.tree = value; break;
  case tlMeadow: _terrain.meadow = value; break;
  case tlRock: _terrain.rock = value; break;
  case tlBuilding: _terrain.building = value; break;
  case tlAqueduct: _terrain.aqueduct = value; break;
  case tlGarden: _terrain.garden = value; break;
  case tlElevation: _terrain.elevation = value; break;
  case clearAll: _terrain.clearFlags(); break;
  case tlWall: _terrain.wall = value; break;
  case tlGateHouse: _terrain.gatehouse = value; break;
  case wasDrawn: _wasDrawn = value; break;
  default: break;
  }
}

void Tile::appendDesirability(int value)
{
   _terrain.desirability = math::clamp( _terrain.desirability += value, -0xff, 0xff );
}

int Tile::getDesirability() const
{
  return _terrain.desirability;
}

LandOverlayPtr Tile::getOverlay() const
{
  return _overlay;
}

void Tile::setOverlay(LandOverlayPtr overlay)
{
  _overlay = overlay;
}

unsigned int Tile::getOriginalImgId() const
{
  return _terrain.imgid;
}

void Tile::setOriginalImgId(unsigned short id)
{
  _terrain.imgid = id;
}

void Tile::fillWaterService(const WaterService type)
{
  _terrain.watersrvc |= (0xf << (type*4));
}

void Tile::decreaseWaterService(const WaterService type)
{
  int tmpSrvValue = (_terrain.watersrvc >> (type*4)) & 0xf;
  //tmpSrvValue = math::clamp( tmpSrvValue-1, 0, 0xf );
  tmpSrvValue = 0;

  _terrain.watersrvc &= ~(0xf<<(type*4));
  _terrain.watersrvc |= tmpSrvValue << (type*4);
}

int Tile::getWaterService(const WaterService type) const
{
  return (_terrain.watersrvc >> (type*4)) & 0xf;
}

std::string TileHelper::convId2PicName( const unsigned int imgId )
{
  // example: for land1a_00004.png, pfx=land1a and id=4
  std::string res_pfx;  // resource name prefix
  int res_id = imgId;   // id of resource

  if( imgId < 245 )
  {
    res_pfx = "plateau";
    res_id = imgId - 200;
  }
  else if( imgId < 548 )
  {
    res_pfx = "land1a";
    res_id = imgId - 244;
  }
  else if( imgId < 779 )
  {
    res_pfx = ResourceGroup::land2a;
    res_id = imgId - 547;
  }
  else if( imgId < 871)
  {
    res_pfx = ResourceGroup::land3a;
    res_id = imgId - 778;
  }
  else
  {
    res_pfx = "land1a";
    res_id = 1;

    // std::cout.setf(std::ios::hex, std::ios::basefield);
    // std::cout << "Unknown image Id " << imgId << std::endl;
    // std::cout.unsetf(std::ios::hex);

    if (imgId == 0xb10 || imgId == 0xb0d)
    {
      res_pfx = ResourceGroup::housing;
      res_id = 51;
    } // TERRIBLE HACK!

    // THROW("Unknown image Id " << imgId);
  }

  std::string ret_str = StringHelper::format( 0xff, "%s_%05d.png", res_pfx.c_str(), res_id );
  return ret_str;
}

int TileHelper::convPicName2Id( const std::string &pic_name )
{
  // example: for land1a_00004.png, return 244+4=248
  std::string res_pfx;  // resource name prefix = land1a
  int res_id;   // idx of resource = 4

  // extract the name and idx from name (ex: [land1a, 4])
  int pos = pic_name.find("_");
  res_pfx = pic_name.substr(0, pos);
  std::stringstream ss(pic_name.substr(pos+1));
  ss >> res_id;

  if (res_pfx == "plateau")
  {
    res_id += 200;
  }
  else if (res_pfx == "land1a")
  {
    res_id += 244;
  }
  else if (res_pfx == ResourceGroup::land2a)
  {
    res_id += 547;
  }
  else if (res_pfx == ResourceGroup::land3a)
  {
    res_id += 778;
  }
  else
  {
    THROW("Unknown image " << pic_name);
  }

  return res_id;
}

int TileHelper::encode(const Tile& tt)
{
  int res = tt.getFlag( Tile::tlTree ) ? 0x11 : 0;
  res += tt.getFlag( Tile::tlRock ) ? 0x2 : 0;
  res += tt.getFlag( Tile::tlWater ) ? 0x4 : 0;
  res += tt.getFlag( Tile::tlBuilding ) ? 0x8 : 0;
  res += tt.getFlag( Tile::tlRoad ) ? 0x40 : 0;
  res += tt.getFlag( Tile::tlMeadow ) ? 0x800 : 0;
  res += tt.getFlag( Tile::tlWall ) ? 0x4000 : 0;
  res += tt.getFlag( Tile::tlElevation ) ? 0x200 : 0;
  res += tt.getFlag( Tile::tlGateHouse ) ? 0x8000 : 0;
  return res;
}

void TileHelper::decode(Tile& tile, const int bitset)
{
  tile.setFlag( Tile::clearAll, true );

  if(bitset & 0x1)    { tile.setFlag( Tile::tlTree, true);      }
  if(bitset & 0x2)    { tile.setFlag( Tile::tlRock, true);      }
  if(bitset & 0x4)    { tile.setFlag( Tile::tlWater, true);     }
  if(bitset & 0x8)    { tile.setFlag( Tile::tlBuilding, true);  }
  if(bitset & 0x10)   { tile.setFlag( Tile::tlTree, true);      }
  if(bitset & 0x20)   { tile.setFlag( Tile::tlGarden, true);    }
  if(bitset & 0x40)   { tile.setFlag( Tile::tlRoad, true);      }
  if(bitset & 0x100)  { tile.setFlag( Tile::tlAqueduct, true);  }
  if(bitset & 0x200)  { tile.setFlag( Tile::tlElevation, true); }
  if(bitset & 0x400)  { tile.setFlag( Tile::tlRock, true );     }
  if(bitset & 0x800)  { tile.setFlag( Tile::tlMeadow, true);    }
  if(bitset & 0x4000) { tile.setFlag( Tile::tlWall, true);      }
  if(bitset & 0x8000) { tile.setFlag( Tile::tlGateHouse, true); }
}

