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

#include "oc3_empire_city_computer.hpp"
#include "oc3_empire.hpp"
#include "oc3_empire_trading.hpp"
#include "oc3_goodstore_simple.hpp"
#include "oc3_goodhelper.hpp"
#include "oc3_gamedate.hpp"
#include "oc3_foreach.hpp"

class ComputerCity::Impl
{
public:
  Point location;
  std::string name;
  EmpirePtr empire;
  bool distantCity;
  SimpleGoodStore sellStore;
  SimpleGoodStore buyStore;
  DateTime lastTimeUpdate;
  DateTime lastTimeMerchantSend;
  unsigned int merchantsNumber;
};

ComputerCity::ComputerCity( EmpirePtr empire, const std::string& name ) : _d( new Impl )
{
  _d->name = name;
  _d->distantCity = false;
  _d->empire = empire;
  _d->merchantsNumber = 0;
}

std::string ComputerCity::getName() const
{
  return _d->name;
}

Point ComputerCity::getLocation() const
{
  return _d->location;
}

void ComputerCity::setLocation( const Point& location )
{
  _d->location = location;
}

bool ComputerCity::isDistantCity() const
{
  return _d->distantCity;
}

void ComputerCity::save( VariantMap& options ) const
{
  options[ "location" ] = _d->location;

  VariantMap vm_sells;
  VariantMap vm_sold;
  VariantMap vm_buys;
  VariantMap vm_bought;

  for( int i=Good::G_NONE; i < Good::G_MAX; i ++ )
  {
    Good::Type gtype = Good::Type ( i );
    std::string tname = GoodHelper::getTypeName( gtype );
    int maxSellStock = _d->sellStore.getMaxQty( gtype );
    if( maxSellStock > 0 )
    {
      vm_sells[ tname ] = maxSellStock / 100;
    }

    int sold = _d->sellStore.getCurrentQty( gtype );
    if( sold > 0 )
    {
      vm_sold[ tname ] = sold / 100;
    }

    int maxBuyStock = _d->buyStore.getMaxQty( gtype );
    if( maxBuyStock > 0 )
    {
      vm_buys[ tname ] = maxBuyStock / 100;
    }

    int bought = _d->buyStore.getCurrentQty( gtype );
    if( bought > 0 )
    {
      vm_bought[ tname ] = bought / 100;
    }
  }

  options[ "sells" ] = vm_sells;
  options[ "buys" ] = vm_buys;
  options[ "sold" ] = vm_sold;
  options[ "bought" ] = vm_bought;
  options[ "lastTimeMerchantSend" ] = _d->lastTimeMerchantSend;
  options[ "lastTimeUpdate" ] = _d->lastTimeUpdate;
}

void ComputerCity::load( const VariantMap& options )
{
  setLocation( options.get( "location" ).toPoint() );

  Variant vTime = options.get( "lastTimeUpdate" );
  _d->lastTimeUpdate = vTime.isNull() ? GameDate::current() : vTime.toDateTime();

  vTime = options.get( "lastTimeMerchantSend" );
  _d->lastTimeMerchantSend = vTime.isNull() ? GameDate::current() : vTime.toDateTime();

  const VariantMap& sells_vm = options.get( "sells" ).toMap();
  for( VariantMap::const_iterator it=sells_vm.begin(); it != sells_vm.end(); it++ )
  {
    Good::Type gtype = GoodHelper::getType( it->first );
    _d->sellStore.setMaxQty( gtype, it->second.toInt() * 100 );
  }

  const VariantMap& sold_vm = options.get( "sold" ).toMap();
  for( VariantMap::const_iterator it=sold_vm.begin(); it != sold_vm.end(); it++ )
  {
    Good::Type gtype = GoodHelper::getType( it->first );
    _d->sellStore.setCurrentQty( gtype, it->second.toInt() * 100 );
  }

  const VariantMap& buys_vm = options.get( "buys" ).toMap();
  for( VariantMap::const_iterator it=buys_vm.begin(); it != buys_vm.end(); it++ )
  {
    Good::Type gtype = GoodHelper::getType( it->first );
    _d->buyStore.setMaxQty( gtype, it->second.toInt() * 100 );
  }

  const VariantMap& bought_vm = options.get( "bought" ).toMap();
  for( VariantMap::const_iterator it=bought_vm.begin(); it != bought_vm.end(); it++ )
  {
    Good::Type gtype = GoodHelper::getType( it->first );
    _d->buyStore.setCurrentQty( gtype, it->second.toInt() * 100 );
  }
}

const GoodStore& ComputerCity::getSells() const
{
  return _d->sellStore;
}

const GoodStore& ComputerCity::getBuys() const
{
  return _d->buyStore;
}

EmpireCityPtr ComputerCity::create( EmpirePtr empire, const std::string& name )
{
  EmpireCityPtr ret( new ComputerCity( empire, name ) );
  ret->drop();

  return ret;
}

void ComputerCity::resolveMerchantArrived( EmpireMerchantPtr merchant )
{
  GoodStore& sellGoods = merchant->getSellGoods();
  GoodStore& buyGoods = merchant->getBuyGoods();

  _d->buyStore.storeAll( buyGoods );
  _d->sellStore.storeAll( sellGoods );

  _d->merchantsNumber = (std::max<int>)( 0, _d->merchantsNumber-1);
}

ComputerCity::~ComputerCity()
{

}

void ComputerCity::timeStep( unsigned int time )
{
  //one year before step need
  if( _d->lastTimeUpdate.getMonthToDate( GameDate::current() ) > 11 )
  {
    _d->lastTimeUpdate = GameDate::current();

    for( int i=Good::G_NONE; i < Good::G_MAX; i ++ )
    {
      Good::Type gtype = Good::Type( i );
      _d->sellStore.setCurrentQty( gtype, _d->sellStore.getMaxQty( gtype ) );     
      _d->buyStore.setCurrentQty( gtype, 0  );
      _d->merchantsNumber = 0;
    }
  }

  if( _d->lastTimeMerchantSend.getMonthToDate( GameDate::current() ) > 2 ) 
  {
    EmpireTradeRouteList routes = _d->empire->getTradeRoutes( getName() );
    _d->lastTimeMerchantSend = GameDate::current();

    if( _d->merchantsNumber >= routes.size() )
    {
      return;
    }

    if( !routes.empty() )
    {
      SimpleGoodStore sellGoods, buyGoods;
      sellGoods.setMaxQty( 2000 );
      buyGoods.setMaxQty( 2000 );
      for( int i=Good::G_NONE; i < Good::G_MAX; i ++ )
      {
        Good::Type gtype = Good::Type( i );

        buyGoods.setMaxQty( gtype, _d->buyStore.getMaxQty( gtype ) );

        //how much space left
        int maxQty = (std::min)( _d->sellStore.getMaxQty( gtype ) / 4, sellGoods.getFreeQty() );
        
        //we want send merchants to all routes
        maxQty /= routes.size();

        int qty = math::clamp( _d->sellStore.getCurrentQty( gtype ), 0, maxQty );

        //have no goods to sell
        if( qty == 0 )
          continue;

        GoodStock& stock = sellGoods.getStock( gtype );  
        stock._maxQty = qty;
        
        //move goods to merchant's storage
        _d->sellStore.retrieve( stock, qty );
      }

      //send merchants to all routes
      foreach( EmpireTradeRoutePtr route, routes )
      {
        _d->merchantsNumber++;
        route->addMerchant( getName(), sellGoods, buyGoods );
      }
    }
  }
}

EmpirePtr ComputerCity::getEmpire() const
{
  return _d->empire;
}
