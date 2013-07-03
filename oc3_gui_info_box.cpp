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

#include <cstdio>

#include "oc3_gui_info_box.hpp"

#include "oc3_tile.hpp"
#include "oc3_exception.hpp"
#include "oc3_gettext.hpp"
#include "oc3_gui_paneling.hpp"
#include "oc3_building_data.hpp"
#include "oc3_house_level.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_event.hpp"
#include "oc3_texturedbutton.hpp"
#include "oc3_label.hpp"
#include "oc3_city.hpp"
#include "oc3_scenario.hpp"
#include "oc3_market.hpp"
#include "oc3_granary.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_goodhelper.hpp"
#include "oc3_farm.hpp"
#include "oc3_house.hpp"
#include "oc3_religion_building.hpp"
#include "oc3_divinity.hpp"
#include "oc3_warehouse.hpp"
#include "oc3_gfx_engine.hpp"
#include "oc3_special_orders_window.hpp"
#include "oc3_goodstore.hpp"
#include "oc3_groupbox.hpp"

class GuiInfoBox::Impl
{
public:
  PictureRef bgPicture;
  Label* lbTitle;
  PushButton* btnExit;
  PushButton* btnHelp;
  bool isAutoPosition;
};

GuiInfoBox::GuiInfoBox( Widget* parent, const Rect& rect, int id )
: Widget( parent, id, rect ), _d( new Impl )
{
  // create the title
  _d->lbTitle = new Label( this, Rect( 50, 10, getWidth()-50, 10 + 30 ), "", true );
  _d->lbTitle->setFont( Font::create( FONT_3 ) );
  _d->lbTitle->setTextAlignment( alignCenter, alignCenter );
  _d->isAutoPosition = true;

  _d->btnExit = new TexturedButton( this, Point( 472, getHeight() - 39 ), Size( 24 ), -1, ResourceMenu::exitInfBtnPicId );
  _d->btnExit->setTooltipText( _("##infobox_tooltip_exit##") );

  _d->btnHelp = new TexturedButton( this, Point( 14, getHeight() - 39 ), Size( 24 ), -1, ResourceMenu::helpInfBtnPicId );
  _d->btnHelp->setTooltipText( _("##infobox_tooltip_help##") );

  CONNECT( _d->btnExit, onClicked(), this, InfoBoxLand::deleteLater );

  _d->bgPicture.reset( Picture::create( getSize() ) );

  // draws the box and the inner black box
  GuiPaneling::instance().draw_white_frame(*_d->bgPicture, 0, 0, getWidth(), getHeight() );
}

GuiInfoBox::~GuiInfoBox()
{
  
}

void GuiInfoBox::draw( GfxEngine& engine )
{
  engine.drawPicture( getBgPicture(), getScreenLeft(), getScreenTop() );
  Widget::draw( engine );
}

Picture& GuiInfoBox::getBgPicture()
{
  return *_d->bgPicture;
}

bool GuiInfoBox::isPointInside( const Point& point ) const
{
  //resolve all screen for self using
  return getParent()->getAbsoluteRect().isPointInside( point );
}

bool GuiInfoBox::onEvent( const NEvent& event)
{
  switch( event.EventType )
  {
  case OC3_MOUSE_EVENT:
    if( event.MouseEvent.Event == OC3_RMOUSE_LEFT_UP )
    {
      deleteLater();
      return true;
    }
    else if( event.MouseEvent.Event == OC3_LMOUSE_LEFT_UP )
    {
      return true;
    }
    break;
  }

  return Widget::onEvent( event );
}

void GuiInfoBox::setTitle( const std::string& title )
{
  _d->lbTitle->setText( title );
}

bool GuiInfoBox::isAutoPosition() const
{
  return _d->isAutoPosition;
}

void GuiInfoBox::setAutoPosition( bool value )
{
  _d->isAutoPosition = value;
}

class GuiInfoService::Impl
{
public:
  Label* dmgLabel;
  Label* lbHelp;
  ServiceBuildingPtr building;
};

GuiInfoService::GuiInfoService( Widget* parent, ServiceBuildingPtr building)
    : GuiInfoBox( parent, Rect( 0, 0, 510, 256 ), -1 ), _sd( new Impl )
{
  _sd->building = building;
  setTitle( BuildingDataHolder::instance().getData( building->getType() ).getPrettyName() );
  paint(); 
}

