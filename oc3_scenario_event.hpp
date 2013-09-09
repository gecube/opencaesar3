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

#ifndef _OPENCAESAR3_SCENARIO_EVENT_H_INCLUDE_
#define _OPENCAESAR3_SCENARIO_EVENT_H_INCLUDE_

#include "oc3_referencecounted.hpp"
#include "oc3_smartptr.hpp"
#include "oc3_positioni.hpp"
#include "oc3_building.hpp"
#include "oc3_predefinitions.hpp"

class ScenarioEvent : public ReferenceCounted
{
public:
  virtual void exec( CityPtr city ) = 0;

protected:
  ScenarioEvent() {}
};

typedef SmartPtr< ScenarioEvent > ScenarioEventPtr;

class DisasterEvent : public ScenarioEvent
{
public:
  typedef enum
  {
    fire, collapse, plague, count
  } Type;
  static void create( const TilePos&, Type type );

  virtual void exec(CityPtr city);

private:
  TilePos _pos;
  Type _type;
};


class BuildEvent : public ScenarioEvent
{
public:
  static void create( const TilePos&, BuildingType type );
  static void create( const TilePos& pos, ConstructionPtr building );

  virtual void exec( CityPtr city );
private:
  TilePos _pos;
  ConstructionPtr _building;
};

class ClearLandEvent : public ScenarioEvent
{
public:
  static void create( const TilePos& );
  virtual void exec( CityPtr city );
private:
  TilePos _pos;
};

class ShowInfoboxEvent : public ScenarioEvent
{
public:
  static void create( const std::string& title, const std::string& text );
  virtual void exec( CityPtr city );
private:
  std::string _title, _text;
};

#endif //_OPENCAESAR3_CITY_EVENT_H_INCLUDE_
