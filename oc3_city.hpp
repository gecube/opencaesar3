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


#ifndef CITY_HPP
#define CITY_HPP

#include "oc3_tilemap.hpp"
#include "oc3_walker.hpp"
#include "oc3_enums.hpp"
#include "oc3_serializer.hpp"
#include "oc3_signals.hpp"
#include "oc3_predefinitions.hpp"
#include "oc3_referencecounted.hpp"
#include "oc3_cityservice.hpp"

class TilePos;
class DateTime;
class CityBuildOptions;

class City : public Serializable, public ReferenceCounted
{
public:
  static CityPtr create();
  ~City();

  void timeStep();  // performs one simulation step
  void monthStep();

  Walkers getWalkerList( const WalkerType type );
  void addWalker( WalkerPtr walker );
  void removeWalker( WalkerPtr walker );

  void addService( CityServicePtr service );
  CityServicePtr findService( const std::string& name ) const;

  LandOverlays& getOverlayList();
  LandOverlays getBuildingList( const BuildingType buildingType );

  void setRoadExit( const TilePos& pos );
  void setBoatEntry( const TilePos& pos );
  void setRoadEntry( const TilePos& pos );
  void setBoatExit( const TilePos& pos );
  
  TilePos getRoadExit() const;
  TilePos getBoatEntry() const;
  TilePos getBoatExit() const;

  TilePos getRoadEntry() const;
  
  void setCameraPos(const TilePos pos);
  TilePos getCameraPos() const;
     
  ClimateType getClimate() const;
  void setClimate(const ClimateType);

  int getTaxRate() const;
  void setTaxRate(const int taxRate);
  long getFunds() const;
  void setFunds(const long funds);

  int getPopulation() const;
  int getProsperity() const;

  Tilemap& getTilemap();

  void save( VariantMap& stream) const;
  void load( const VariantMap& stream);

  // add construction
  void build( const BuildingType type, const TilePos& pos );
  void build( ConstructionPtr building, const TilePos& pos );

  CityBuildOptions& getBuildOptions();

  void disaster( const TilePos& pos, DisasterType type );
  // remove construction
  void clearLand( const TilePos& pos );
  // collect taxes from all houses
  void collectTaxes();

  const DateTime& getDate() const;
  void setDate( const DateTime& time );
   
oc3_signals public:
  Signal1<int>& onPopulationChanged();
  Signal1<int>& onFundsChanged();
  Signal1<const DateTime&>& onMonthChanged();
  Signal1<std::string>& onWarningMessage();
  Signal2<const TilePos&, const std::string& >& onDisasterEvent();

private:
  City();
  void _calculatePopulation();

  class Impl;
  ScopedPtr< Impl > _d;
};

class CityHelper
{
public:
  CityHelper( CityPtr city ) : _city( city ) {}

  template< class T >
  std::list< SmartPtr< T > > getBuildings( const BuildingType type )
  {
    std::list< SmartPtr< T > > ret;
    LandOverlays buildings = _city->getBuildingList( type );
    for( LandOverlays::iterator it = buildings.begin(); it != buildings.end(); it++  )
    {
      SmartPtr< T > b = (*it).as<T>();
      if( b.isValid() )
      {
        ret.push_back( b );
      }
    }

    return ret;
  }

  template< class T >
  SmartPtr< T > getBuilding( const TilePos& pos )
  {
    LandOverlayPtr overlay = _city->getTilemap().at( pos ).getTerrain().getOverlay();
    return overlay.as< T >();
  }

protected:
  CityPtr _city;
};

#endif