void GuiInfoService::paint()
{
  GuiPaneling::instance().draw_black_frame( *_d->bgPicture, 16, 136, getWidth() - 32, 62 );
  
  if( _sd->building->getMaxWorkers() > 0 )
  {
    drawWorkers( 150 );
  }

  _sd->dmgLabel = new Label( this, Rect( 50, getHeight() - 50, getWidth() - 50, getHeight() - 16 ) ); 
  std::string text = StringHelper::format( 0xff, "%d%% damage - %d%% fire", 
                                           (int)_sd->building->getDamageLevel(), (int)_sd->building->getFireLevel());
  _sd->dmgLabel->setText( text );

  _sd->lbHelp = new Label( this, Rect( 16, 50, getWidth() - 16, 130 ) );
}

void GuiInfoService::setText(const std::string& text)
{
  _sd->lbHelp->setText( text );
}

void GuiInfoService::drawWorkers( int paintY )
{
  // picture of citizen
  Picture& pic = Picture::load( ResourceGroup::panelBackground, 542);
  _d->bgPicture->draw( pic, 16+15, paintY);

  // number of workers
  std::string text = StringHelper::format( 0xff, _("%d employers (%d requred)"), 
                                            _sd->building->getWorkers(), _sd->building->getMaxWorkers() );

  Font font = Font::create( FONT_2 );
  font.draw( *_d->bgPicture, text, 16 + 42, paintY + 5 );
}


class InfoBoxHouse::Impl
{
public:
  HousePtr house;
  Label* lbHabitants;
  Label* lbCrime;
};

InfoBoxHouse::InfoBoxHouse( Widget* parent, const Tile& tile )
    : GuiInfoBox( parent, Rect( 0, 0, 510, 360 ), -1 ),
      _ed( new Impl )
{
  _ed->house = tile.getTerrain().getOverlay().as<House>();
  setTitle( _ed->house->getName() );
  _paint();
}


void InfoBoxHouse::_paint()
{
  int lbHeight = 20;
  GuiPaneling::instance().draw_black_frame( *_d->bgPicture, 16, 150, 
                                               _d->btnExit->getRight() - _d->btnHelp->getLeft(), 
                                               _d->btnExit->getTop() - 150 - 5 );

  drawHabitants();
    
  int taxes = -1; // _house->getMonthlyTaxes();
  Label* taxesLb = new Label( this, Rect( 16 + 15, _ed->lbHabitants->getBottom(), getWidth() - 16, 
                                            _ed->lbHabitants->getBottom() + lbHeight ), "", false, true );
  char buffer[200];
  if (taxes == -1)
  {
    sprintf(buffer, _("Aucun percepteur ne passe ici. Ne paye pas de taxes"));
    taxesLb->setFont( Font::create( FONT_2_RED ) );
  }
  else
  {
    sprintf(buffer, _("Paye %d Denarii de taxes par mois"), taxes);
  }

  taxesLb->setText( buffer );
  //_paintY+=22;
  
  _ed->lbCrime = new Label( this, taxesLb->getRelativeRect() + Point( 0, 22 ), "", false, true );
  sprintf(buffer, _("Inhabitants didn't report about crimes"));
  _ed->lbCrime->setText( buffer );

  int startY = _ed->lbCrime->getBottom() + 10;
  if( _ed->house->getLevelSpec().getHouseLevel() > 2 )
  {
    drawGood(G_WHEAT, 0, 0, startY );
  }
  else
  {
    Label* lb = new Label( this, _ed->lbCrime->getRelativeRect() + Point( 0, 30 ), "", false, true ); 
    lb->setHeight( 40 );
    lb->setLineIntervalOffset( -6 );
    lb->setText( _("Inabitants of tents provide food themselves, conducting a subsistence economy") );
    lb->setWordWrap( true );
    startY = lb->getTop();
  }

  drawGood(G_POTTERY, 0, 1, startY);
  drawGood(G_FURNITURE, 1, 1, startY);
  drawGood(G_OIL, 2, 1, startY);
  drawGood(G_WINE, 3, 1, startY);
}


