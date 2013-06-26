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

#include "oc3_cityservice_info.hpp"
#include "oc3_city.hpp"
#include "oc3_safetycast.hpp"
#include "oc3_positioni.hpp"
#include "oc3_house.hpp"
#include "oc3_house_level.hpp"
#include "oc3_tile.hpp"
#include "oc3_time.hpp"

class CityParameters
{
public:
  int population;
  int funds;

  CityParameters()
  {
    population = 0;
    funds = 0;
  }
};

class CityServiceInfo::Impl
{
public:
  DateTime lastDate;
  std::vector< CityParameters > params;
};

CityServicePtr CityServiceInfo::create( City& city )
{
  CityServicePtr ret( new CityServiceInfo( city ) );
  ret->drop();

  return ret;
}

CityServiceInfo::CityServiceInfo( City& city )
  : CityService( city, "info" ), _d( new Impl )
{
  _d->lastDate = city.getDate();
  _d->params.resize( 12 );
}

void CityServiceInfo::update( const unsigned int time )
{
  if( time % 44 != 1 )
    return;

  if( _city.getDate().getMonth() != _d->lastDate.getMonth() )
  {
    _d->lastDate = _city.getDate();

    _d->params.erase( _d->params.begin() );
    _d->params.push_back( CityParameters() );

    CityParameters& last = _d->params.back();
    last.population = _city.getPopulation();
    last.funds = _city.getFunds();    
  }
}
