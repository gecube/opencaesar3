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

#include "oc3_building_data.hpp"

#include <map>
#include "oc3_gettext.hpp"
#include "oc3_saveadapter.hpp"
#include "oc3_stringhelper.hpp"

BuildingData BuildingData::invalid = BuildingData( B_NONE, "unknown", 0 );

struct BuildingTypeEquale
{
  BuildingType type; 
  std::string name;
};

BuildingTypeEquale bldTypeEquales[] = { 
  { B_AMPHITHEATER,   "amphitheater"},  
  { B_THEATER,        "theater" },
  { B_HIPPODROME,     "hippodrome" },
  { B_COLLOSSEUM,     "colloseum" },
  { B_ACTOR,          "artist_colony" },
  { B_GLADIATOR,      "gladiator_pit" },
  { B_LION,           "lion_pit" },             
  { B_CHARIOT,        "chatioteer_school" },
  { B_HOUSE,          "house" },               
  { B_ROAD,           "road" },
  { B_PLAZA,          "plaza" },
  { B_GARDEN,         "garden" },             
  { B_SENATE,         "senate_1" },
  { B_FORUM,          "forum_1" },             
  { B_GOVERNOR_HOUSE, "governor_palace_1" },
  { B_GOVERNOR_VILLA, "governor_palace_2" },  
  { B_GOVERNOR_PALACE,"governor_palace_3" },
  { B_FORT_LEGIONNAIRE, "fort_legionaries" }, 
  { B_FORT_JAVELIN,   "fort_javelin" },
  { B_FORT_MOUNTED,   "fort_horse" },   
  { B_PREFECTURE,        "prefecture" },
  { B_BARRACKS,       "barracks" },         
  { B_MILITARY_ACADEMY, "military_academy" },
  { B_DOCTOR,         "clinic" },             
  { B_HOSPITAL,       "hospital" },
  { B_BATHS,          "baths" },               
  { B_BARBER,         "barber" }, 
  { B_SCHOOL,         "school" },             
  { B_COLLEGE,        "academy" },
  { B_LIBRARY,        "library" },           
  { B_MISSION_POST,   "mission post" },
  { B_TEMPLE_CERES,   "small_ceres_temple" }, 
  { B_TEMPLE_NEPTUNE, "small_neptune_temple" },
  { B_TEMPLE_MARS,    "small_mars_temple" },   
  { B_TEMPLE_MERCURE, "small_mercury_temple" },
  { B_TEMPLE_VENUS,   "small_venus_temple" },
  { B_BIG_TEMPLE_CERES,  "big_ceres_temple" },
  { B_BIG_TEMPLE_NEPTUNE,"big_neptune_temple" },
  { B_BIG_TEMPLE_MARS,   "big_mars_temple"},
  { B_BIG_TEMPLE_MERCURE,"big_mercury_temple"},
  { B_BIG_TEMPLE_VENUS,  "big_venus_temple"},
  { B_TEMPLE_ORACLE,     "oracle"},
  { B_MARKET,            "market"},
  { B_GRANARY,        "granery"},
  { B_WAREHOUSE,      "warehouse"},
  { B_WHEAT_FARM,          "wheat_farm"},
  { B_FRUIT,          "fig_farm"},
  { B_VEGETABLE,      "vegetable_farm"},
  { B_OLIVE,          "olive_farm"},
  { B_GRAPE,          "vinard"},
  { B_MEAT,           "meat_farm"},
  { B_MARBLE,         "quarry"},
  { B_IRON_MINE,           "iron_mine"},
  { B_TIMBER,         "lumber_mill"},
  { B_CLAY_PIT,       "clay_pit"},
  { B_WINE,           "wine_workshop"},
  { B_OIL,            "oil_workshop"},
  { B_WEAPON,         "weapons_workshop"},
  { B_FURNITURE,      "furniture_workshop"},
  { B_POTTERY,        "pottery_workshop"},
  { B_ENGINEER_POST,       "engineering_post"},
  { B_STATUE1,        "statue_small"},
  { B_STATUE2,        "statue_middle"},
  { B_STATUE3,        "statue_big"},
  { B_LOW_BRIDGE,     "low_bridge"},
  { B_HIGH_BRIDGE,    "high_bridge"},
  { B_DOCK,           "dock"},
  { B_SHIPYARD,       "shipyard"},
  { B_WHARF,          "wharf"},
  { B_TRIUMPHAL_ARCH, "triumphal_arch"},
  { B_WELL,           "well"},
  { B_FOUNTAIN,       "fountain"},
  { B_AQUEDUCT,       "aqueduct"},
  { B_RESERVOIR,      "reservoir"},
  { B_NATIVE_HUT,     "native_hut"},
  { B_NATIVE_CENTER,  "native_center"},
  { B_NATIVE_FIELD,   "native_field"},
  { B_BURNING_RUINS,  "burning_ruins"},
  { B_BURNED_RUINS,   "burned_ruins"},
  { B_COLLAPSED_RUINS,"collapsed_ruins"},
  { B_FORUM_2, "forum_2" },
  { B_GATEHOUSE, "gatehouse" },
  { B_SENATE_2, "senate_2" },
  { B_TOWER, "tower" },
  { B_WALL, "wall"  },

  { B_NONE, "" }
};

