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

#include <map>

class CityFunds::Impl
{
public:
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

VariantMap CityFunds::save() const
{
  VariantMap ret;

  ret[ "money" ] = _d->money;
  
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
  _d->money = stream.get( "money" ).toInt();

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

void FundIssue::importGoods( CityPtr city, Good::Type type, int qty )
{
  CityTradeOptions& options = city->getTradeOptions();
  int price = options.getSellPrice( type );

  FundIssue issue( CityFunds::importGoods, -price * qty / 100 );
  city->getFunds().resolveIssue( issue );
}

void FundIssue::exportGoods( CityPtr city, Good::Type type, int qty )
{
  CityTradeOptions& options = city->getTradeOptions();
  int price = options.getBuyPrice( type );

  FundIssue issue( CityFunds::exportGoods, price * qty / 100 );
  city->getFunds().resolveIssue( issue );
}

void FundIssue::resolve( CityPtr city, int type, int money )
{
  FundIssue issue( type, money );
  city->getFunds().resolveIssue( issue );
}
