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

#include "oc3_cityservice_culture.hpp"
#include "oc3_city.hpp"
#include "oc3_safetycast.hpp"
#include "oc3_positioni.hpp"
#include "oc3_building_house.hpp"
#include "oc3_house_level.hpp"
#include "oc3_tile.hpp"
#include "oc3_time.hpp"
#include "oc3_gamedate.hpp"
#include "oc3_religion_building.hpp"

struct Coverage2Point{
  double coverage;
  int points;
} ;

typedef Coverage2Point CoveragePoints[6];
static const CoveragePoints religionPoints = { {1.0, 30}, {0.86,22}, {0.71,14}, {0.51, 9}, {0.31, 3}, {0.0, 0} };
static const CoveragePoints theatresPoints = { {1.0, 25}, {0.86,18}, {0.71,12}, {0.51, 8}, {0.31, 3}, {0.0, 0} };
static const CoveragePoints librariesPoints = { {1.0, 20}, {0.86,14}, {0.71,8}, {0.51, 4}, {0.31, 2}, {0.0, 0} };
static const CoveragePoints schoolsPoints = { {1.0, 15}, {0.86,10}, {0.71,6}, {0.51, 4}, {0.31, 1}, {0.0, 0} };
static const CoveragePoints academiesPoints = { {1.0, 10}, {0.86,7}, {0.71,4}, {0.51, 2}, {0.31, 1}, {0.0, 0} };

class CityServiceCulture::Impl
{
public:
  CityPtr city;
  DateTime lastDate;
  int culture;
  int parishionersCount;
  int theaterVisitors;
  int libraryVisitors;
  int schoolVisitors;
  int collegeVisitors;
  float religionCoverage;
  float theatersCoverage;
  float libraryCoverage;
  float schoolCoverage;
  float collegeCoverage;
  int religionPoints;
  int theatresPoints;
  int libraryPoints;
  int schoolPoints;
  int collegePoints;

  int convCoverage2Points( const CoveragePoints& covp, const double value )
  {
    for( int i=0; i < 6; i++ )
    {
      if( value >= covp[ i ].coverage )
      {
        return (int)(covp[ i ].points * value);
      }
    }

    return 0;
  }
};

CityServicePtr CityServiceCulture::create( CityPtr city )
{
  CityServicePtr ret( new CityServiceCulture( city ) );
  ret->drop();

  return ret;
}

CityServiceCulture::CityServiceCulture( CityPtr city )
  : CityService( "culture" ), _d( new Impl )
{
  _d->city = city;
  _d->lastDate = GameDate::current();
  _d->culture = 0;
}

void CityServiceCulture::update( const unsigned int time )
{
  if( time % 44 != 1 )
    return;

  if( _d->lastDate.getMonthToDate( GameDate::current() ) > 0 )
  {
    _d->lastDate = GameDate::current();
    _d->parishionersCount = 0;
    _d->theaterVisitors = 0;
    int cityPopulation = _d->city->getPopulation();

    const LandOverlays& overlays = _d->city->getOverlayList();

    for( LandOverlays::const_iterator it=overlays.begin(); it != overlays.end(); it++ )
    {
      TemplePtr temple = (*it).as<Temple>();

      if( temple.isValid() )
      {
        _d->parishionersCount += temple->getParishionerNumber();
      }
    }

    _d->religionCoverage = _d->parishionersCount / (float)cityPopulation;
    _d->religionPoints = _d->convCoverage2Points( religionPoints, _d->religionCoverage );

    for( LandOverlays::const_iterator it=overlays.begin(); it != overlays.end(); it++ )
    {
      TheaterPtr theater = (*it).as<Theater>();

      if( theater.isValid() )
      {
        _d->theaterVisitors += theater->getVisitorsNumber();
      }
    }
    _d->theatersCoverage = _d->theaterVisitors / (float)cityPopulation;
    _d->theatresPoints = _d->convCoverage2Points( theatresPoints, _d->theatersCoverage );

    for( LandOverlays::const_iterator it=overlays.begin(); it != overlays.end(); it++ )
    {
      LibraryPtr library = (*it).as<Library>();

      if( library.isValid() )
      {
        _d->libraryVisitors += library->getVisitorsNumber();
      }
    }
    _d->libraryCoverage = _d->libraryVisitors / (float)cityPopulation;
    _d->libraryPoints = _d->convCoverage2Points( librariesPoints, _d->libraryCoverage );

    for( LandOverlays::const_iterator it=overlays.begin(); it != overlays.end(); it++ )
    {
      SchoolPtr school = (*it).as<School>();

      if( school.isValid() )
      {
        _d->schoolVisitors += school->getVisitorsNumber();
      }
    }
    _d->schoolCoverage = _d->schoolVisitors / (float)cityPopulation;
    _d->schoolPoints = _d->convCoverage2Points( schoolsPoints, _d->schoolCoverage );

    for( LandOverlays::const_iterator it=overlays.begin(); it != overlays.end(); it++ )
    {
      CollegePtr college = (*it).as<College>();

      if( college.isValid() )
      {
        _d->collegeVisitors += college->getVisitorsNumber();
      }
    }
    _d->collegeCoverage = _d->collegeVisitors / (float)cityPopulation;
    _d->collegePoints = _d->convCoverage2Points( academiesPoints, _d->collegeCoverage );

    _d->culture = ( _d->culture + _d->religionPoints + _d->theatresPoints + 
                    _d->libraryPoints + _d->schoolPoints + _d->collegePoints ) / 2;
  }
}

int CityServiceCulture::getValue() const
{
  return _d->culture;
}