void InfoBoxHouse::drawHabitants()
{
  // citizen or patrician picture
  int picId = _ed->house->getLevelSpec().isPatrician() ? 541 : 542; 
   
  Picture& citPic = Picture::load( ResourceGroup::panelBackground, picId );
  _d->bgPicture->draw( citPic, 16+15, 157 );

  // number of habitants
  _ed->lbHabitants = new Label( this, Rect( 60, 157, getWidth() - 16, 157 + citPic.getHeight() ), "", false, true );
  char buffer[200];
  int freeRoom = _ed->house->getMaxHabitants() - _ed->house->getNbHabitants();
  if( freeRoom > 0 )
  {
    // there is some room for new habitants!
    sprintf(buffer, _("%d citizens, additional rooms for %d"), _ed->house->getNbHabitants(), freeRoom);
  }
  else if (freeRoom == 0)
  {
    // full house!
    sprintf(buffer, _("%d citizens"), _ed->house->getNbHabitants());
  }
  else if (freeRoom < 0)
  {
    // too many habitants!
    sprintf(buffer, _("%d citizens, %d habitants en trop"), _ed->house->getNbHabitants(), -freeRoom);
    _ed->lbHabitants->setFont( Font::create( FONT_2_RED ) );
  }

  _ed->lbHabitants->setText( buffer );
}

void InfoBoxHouse::drawGood(const GoodType &goodType, const int col, const int row, const int startY )
{
  Font font = Font::create( FONT_2 );
  int qty = _ed->house->getGoodStore().getCurrentQty(goodType);

  // pictures of goods
  const Picture& pic = GoodHelper::getPicture( goodType );
  _d->bgPicture->draw(pic, 31 + 100 * col, startY + 2 + 30 * row);

  std::string text = StringHelper::format( 0xff, "%d", qty);
  font.draw( *_d->bgPicture, text, 61 + 100 * col, startY + 30 * row );
}

GuiInfoFactory::GuiInfoFactory( Widget* parent, const Tile& tile)
    : GuiInfoBox( parent, Rect( 0, 0, 510, 256 ), -1 )
{
  _building = tile.getTerrain().getOverlay().as<Factory>();
  setTitle( BuildingDataHolder::instance().getData( _building->getType() ).getPrettyName() );
  paint();
}

void GuiInfoFactory::paint()
{
   // paint picture of out good
   //Font &font_red = FontCollection::instance().getFont(FONT_2_RED);
  Font font = Font::create( FONT_2 );

  // paint progress
  int progress = _building->getProgress();
  int _paintY = _d->lbTitle->getBottom();

  std::string text = StringHelper::format( 0xff, _("Le travail est a %d%% termine."), progress );
  font.draw(*_d->bgPicture, text, 32, _paintY);
  _paintY+=22;

  if( _building->getOutGoodType() != G_NONE )
  {
    const Picture &pic = GoodHelper::getPicture( _building->getOutGoodType() );
    _d->bgPicture->draw(pic, 10, 10);
  }

  // paint picture of in good
  if( _building->getInGood()._goodType != G_NONE )
  {
    const Picture &pic = GoodHelper::getPicture( _building->getInGood()._goodType );
    _d->bgPicture->draw(pic, 32, _paintY+2);
    int amount = _building->getInGood()._currentQty / 100;
    std::string goodName = GoodHelper::getName( _building->getInGood()._goodType );
   
    text = StringHelper::format( 0xff, _("%s en stock: %d unites"), goodName.c_str(), amount );
    
    font.draw( *_d->bgPicture, text, 32 + 25, _paintY);
  }

  GuiPaneling::instance().draw_black_frame( *_d->bgPicture, 16, 147, getWidth()-32, 62);
  drawWorkers( 147 + 10 );
}


void GuiInfoFactory::drawWorkers( int paintY )
{
   // picture of citizen
   Picture *pic = &Picture::load( ResourceGroup::panelBackground, 542 );
   _d->bgPicture->draw( *pic, 16+15, paintY);

   // number of workers
   std::string text = StringHelper::format( 0xff, _("%d employes (%d requis)"), _building->getWorkers(), _building->getMaxWorkers());

   Font font = Font::create( FONT_2 );
   font.draw(*_d->bgPicture, text, 16+42, paintY+5 );
}


std::string GuiInfoFactory::getInfoText()
{
   std::string textKey = GoodHelper::getName( _building->getOutGood()._goodType );
   if (_building->isActive() == false)
   {
      textKey+= "- Production arretee sous ordre du gouverneur";
   }
   else if (_building->getInGood()._goodType != G_NONE && _building->getInGood()._currentQty == 0)
   {
      textKey+= "- Production arretee par manque de matiere premiere";
   }
   else
   {
      int nbWorkers = _building->getWorkers();

      if (nbWorkers == 0)
      {
         textKey+= "- Production arretee par manque de personnel";
      }
      else if (nbWorkers < 5)
      {
         textKey+= "- Production quasimment a l'arret par manque de main-d'oeuvre";
      }
      else if (nbWorkers < 7)
      {
         textKey+= "- Production tres ralentie par manque d'employes";
      }
      else if (nbWorkers < 9)
      {
         textKey+= "- Production limitee par manque de personnel";
      }
      else if (nbWorkers < 10)
      {
         textKey+= "- Production legerement ralentie par manque d'employes";
      }
      else
      {
         textKey+= "- Production a son maximum";
      }

   }

   return _(textKey.c_str());
}