BuildingData::BuildingData(const BuildingType buildingType, const std::string &name, const int cost)
{
  _buildingType = buildingType;
  _name = name;
  std::string key = "building_"+name;
  _prettyName = _(key.c_str());  // i18n translation
  _cost = cost;
  _baseDesirability = 0;
  _desirabilityRange = 0;
  _desirabilityStep = 0;
}

std::string BuildingData::getName() const
{
  return _name;
}

std::string BuildingData::getPrettyName() const
{
  return _prettyName;
}

BuildingType BuildingData::getType() const
{
  return _buildingType;
}

int BuildingData::getCost() const
{
  return _cost;
}

int BuildingData::getEmployers() const
{
  return _employers;
}

std::string BuildingData::getResouceGroup() const
{
  return _resourceGroup;
}

int BuildingData::getResourceIndex() const
{
  return _rcIndex;
}

char BuildingData::getDesirbilityInfluence() const
{
  return _baseDesirability;
}

char BuildingData::getDesirbilityRange() const
{
  return _desirabilityRange;
}

char BuildingData::getDesirabilityStep() const
{
  return _desirabilityStep;
}

class BuildingDataHolder::Impl
{
public:
  typedef std::map<BuildingType, BuildingData> BuildingsMap; 
  typedef std::map<GoodType, BuildingType> FactoryInMap;

  BuildingsMap buildings;// key=building_type, value=data
  FactoryInMap mapBuildingByInGood;
};

BuildingDataHolder& BuildingDataHolder::instance()
{
  static BuildingDataHolder inst;
  return inst;
}

BuildingType BuildingDataHolder::getBuildingTypeByInGood(const GoodType inGoodType)
{
  BuildingType res = B_NONE;

  Impl::FactoryInMap::iterator mapIt;
  mapIt = _d->mapBuildingByInGood.find(inGoodType);
  if (mapIt != _d->mapBuildingByInGood.end())
  {
    res = mapIt->second;
  }
  return res;
}

BuildingData& BuildingDataHolder::getData(const BuildingType buildingType)
{
  Impl::BuildingsMap::iterator mapIt;
  mapIt = _d->buildings.find(buildingType);
  if (mapIt == _d->buildings.end())
  {
    StringHelper::debug( 0xff, "Unknown building %d", buildingType );
    return BuildingData::invalid;
  }
  return mapIt->second;
}

bool BuildingDataHolder::hasData(const BuildingType buildingType)
{
  bool res = true;
  Impl::BuildingsMap::iterator mapIt;
  mapIt = _d->buildings.find(buildingType);
  if (mapIt == _d->buildings.end())
  {
    res = false;
  }
  return res;
}

void BuildingDataHolder::addData(const BuildingData &data)
{
  BuildingType buildingType = data.getType();

  if (hasData(buildingType))
  {
    StringHelper::debug( 0xff, "Building is already set %s", data.getName().c_str() );
    return;
  }

  _d->buildings.insert(std::make_pair(buildingType, data));
}


BuildingDataHolder::BuildingDataHolder() : _d( new Impl )
{
}

void BuildingDataHolder::initialize( const std::string& filename )
{
  // populate _mapBuildingByInGood
  _d->mapBuildingByInGood[G_IRON]   = B_WEAPON;
  _d->mapBuildingByInGood[G_TIMBER] = B_FURNITURE;
  _d->mapBuildingByInGood[G_CLAY]   = B_POTTERY;
  _d->mapBuildingByInGood[G_OLIVE]  = B_OIL;
  _d->mapBuildingByInGood[G_GRAPE]  = B_WINE;

  VariantMap constructions = SaveAdapter::load( filename );

  for( VariantMap::iterator it=constructions.begin(); it != constructions.end(); it++ )
  {
    VariantMap options = (*it).second.toMap();

    const BuildingType btype = getType( (*it).first );
    if( btype == B_NONE )
    {
      StringHelper::debug( 0xff, "!!!Warning: can't associate type with %s", (*it).first.c_str() );
      continue;
    }

    Impl::BuildingsMap::const_iterator bdataIt = _d->buildings.find( btype );
    if( bdataIt != _d->buildings.end() )
    {
      StringHelper::debug( 0xff, "!!!Warning: type %s also initialized", (*it).first.c_str() );
      continue;
    }

    BuildingData bData( btype, (*it).first, options[ "cost" ].toInt() );
    const std::string pretty = options[ "pretty" ].toString();
    if( !pretty.empty() )
      bData._prettyName = pretty;

    bData._baseDesirability  = options[ "desirability" ].toInt();
    bData._desirabilityRange = options[ "desrange" ].toInt();
    bData._desirabilityStep  = options[ "desstep" ].toInt();
    bData._employers = options[ "employers" ].toInt();
    bData._resourceGroup = options[ "resource" ].toString();
    bData._rcIndex = options[ "rcindex" ].toInt();

    addData( bData );
  }
}

BuildingType BuildingDataHolder::getType( const std::string& name )
{
  int index=0;
  std::string typeName = bldTypeEquales[ index ].name;

  while( typeName.size() > 0 )
  {
    if( name == typeName )
      return bldTypeEquales[ index ].type;

    index++;
    typeName = bldTypeEquales[ index ].name;
  }

  return B_NONE;
}