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

#ifndef __OPENCAESAR3_WATER_BUILDGINDS_INCLUDED__
#define __OPENCAESAR3_WATER_BUILDGINDS_INCLUDED__

#include "oc3_service_building.hpp"

class WaterSource;
class Aqueduct;
class Reservoir;

class WaterSource : public Construction
{
public:
  WaterSource( const BuildingType type, const Size& size );
  
  virtual void addWater( const WaterSource& source );
  virtual bool haveWater() const;
  virtual void timeStep(const unsigned long time);
  int getId() const;

protected:
  virtual void _waterStateChanged() {};
  virtual void _produceWater( const TilePos* points, const int size );
  

  typedef std::map< int, int > WaterSourceMap;
  WaterSourceMap _sourceMap;
  int _water;
  bool _lastWaterState;
};

class Aqueduct : public WaterSource
{
public:
  Aqueduct();

  virtual void build(const TilePos& pos );
  Picture& computePicture();
  virtual void setTerrain(TerrainTile &terrain);
  virtual bool canBuild(const TilePos& pos ) const;
  virtual bool isNeedRoadAccess() const;
  void updatePicture();
  void addWater( const WaterSource& source );
  virtual void destroy();

protected:
  virtual void _waterStateChanged();
};

typedef SmartPtr< Aqueduct > AqueductPtr;

class Reservoir : public WaterSource
{
public:
  Reservoir();
  ~Reservoir();

  virtual void build(const TilePos& pos );
  virtual bool canBuild(const TilePos& pos ) const;
  virtual bool isNeedRoadAccess() const;
  virtual void setTerrain(TerrainTile &terrain);
  virtual void timeStep(const unsigned long time);
  virtual void destroy();

private:
  bool _isWaterSource;
  bool _isNearWater( const TilePos& pos ) const;
};

class BuildingFountain : public ServiceBuilding
{
public:
  BuildingFountain();

  virtual void build( const TilePos& pos );
  virtual bool canBuild( const TilePos& pos ) const;
  virtual void deliverService();
  virtual void timeStep(const unsigned long time);
  virtual bool isNeedRoadAccess() const;
private:
  bool _haveReservoirWater;
};

#endif // __OPENCAESAR3_WATER_BUILDGINDS_INCLUDED__