class GuiInfoGranary::Impl
{
public:
   GranaryPtr building;
   PushButton* btnOrders;
};

GuiInfoGranary::GuiInfoGranary( Widget* parent, const Tile& tile )
    : GuiInfoBox( parent, Rect( 0, 0, 510, 280 ), -1 ), _gd( new Impl )
{
  _gd->building = tile.getTerrain().getOverlay().as<Granary>();
  Size btnOrdersSize( 350, 20 );
  _gd->btnOrders = new PushButton( this, Rect( Point( (getWidth() - btnOrdersSize.getWidth())/ 2, getHeight() - 34 ), btnOrdersSize), 
                                   _("##special_orders##"), -1, false, PushButton::WhiteBorderUp );
  CONNECT( _gd->btnOrders, onClicked(), this, GuiInfoGranary::showSpecialOrdersWindow );

  setTitle( BuildingDataHolder::instance().getData( _gd->building->getType()).getPrettyName() );

  paint();
}


void GuiInfoGranary::showSpecialOrdersWindow()
{
  Point pos;
  if( getTop() > getParent()->getHeight() / 2 )
  {
    pos = Point( getScreenLeft(), getScreenBottom() - 450 );   
  }
  else
  {
    pos = getAbsoluteRect().UpperLeftCorner;
  }

  new GranarySpecialOrdersWindow( getParent(), pos, _gd->building );
}


void GuiInfoGranary::paint()
{
   //Font &font_red = FontCollection::instance().getFont(FONT_2_RED);
  Font font = Font::create( FONT_2 );

  // summary: total stock, free capacity
  int _paintY = _d->lbTitle->getBottom();
  int currentQty = _gd->building->getGoodStore().getCurrentQty();
  int maxQty = _gd->building->getGoodStore().getMaxQty();
  std::string desc = StringHelper::format( 0xff, _("%d unites en stock. Espace pour %d unites."), currentQty, maxQty-currentQty);

  font.draw( *_d->bgPicture, desc, 16, _paintY+5 );
  _paintY+=40;

  int _col2PaintY = _paintY;
  drawGood(G_WHEAT, 0, _paintY);
  drawGood(G_FISH, 0, _paintY);
  drawGood(G_MEAT, 0, _paintY);
  drawGood(G_FRUIT, 1, _col2PaintY);
  drawGood(G_VEGETABLE, 1, _col2PaintY);

  _paintY+=12;
  GuiPaneling::instance().draw_black_frame(*_d->bgPicture, 16, _paintY, getWidth()-32, 62);
  _paintY+=12;

  drawWorkers( _paintY );
}


void GuiInfoGranary::drawWorkers( int paintY )
{
   // picture of citizen
   Picture& pic = Picture::load( ResourceGroup::panelBackground, 542);
   _d->bgPicture->draw(pic, 16+15, paintY);

   // number of workers
   std::string text = StringHelper::format( 0xff, _("%d employers (%d requires)"), _gd->building->getWorkers(), _gd->building->getMaxWorkers());

   Font font = Font::create( FONT_2 );
   font.draw(*_d->bgPicture, text, 16+42, paintY+5 );
}


void GuiInfoGranary::drawGood(const GoodType &goodType, int col, int& paintY)
{
  std::string goodName = GoodHelper::getName( goodType );

  Font font = Font::create( FONT_2 );
  int qty = _gd->building->getGoodStore().getCurrentQty(goodType);

  // pictures of goods
  const Picture& pic = GoodHelper::getPicture( goodType );
  _d->bgPicture->draw(pic, (col == 0 ? 31 : 250), paintY);

  std::string outText = StringHelper::format( 0xff, "%d %s", qty, goodName.c_str() );
  font.draw( *_d->bgPicture, outText, (col == 0 ? 61 : 280), paintY );
  paintY += 25;
}

