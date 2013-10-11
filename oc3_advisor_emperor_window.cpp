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


#include "oc3_advisor_emperor_window.hpp"
#include "oc3_picture_decorator.hpp"
#include "oc3_gettext.hpp"
#include "oc3_pushbutton.hpp"
#include "oc3_gui_label.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_gfx_engine.hpp"
#include "oc3_gui_groupbox.hpp"
#include "oc3_listbox.hpp"
#include "oc3_listboxitem.hpp"

class AdvisorEmperorWindow::Impl
{
public:
  int money;
  int wantSend;
  PictureRef background;
  Label* lbEmperorFavour;
  Label* lbEmperorFavourDesc;
  Label* lbPost;
  Label* lbPrimaryFunds;  
  PushButton* btnSendGift;
  PushButton* btnSend2City;
  PushButton* btnChangeSalary; 

  void sendMoney()
  {
    money -= wantSend;
    onSendMoneySignal.emit( wantSend );
  }

public oc3_signals:
  Signal1<int> onChangeSalarySignal;
  Signal1<int> onSendMoneySignal;
};

void AdvisorEmperorWindow::_showChangeSalaryWindow()
{
  Rect wdgRect( Point( (getWidth() - 510) / 2, (getHeight() -400)/2), Size( 510, 400 ) );
  GroupBox* gb = new GroupBox( this, wdgRect, -1, GroupBox::whiteFrame );

  new Label( gb, Rect( 15, 15, gb->getWidth() - 15, 35), _("##set_mayor_salary##"),  false, Label::bgNone );
  ListBox* lbx = new ListBox( gb, Rect( 16, 50, gb->getWidth() - 16, gb->getHeight() - 100 ) );
  lbx->setItemHeight( 22 );
  lbx->setTextAlignment( alignCenter, alignCenter );
  lbx->setItemFont( Font::create( FONT_2_WHITE ) );

  ListBoxItem* item = &lbx->addItem( _("##citizen_salary##") ); item->setTag( 0 );
  item = &lbx->addItem( _("##clerk_salary##") ); item->setTag( 2 );
  item = &lbx->addItem( _("##engineer_salary##") ); item->setTag( 5 );
  item = &lbx->addItem( _("##architect_salary##") ); item->setTag( 8 );
  item = &lbx->addItem( _("##questor_salary##") ); item->setTag( 12 );
  item = &lbx->addItem( _("##procurate_salary##") ); item->setTag( 20 );
  item = &lbx->addItem( _("##edil_salary##") ); item->setTag( 30 );
  item = &lbx->addItem( _("##pretor_salary##") ); item->setTag( 40 );
  item = &lbx->addItem( _("##consoul_salary##") ); item->setTag( 60 );
  item = &lbx->addItem( _("##proconsoul_salary##") ); item->setTag( 80 );
  item = &lbx->addItem( _("##caesar_salary##") ); item->setTag( 100 );

  PushButton* btn = new PushButton( gb, Rect( Point( 176, gb->getHeight() - 32 ), Size( 160, 20) ), _("##cancel##"), -1, false, PushButton::whiteBorderUp );
  CONNECT( btn, onClicked(), gb, GroupBox::deleteLater );
}

void AdvisorEmperorWindow::_showSend2CityWindow()
{
  _d->wantSend = 0;
  GroupBox* gb = new GroupBox( this, Rect( Point( 50, 175 ), Size( 510, 160 )), -1, GroupBox::whiteFrame );

  Label* lbTitle = new Label( gb, Rect( 85, 15, gb->getWidth() - 85, 40), _("##send_money_to_city##"),  false, Label::bgNone );
  lbTitle->setTextAlignment( alignCenter, alignCenter );
  lbTitle->setFont( Font::create( FONT_3 ) );
  new Label( gb, Rect( Point( 50, 50), Size( 415, 60 ) ), "", false, Label::bgBlack );

  Point start( 65, 55 );
  Point offset( 80, 0 );
  Size btnSize( 65, 15 );
  new PushButton( gb, Rect( start, btnSize ), "0", 0x0f00, false, PushButton::blackBorderUp ); start += offset;
  new PushButton( gb, Rect( start, btnSize ), "500", 0x0f01, false,PushButton::blackBorderUp ); start += offset;
  new PushButton( gb, Rect( start, btnSize ), "2000", 0x0f04, false, PushButton::blackBorderUp ); start += offset;
  new PushButton( gb, Rect( start, btnSize ), "5000", 0x0f0a, false, PushButton::blackBorderUp ); start += offset;
  new PushButton( gb, Rect( start, btnSize ), _("##send_all##"), 0xf0ff, false,PushButton::blackBorderUp );

  Rect btnRect = Rect( 80, gb->getHeight() - 35, 80 + 160, gb->getHeight() - 15 );
  PushButton* btnSend = new PushButton( gb, btnRect, _("##send_money##"), -1, false, PushButton::whiteBorderUp );
  PushButton* btnCancel = new PushButton( gb, btnRect + Point( 190, 0), _("##cancel##"), -1, false, PushButton::whiteBorderUp );

  CONNECT( btnSend, onClicked(), _d.data(), Impl::sendMoney );
  CONNECT( btnSend, onClicked(), gb, GroupBox::deleteLater );
  CONNECT( btnCancel, onClicked(), gb, GroupBox::deleteLater );
}

