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

#include "oc3_cityfunds.hpp"
#include "oc3_city.hpp"
#include "oc3_city_trade_options.hpp"
#include "oc3_building_house.hpp"

#include <map>

class CityFunds::Impl
{
public:
  int taxRate;
  int workerSalary;
  int money;
  int lastYeapUpdate;

  typedef std::map< CityFunds::IssueType, int > IssuesValue;
  typedef std::vector< IssuesValue > IssuesHistory;
  IssuesHistory history;

oc3_signals public:
  Signal1<int> onChangeSignal;
};

CityFunds::CityFunds() : _d( new Impl )
{
  _d->money = 0;
  _d->workerSalary = 30;
  _d->lastYeapUpdate = 0;
  _d->history.push_back( Impl::IssuesValue() );
}

void CityFunds::resolveIssue( FundIssue issue )
{
  switch( issue.type )
  {
  case unknown: _OC3_DEBUG_BREAK_IF( "wrong issue" ); break;
  default:
    {
      Impl::IssuesValue& step = _d->history.front();
      if( step.find( (IssueType)issue.type ) == step.end() )
      {
        step[ (IssueType)issue.type ] = 0;
      }

      step[ (IssueType)issue.type ] += abs( issue.money );
      _d->money += issue.money;
    }
  break;
  }

  _d->onChangeSignal.emit( _d->money );
}

int CityFunds::getValue() const
{
  return _d->money;
}

void CityFunds::updateHistory( const DateTime& date )
{
  if( _d->lastYeapUpdate >= date.getYear() )
  {
    return;
  }

  _d->lastYeapUpdate = date.getYear();
  _d->history.insert( _d->history.begin(), Impl::IssuesValue() );

  if( _d->history.size() > 2 )
  {
    _d->history.pop_back();
  }
}

int CityFunds::getIssueValue( IssueType type, int age ) const
{
  if( (unsigned int)age >= _d->history.size() )
    return 0;

  Impl::IssuesValue step = _d->history[ age ];
     
  Impl::IssuesValue::iterator it = step.find( type );
  return ( it == step.end() ) ? 0 : it->second;
}

int CityFunds::getTaxRate() const
{
  return _d->taxRate;
}

void CityFunds::setTaxRate(const unsigned int value)
{
  _d->taxRate = value;
}

int CityFunds::getWorkerSalary() const
{
  return _d->workerSalary;
}

void CityFunds::setWorkerSalary(const unsigned int value)
{
  _d->workerSalary = value;
}

VariantMap CityFunds::save() const
{
  VariantMap ret;

  ret[ "money" ] = _d->money;
  ret[ "taxRate" ] = _d->taxRate;
  ret[ "workerSalary" ] = _d->workerSalary;
  
  VariantList history;
  for( Impl::IssuesHistory::iterator stepIt=_d->history.begin(); stepIt != _d->history.end(); stepIt++ )
  {
    VariantList stepHistory;
    for( Impl::IssuesValue::iterator it = (*stepIt).begin(); it != (*stepIt).end(); it++ )
    {
      stepHistory.push_back( it->first );
      stepHistory.push_back( it->second );
    }

    history.push_back( stepHistory );
  }

  ret[ "history" ] = history;

  return ret;
}

void CityFunds::load( const VariantMap& stream )
{
  _d->money = (int)stream.get( "money", 0 );
  _d->taxRate = (int)stream.get( "taxRate", 7 );
  _d->workerSalary = (int)stream.get( "workerSalary", 30 );

  VariantList history = stream.get( "history" ).toList();
  for( VariantList::iterator it = history.begin(); it != history.end(); it++ )
  {
    _d->history.push_back( Impl::IssuesValue() );
    Impl::IssuesValue& last = _d->history.back();
    const VariantList& historyStep = (*it).toList();
    VariantList::const_iterator stepIt=historyStep.begin(); 
    while( stepIt != historyStep.end() )
    {
      IssueType type = (IssueType)stepIt->toInt(); stepIt++;
      int value = it->toInt(); stepIt++;
      
      last[ type ] = value;
    }
  }
}

CityFunds::~CityFunds()
{

}

Signal1<int>& CityFunds::onChange()
{
  return _d->onChangeSignal;
}


unsigned int CityStatistic::getCurrentWorkersNumber(CityPtr city)
{
  CityHelper helper( city );

  WorkingBuildingList buildings = helper.getBuildings<WorkingBuilding>( B_MAX );

  int workersNumber = 0;
  foreach( WorkingBuildingPtr bld, buildings )
  {
    workersNumber += bld->getWorkers();
  }

  return workersNumber;
}

unsigned int CityStatistic::getAvailableWorkersNumber(CityPtr city)
{
  CityHelper helper( city );

  HouseList houses = helper.getBuildings<House>( B_HOUSE );

  int avWrksNumber = 0;
  foreach( HousePtr house, houses )
  {
    avWrksNumber += house->getServiceValue( Service::workersRecruter );
  }

  return avWrksNumber;
}

unsigned int CityStatistic::getVacantionsNumber(CityPtr city)
{
  CityHelper helper( city );

  WorkingBuildingList buildings = helper.getBuildings<WorkingBuilding>( B_MAX );

  int workersNumber = 0;
  foreach( WorkingBuildingPtr bld, buildings )
  {
    workersNumber += bld->getMaxWorkers();
  }

  return workersNumber;
}

unsigned int CityStatistic::getMontlyWorkersWages(CityPtr city)
{
  int workersNumber = getCurrentWorkersNumber( city );

  if( workersNumber == 0 )
    return 0;

  //wages all worker in year
  //workers take salary in sestertius 1/100 part of dinarius
  int wages = workersNumber * city->getFunds().getWorkerSalary() / 100;

  wages = std::max<int>( wages, 1 );

  return wages;
}

unsigned int CityStatistic::getWorklessNumber(CityPtr city)
{
  CityHelper helper( city );

  HouseList houses = helper.getBuildings<House>( B_HOUSE );

  int worklessNumber = 0;
  foreach( HousePtr house, houses )
  {
    worklessNumber += house->getServiceValue( Service::workersRecruter );
  }

  return worklessNumber;
}