class InfoBoxWarehouse::Impl
{
public:
  WarehousePtr building;
  PushButton* btnOrders;
  Point workerFramePos;
};

InfoBoxWarehouse::InfoBoxWarehouse( Widget* parent, const Tile& tile )
: GuiInfoBox( parent, Rect( 0, 0, 510, 360 ), -1 ), _wd( new Impl )
{
  _wd->building = tile.getTerrain().getOverlay().as<Warehouse>();
  Size btnOrdersSize( 350, 20 );
  _wd->btnOrders = new PushButton( this, Rect( Point( (getWidth() - btnOrdersSize.getWidth()) / 2, getHeight() - 34 ), btnOrdersSize ), 
                                   _("##special_orders##"), -1, false, PushButton::WhiteBorderUp );

  CONNECT( _wd->btnOrders, onClicked(), this, InfoBoxWarehouse::showSpecialOrdersWindow );

  setTitle( BuildingDataHolder::instance().getData( _wd->building->getType()).getPrettyName() );

  paint();
}

void InfoBoxWarehouse::showSpecialOrdersWindow()
{
  Point pos;
  if( getTop() > getParent()->getHeight() / 2 )
  {
    pos = Point( getScreenLeft(), getScreenBottom() - 450 );   
  }
  else
  {
    pos = getAbsoluteRect().UpperLeftCorner;
  }

  new WarehouseSpecialOrdersWindow( getParent(), pos, _wd->building );
}

void InfoBoxWarehouse::paint()
{
  //Font &font_red = FontCollection::instance().getFont(FONT_2_RED);
  Font font = Font::create( FONT_2 );

  // summary: total stock, free capacity
  int _paintY = _d->lbTitle->getBottom();

  int _col2PaintY = _paintY;
  drawGood(G_WHEAT, 0, _paintY);
  drawGood(G_VEGETABLE, 0, _paintY);
  drawGood(G_FRUIT, 0, _paintY);
  drawGood(G_OLIVE, 0, _paintY);
  drawGood(G_GRAPE, 0, _paintY);

  _paintY = _col2PaintY;
  drawGood(G_MEAT, 1, _paintY);
  drawGood(G_WINE, 1, _paintY);
  drawGood(G_OIL, 1, _paintY);
  drawGood(G_IRON, 1, _paintY);
  drawGood(G_TIMBER, 1, _paintY);

  _paintY = _col2PaintY;
  drawGood(G_CLAY, 2, _paintY);
  drawGood(G_MARBLE, 2, _paintY);
  drawGood(G_WEAPON, 2, _paintY);
  drawGood(G_FURNITURE, 2, _paintY);
  drawGood(G_POTTERY, 2, _paintY);

  _wd->workerFramePos = Point( 16, 170 );
  GuiPaneling::instance().draw_black_frame(*_d->bgPicture, _wd->workerFramePos.getX(), _wd->workerFramePos.getY(), getWidth()-32, 62);

  drawWorkers();
}


void InfoBoxWarehouse::drawWorkers()
{
  // picture of citizen
  Picture& pic = Picture::load( ResourceGroup::panelBackground, 542);
  _d->bgPicture->draw(pic, _wd->workerFramePos + Point( 20, 10 ) );

  // number of workers
  std::string text = StringHelper::format( 0xff, _("%d employers (%d requires)"), _wd->building->getWorkers(), _wd->building->getMaxWorkers());

  Font font = Font::create( FONT_2 );
  font.draw(*_d->bgPicture, text, _wd->workerFramePos + Point( 40, 10 ) );
}


void InfoBoxWarehouse::drawGood(const GoodType &goodType, int col, int& paintY)
{
  std::string goodName = GoodHelper::getName( goodType );

  Font font = Font::create( FONT_2 );
  int qty = _wd->building->getGoodStore().getCurrentQty(goodType);

  // pictures of goods
  const Picture& pic = GoodHelper::getPicture( goodType );
  _d->bgPicture->draw(pic, col * 150 + 15, paintY);

  std::string outText = StringHelper::format( 0xff, "%d %s", qty, goodName.c_str() );
  font.draw( *_d->bgPicture, outText, col * 150 + 45, paintY );
  paintY += 25;
}

class InfoBoxTemple::Impl
{
public:
  TemplePtr temple;
  Font font;
};