bool AdvisorEmperorWindow::onEvent(const NEvent& event)
{
  if( event.EventType == OC3_GUI_EVENT  )
  {
    if( event.GuiEvent.EventType == OC3_LISTBOX_CHANGED )
    {
      if( ListBox* lstBox = safety_cast< ListBox* >( event.GuiEvent.Caller ) )
      {
        _d->onChangeSalarySignal.emit( lstBox->getSelectedItem().getTag() );
      }
    }
    else if( event.GuiEvent.EventType == OC3_BUTTON_CLICKED )
    {
      int id = event.GuiEvent.Caller->getID();
      if( id > 0 && ((id & 0x0f00) == 0x0f00) )
      {
        int multiplier = id & 0xff;
        _d->wantSend = math::clamp( (multiplier == 0xff ? _d->money : (multiplier * 500)), 0, _d->money);
      }
    }
  }

  return Widget::onEvent( event );
}

Signal1<int>&AdvisorEmperorWindow::onChangeSalary()
{
  return _d->onChangeSalarySignal;
}

Signal1<int>&AdvisorEmperorWindow::onSendMoney()
{
  return _d->onSendMoneySignal;
}

AdvisorEmperorWindow::AdvisorEmperorWindow(Widget* parent, int maxMoney, int id )
: Widget( parent, id, Rect( 0, 0, 1, 1 ) ), _d( new Impl )
{
  _d->money = maxMoney;
  setGeometry( Rect( Point( (parent->getWidth() - 640 )/2, parent->getHeight() / 2 - 242 ),
               Size( 640, 432 ) ) );

  Label* title = new Label( this, Rect( 10, 10, getWidth() - 10, 10 + 40) );
  title->setText( "Player name" );
  title->setFont( Font::create( FONT_3 ) );
  title->setTextAlignment( alignCenter, alignCenter );

  _d->background.reset( Picture::create( getSize() ) );
  //main _d->_d->background
  PictureDecorator::draw( *_d->background, Rect( Point( 0, 0 ), getSize() ), PictureDecorator::whiteFrame );

  //buttons _d->_d->background
  Point startPos( 32, 91 );
  PictureDecorator::draw( *_d->background, Rect( startPos, Size( 570, 220 ) ), PictureDecorator::blackFrame );
  PictureDecorator::draw( *_d->background, Rect( 66, 325, 66 + 510, 325 + 94 ), PictureDecorator::blackFrame );
  
  _d->lbEmperorFavour = new Label( this, Rect( Point( 58, 44 ), Size( 550, 20 ) ), "Favour of the emperor 50" );
  _d->lbEmperorFavourDesc = new Label( this, _d->lbEmperorFavour->getRelativeRect() + Point( 0, 20 ), "The emperor has mixed feelings to you" );

  _d->lbPost = new Label( this, Rect( Point( 70, 336 ), Size( 240, 26 ) ), "Post");
  _d->lbPrimaryFunds = new Label( this, Rect( Point( 70, 370 ), Size( 240, 20 ) ), "PrimaryFunds 0" );

  _d->btnSendGift = new PushButton( this, Rect( Point( 322, 343), Size( 250, 20 ) ), "Send gift", -1, false, PushButton::blackBorderUp );
  _d->btnSend2City = new PushButton( this, Rect( Point( 322, 370), Size( 250, 20 ) ), "Send to city", -1, false, PushButton::blackBorderUp );
  _d->btnChangeSalary = new PushButton( this, Rect( Point( 70, 395), Size( 500, 20 ) ), "Change salary", -1, false, PushButton::blackBorderUp );  
  CONNECT( _d->btnChangeSalary, onClicked(), this, AdvisorEmperorWindow::_showChangeSalaryWindow );
  CONNECT( _d->btnSend2City, onClicked(), this, AdvisorEmperorWindow::_showSend2CityWindow );
}

void AdvisorEmperorWindow::draw( GfxEngine& painter )
{
  if( !isVisible() )
    return;

  painter.drawPicture( *_d->background, getScreenLeft(), getScreenTop() );

  Widget::draw( painter );
}
