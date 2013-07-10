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

#include "oc3_forum.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_picture.hpp"
#include "oc3_scenario.hpp"
#include "oc3_walker_taxcollector.hpp"

class Forum::Impl
{
public:
  int taxInThisMonth;
  int citizensReached;
};

Forum::Forum() : ServiceBuilding(S_FORUM, B_FORUM, Size(2)), _d( new Impl )
{
  _d->taxInThisMonth = 0;
  _d->citizensReached = 0;
  setMaxWorkers( 6 );
  setWorkers( 0 );
  setPicture( Picture::load( ResourceGroup::govt, 10));
}

void Forum::deliverService()
{
  if( getWorkers() > 0 && getWalkerList().size() == 0 )
  {
    TaxCollectorPtr walker = TaxCollector::create( Scenario::instance().getCity() );
    walker->send2City( this );

    if( !walker->isDeleted() )
    {
      addWalker( walker.as<Walker>() );
    }
  }
}

void Forum::timeStep( const unsigned long time )
{
  const Walkers& walkers = getWalkerList();
  for( Walkers::const_iterator it=walkers.begin(); it != walkers.end(); it++ )
  {
    if( (*it)->isDeleted() )
    {
      SmartPtr< TaxCollector > collector = (*it).as< TaxCollector >();
      if( collector.isValid() )
      {
        _d->taxInThisMonth += collector->getMoney();
        _d->citizensReached += collector->getCitizensReachedCount();
      }
    }
  }

  Building::timeStep( time );
}

int Forum::collectTaxes()
{
  int taxes = _d->taxInThisMonth;
  _d->taxInThisMonth = 0;
  _d->citizensReached = 0;
  return taxes;
}

int Forum::getPeoplesReached() const
{
  return _d->citizensReached;
}