InfoBoxTemple::InfoBoxTemple( Widget* parent, const Tile& tile )
  : GuiInfoBox( parent, Rect( 0, 0, 510, 256 ), -1 ), _td( new Impl )
{
  _td->font = Font::create( FONT_2 );
  _td->temple = tile.getTerrain().getOverlay().as<Temple>();
  RomeDivinityPtr divn = _td->temple->getDivinity();

  std::string text = StringHelper::format( 0xff, "##Temple of ##%s (%s)", 
                                                 divn->getName().c_str(), 
                                                 divn->getShortDescription().c_str() );
  setTitle( text );

  drawWorkers();
  drawPicture();
}

void InfoBoxTemple::drawWorkers()
{
  int y = 56;
  GuiPaneling::instance().draw_black_frame(*_d->bgPicture, 16, y, getWidth() - 32, 62 );
  // picture of citizen
  Picture& pic = Picture::load( ResourceGroup::panelBackground, 542 );
  _d->bgPicture->draw( pic, 16+15, y + 12);

  // number of workers
  std::string text = StringHelper::format( 0xff, _("%d employers (%d requires)"), 
                                                 _td->temple->getWorkers(), 
                                                 _td->temple->getMaxWorkers());

  _td->font.draw( *_d->bgPicture, text, 16 + 15 + pic.getWidth() + 5, y+18 );
}

void InfoBoxTemple::drawPicture()
{
  const Picture& pic = _td->temple->getDivinity()->getPicture();
  _d->bgPicture->draw( pic, 192, 140 );
}

class GuiInfoMarket::Impl
{
public:
  MarketPtr market;
  Font goodFont;
  Label* lbAbout;

};

GuiInfoMarket::GuiInfoMarket( Widget* parent, const Tile& tile )
    : GuiInfoBox( parent, Rect( 0, 0, 510, 256 ), -1 ), _md( new Impl )
{
   _md->market = tile.getTerrain().getOverlay().as<Market>();
   _md->goodFont = Font::create( FONT_2 );
   _md->lbAbout = new Label( this, _d->lbTitle->getRelativeRect() + Point( 0, 30 ) );
   _md->lbAbout->setWordWrap( true );

   setTitle( _("building_market") );
   paint();
}


void GuiInfoMarket::paint()
{
  if( _md->market->getWorkers() > 0 )
  {
    GoodStore& goods = _md->market->getGoodStore();
    int furageSum = 0;
    // for all furage types of good
    for (int goodType = 0; goodType<G_OLIVE; ++goodType)
    {
      furageSum += goods.getCurrentQty( (GoodType)goodType );     
    }

    int paintY = 78;
    if( 0 < furageSum )
    {
      drawGood(G_WHEAT, 0, paintY );
      drawGood(G_FISH, 1, paintY);
      drawGood(G_MEAT, 2, paintY);
      drawGood(G_FRUIT, 3, paintY);
      drawGood(G_VEGETABLE, 4, paintY);
      _md->lbAbout->setHeight( 25 );
    }
    else
    {
      _md->lbAbout->setHeight( 50 );
    }

    paintY += 24;
    drawGood(G_POTTERY, 0, paintY);
    drawGood(G_FURNITURE, 1, paintY);
    drawGood(G_OIL, 2, paintY);
    drawGood(G_WINE, 3, paintY); 

    _md->lbAbout->setText( 0 == furageSum ? _("##market_search_food_source##") : _("##market_about##"));
  }
  else
  {
    _md->lbAbout->setHeight( 50 );
    _md->lbAbout->setText( _("##market_not_work##") );
  }

  drawWorkers();
}

void GuiInfoMarket::drawWorkers()
{
  int y = 136;
  GuiPaneling::instance().draw_black_frame(*_d->bgPicture, 16, y, getWidth() - 32, 62 );
  // picture of citizen
  Picture& pic = Picture::load( ResourceGroup::panelBackground, 542);
  _d->bgPicture->draw( pic, 16+15, y + 12);

  // number of workers
  std::string text = StringHelper::format( 0xff, _("%d employers (%d requires)"), 
                                                   _md->market->getWorkers(), 
                                                   _md->market->getMaxWorkers());

  _md->goodFont.draw(*_d->bgPicture, text, 16+42, y+18 );
}


void GuiInfoMarket::drawGood(const GoodType &goodType, int index, int paintY )
{
  int startOffset = 25;

  int offset = ( getWidth() - startOffset * 2 ) / 5;
  std::string goodName = GoodHelper::getName( goodType );

  // pictures of goods
  const Picture& pic = GoodHelper::getPicture( goodType );
  Point pos( index * offset + startOffset, paintY );
  _d->bgPicture->draw( pic, pos.getX(), pos.getY() );

  std::string outText = StringHelper::format( 0xff, "%d", _md->market->getGoodStore().getCurrentQty(goodType) );
  _md->goodFont.draw(*_d->bgPicture, outText, pos.getX() + 30, pos.getY() );
}

class GuiBuilding::Impl
{
public:
  BuildingPtr building;
};

GuiBuilding::GuiBuilding( Widget* parent, const Tile& tile )
    : GuiInfoBox( parent, Rect( 0, 0, 450, 220 ), -1 ), _bd( new Impl )
{
  _bd->building = tile.getTerrain().getOverlay().as<Building>();
  setTitle( BuildingDataHolder::instance().getData( _bd->building->getType()).getPrettyName() );

  paint();
}

void GuiBuilding::paint()
{
   int paintY = _d->lbTitle->getBottom() + 10;
   GuiPaneling::instance().draw_black_frame(*_d->bgPicture, 16, paintY, getWidth()-32, getHeight()-paintY-16);
   paintY+=10;  
}

InfoBoxLand::InfoBoxLand( Widget* parent, const Tile& tile )
    : GuiInfoBox( parent, Rect( 0, 0, 510, 350 ), -1 )
{
  _text = new Label( this, Rect( 38, 239, 470, 338 ), "", true );
  _text->setFont( Font::create( FONT_2 ) );
  _text->setWordWrap( true );

  if( tile.getTerrain().isTree() )
  {
    setTitle( _("##trees_and_forest_caption") );
    _text->setText( _("##trees_and_forest_text"));
  } 
  else if( tile.getTerrain().isWater() )
  {
    setTitle( _("##water_caption") );
    _text->setText( _("##water_text"));
  }
  else if( tile.getTerrain().isRock() )
  {
    setTitle( _("##rock_caption") );
    _text->setText( _("##rock_text"));
  }
  else if( tile.getTerrain().isRoad() )
  {
    if( tile.getTerrain().getOverlay()->getType() == B_PLAZA )
    {
      setTitle( _("##plaza_caption") );
      _text->setText( _("##plaza_text"));
    }
    else 
    {
     setTitle( _("##road_caption") );
      _text->setText( _("##road_text"));
    }
  }
  else 
  {
    setTitle( _("##clear_land_caption") );
    _text->setText( _("##clear_land_text"));
  }
 
  CityPtr oCity = Scenario::instance().getCity();
  Tilemap& oTilemap = oCity->getTilemap();
  int size = oTilemap.getSize();
  int border_size = (162 - size) / 2;
  
  int index = (size - tile.getJ() - 1 + border_size) * 162 + tile.getI() + border_size;
  
  const TerrainTile& terrain = tile.getTerrain();

  std::string text = StringHelper::format( 0xff, "Tile at: (%d,%d) %04X %02X %04X %02X %02X %02X",
                                           tile.getI(), tile.getJ(),  
                                          ((short int) terrain.getOriginalImgId() ),
                                          ((short int) terrain.getEdgeData()),
                                          ((short int) terrain.getTerrainData()),  
                                          ((short int) terrain.getTerrainRndmData()),  
                                          ((short int) terrain.getRandomData()),
                                          ((short int) terrain.getElevationData() ) );
  
  _text->setText( text );
  
  GuiPaneling::instance().draw_black_frame( *_d->bgPicture, 16, _d->lbTitle->getBottom() + 10, getWidth()-32, 180 );
}

void InfoBoxLand::setText( const std::string& text )
{
    _text->setText( text );
}

InfoBoxFreeHouse::InfoBoxFreeHouse( Widget* parent, const Tile& tile )
    : InfoBoxLand( parent, tile )
{
    setTitle( _("##freehouse_caption##") );

    if( tile.getTerrain().getOverlay().as<Construction>()->getAccessRoads().size() == 0 )
    {
      setText( _("##freehouse_text_noroad##") );
    }
    else
    {
      setText( _("##freehouse_text##") );
    }
}   

class InfoBoxRawMaterial::Impl
{
public:
  FactoryPtr rawmb;
  Label* lbProgress;
  Label* lbAbout;
  Label* lbDesc;
  Label* dmgLabel;

  void updateAboutText();
};

void InfoBoxRawMaterial::Impl::updateAboutText()
{
  std::string text = _("##farm_working_normally##");
  if( rawmb->getWorkers() == 0 )
  {
    text = _("##farm_have_no_workers##");
  }
  else if( rawmb->getWorkers() <= rawmb->getMaxWorkers() / 2 )
  {
    text = _("##farm_working_bad##");
  }

  lbAbout->setText( text );
}

InfoBoxRawMaterial::InfoBoxRawMaterial( Widget* parent, const Tile& tile )
: GuiInfoBox( parent, Rect( 0, 0, 510, 350 ), -1 ), _fd( new Impl )
{
  _fd->rawmb = tile.getTerrain().getOverlay().as<Factory>();
  
  GuiPaneling::instance().draw_black_frame( *_d->bgPicture, 16, 146, getWidth() - 32, 64 );

  // picture of citizen
  Picture& pic = Picture::load( ResourceGroup::panelBackground, 542);
  _d->bgPicture->draw( pic, 16+15, 160 );

  if( _fd->rawmb->getOutGoodType() != G_NONE )
  {
    const Picture &pic = GoodHelper::getPicture( _fd->rawmb->getOutGoodType() );
    _d->bgPicture->draw(pic, 10, 10);
  }

  // number of workers
  std::string text = StringHelper::format( 0xff, _("%d employers (%d requires)"), 
                                           _fd->rawmb->getWorkers(), _fd->rawmb->getMaxWorkers() );

  Font font = Font::create( FONT_2 );
  font.draw( *_d->bgPicture, text, 16+42, 158+5 );

  _fd->dmgLabel = new Label( this, Rect( 50, getHeight() - 50, getWidth() - 50, getHeight() - 16 ) ); 
  text = StringHelper::format( 0xff, "%d%% damage - %d%% fire", 
  (int)_fd->rawmb->getDamageLevel(), (int)_fd->rawmb->getFireLevel());
  _fd->dmgLabel->setText( text );

  text = StringHelper::format( 0xff, _("Production %d%% complete."), _fd->rawmb->getProgress() );
  _fd->lbProgress = new Label( this, Rect( 32, 50, getWidth() - 16, 50 + 32 ), text );
  _fd->lbAbout = new Label( this, Rect( 32, _fd->lbProgress->getBottom() + 6, getWidth() - 16, 130 ) );
  _fd->lbAbout->setWordWrap( true );

  std::string desc, name;
  GoodType goodType = G_NONE;
  switch( _fd->rawmb->getOutGoodType() )
  {
    case B_WHEAT_FARM:
      desc.assign( _("##farm_description_wheat##") );
      name.assign( _("##farm_title_wheat##") );
      goodType = G_WHEAT;
      break;
    case B_FRUIT:
      desc.assign( _("##farm_description_fruit##") );
      name.assign( _("##farm_title_fruit##") );
      goodType = G_FRUIT;
      break;
    case B_OLIVE:
      desc.assign( _("##farm_description_olive##") );
      name.assign( _("##farm_title_olive##") );
      goodType = G_OLIVE;
      break;
    case B_GRAPE:
      desc.assign( _("##farm_description_vine##") );
      name.assign( _("##farm_title_vine##") );
      goodType = G_GRAPE;
      break;
    case B_MEAT:
      desc.assign( _("##farm_description_meat##") );
      name.assign( _("##farm_title_meat##") );
      goodType = G_MEAT;
      break;
    case B_VEGETABLE:
      desc.assign( _("##farm_description_vegetable##") );
      name.assign( _("##farm_title_vegetable##") );
      goodType = G_VEGETABLE;
      break;
    default:
      break;
  }
  _fd->lbDesc = new Label( this, Rect( 32, 236, getWidth() - 50, getHeight() - 50 ), desc );
  _fd->lbDesc->setWordWrap( true );

  setTitle( name );

   // pictures of goods
  const Picture& goodIcon = GoodHelper::getPicture( goodType );
  _d->bgPicture->draw( goodIcon, 16, 16 );

  _fd->updateAboutText();
}

class InfoBoxBasic::Impl
{
public:
  Label* lbText;
};

InfoBoxBasic::InfoBoxBasic( Widget* parent, const Tile& tile )
: GuiInfoBox( parent, Rect( 0, 0, 510, 300 ), -1 ), _bd( new Impl )
{
  _bd->lbText = new Label( this, Rect( 32, 64, 510 - 32, 300 - 48 ) );
  _bd->lbText->setWordWrap( true );
}

void InfoBoxBasic::setText( const std::string& text )
{
  _bd->lbText->setText( text );
}
