#ifndef __C3_REV_INCLUDE__
#define __C3_REV_INCLUDE__

#define __uint32_t unsigned int

struct C3MapPoint
{
  int x;
  int y;
};

struct C3GraphicOffsetID
{
  int xOffset;
  int graphicId;
};

typedef unsigned int BuildingType;

struct C3CustomButton
{
  unsigned short xStart;
  unsigned short yStart;
  unsigned short xEnd;
  unsigned short yEnd;
  unsigned int mouseClickHandler;
  unsigned int rightClickHangler;
  unsigned char field_10;
  unsigned char field_11;
  unsigned char field_12;
  unsigned int type;
  unsigned int parameter;
  unsigned int parameter2;
};

struct C3MenuItem
{
  unsigned short yOffset;
  unsigned short textNumber;
  int handler;
  int field_8;
  int parameter;
};

struct C3Menu
{
  unsigned short xOffset;
  unsigned short xEnd;
  unsigned short yOffset;
  unsigned short textGroup;
  C3MenuItem* items;
  unsigned int numItems;
};

struct C3Graphic
{
  unsigned int offset;
  unsigned int data_length;
  unsigned int uncompressed_length;
  unsigned int unused_0C;
  unsigned int invert_offset;
  unsigned short width;
  unsigned short height;
  unsigned int unused_18;
  unsigned short unused_1C;
  unsigned short unknown_1E;
  unsigned short unused_20;
  unsigned short sprite_top_offset;
  unsigned short sprite_left_offset;
  unsigned short animation[300];
  unsigned char type;
  unsigned char compr_flag;
  unsigned char extern_flag;
  unsigned char has_compressed_part;
  unsigned char unknown_36;
  unsigned char building_size;
  unsigned char bitmap_id;
  unsigned char unused_39;
  unsigned char unknown_3A;
  unsigned char unused_3B;
  unsigned char unknown_3C;
  unsigned char unknown_3D;
  unsigned short unused_3E;
};

struct C3ImageButton
{
  unsigned short xOffset;
  unsigned short yOffset;
  unsigned short width;
  unsigned short height;
  unsigned short field_8;
  unsigned short graphicCollectionId;
  unsigned short graphicIdOffsetInCollection;
  unsigned short field_E;
  unsigned int mouseClickHandler;
  unsigned int rightClickHangler;
  unsigned char state_enabled;
  unsigned char field_19;
  unsigned char field_1A;
  unsigned char field_1B;
  unsigned int parameter1;
  unsigned int parameter2;
};

#define __TM_GD 100

C3GraphicOffsetID topmenuGraphics640[__TM_GD];
C3GraphicOffsetID topmenuGraphics800[__TM_GD];
C3GraphicOffsetID topmenuGraphics1024[__TM_GD];

const int Direction_Nort = 0;
const int Direction_East = 2;
const int Direction_South = 4;
const int Direction_West = 6;

const int Emp_City = 2;
const int Emp_UnusedValue2 = 3;
const int Emp_BattleIcon = 4;
const int Emp_LandTradeRoute =5;
const int Emp_SeaTradeRoute = 6;
const int Emp_DistantBattleRomanArmy = 7;
const int Emp_DistantBattleEnemyArmy = 8;


const BuildingType B_HouseVacantLot = 0xA;
const BuildingType B_ClearLand = 0x9;
const BuildingType B_Road = 0x5;
const BuildingType B_ReservoirSimon = 0x7;
const BuildingType B_Aqueduct = 0x8;
const BuildingType B_Fountain = 0x5B;
const BuildingType B_Well = 0x5c;
const BuildingType B_Barber = 0x31;
const BuildingType B_Bathhouse = 0x30;
const BuildingType B_Doctor = 0x2e;
const BuildingType B_Hospital = 0x2f;
const BuildingType B_Menu_SmallTemples = 0x60;
const BuildingType B_Menu_LargeTemples = 0x61;
const BuildingType B_Oracle = 0x62;
const BuildingType B_School = 0x33;
const BuildingType B_Academy = 0x34;
const BuildingType B_Library = 0x35;
const BuildingType B_Theater = 0x1f;
const BuildingType B_Amphitheater = 0x1e;
const BuildingType B_Colosseum = 0x21;
const BuildingType B_Hippodrome = 0x20;
const BuildingType B_GladiatorSchool = 0x22;
const BuildingType B_LionHouse = 0x23;
const BuildingType B_ActorColony = 0x24;
const BuildingType B_ChariotMaker = 0x25;
const BuildingType B_Forum = 0x56;
const BuildingType B_SenateUpgraded = 0x55;
const BuildingType B_GovernorsHouse = 0x4d;
const BuildingType B_GovernorsVilla = 0x4e;
const BuildingType B_GovernorsPalace = 0x4f;
const BuildingType B_SmallStatue = 0x29;
const BuildingType B_MediumStatue = 0x2a;
const BuildingType B_LargeStatue = 0x2b;
const BuildingType B_Gardens = 0x27;
const BuildingType B_Plaza = 0x26;
const BuildingType B_EngineersPost = 0x51;
const BuildingType B_MissionPost = 0x50;
const BuildingType B_Shipyard  = 0x4a;
const BuildingType B_Dock = 0x4b;
const BuildingType B_Wharf = 0x4c;
const BuildingType B_Wall = 0x6;
const BuildingType B_Tower = 0x3b;
const BuildingType B_Gatehouse = 0x3a;
const BuildingType B_Prefecture  = 0x37;
const BuildingType B_FortGround__= 0x39;
const BuildingType B_MilitaryAcademy = 0x5e;
const BuildingType B_Barracks  = 0x5f;
const BuildingType B_DistributionCenter_Unused = 0x32;
const BuildingType B_Menu_Farms  = 0x2;
const BuildingType B_Menu_RawMaterials  = 0x3;
const BuildingType B_Menu_Workshops  = 0x4;
const BuildingType B_Market = 0x46;
const BuildingType B_Granary = 0x47;
const BuildingType B_Warehouse = 0x48;
const BuildingType B_LowBridge = 0x52;
const BuildingType B_Reservoir = 0x5a;
const BuildingType B_ShipBridge = 0x53;
const BuildingType B_TriumphalArch = 0x38;
const BuildingType B_HouseLuxuryPalace = 0x1D;
const BuildingType B_WheatFarm = 0x64;
const BuildingType B_VegetableFarm = 0x65;
const BuildingType B_FruitFarm = 0x66;
const BuildingType B_OliveFarm = 0x67;
const BuildingType B_VinesFarm = 0x68;
const BuildingType B_PigFarm = 0x69;
const BuildingType B_ClayPit = 0x6d;
const BuildingType B_MarbleQuarry = 0x6a;
const BuildingType B_IronMine = 0x6b;
const BuildingType B_TimberYard = 0x6c;
const BuildingType B_WineWorkshop  = 0x6e;
const BuildingType B_OilWorkshop = 0x6f;
const BuildingType B_WeaponsWorkshop = 0x70;
const BuildingType B_FurnitureWorkshop = 0x71;
const BuildingType B_PotteryWorkshop = 0x72;
const BuildingType B_NativeHut = 0x58;
const BuildingType B_NativeMeeting = 0x59;
const BuildingType B_NativeCrops = 0x5D;
typedef unsigned int GoodType;

const GoodType G_Wheat = 1;
const GoodType G_Vegetables = 2;
const GoodType G_Fruit = 3;
const GoodType G_Olives = 4;
const GoodType G_Vines = 5;
const GoodType G_MeatFish = 6;
const GoodType G_Clay = 0xb;
const GoodType G_Marble = 0xc;
const GoodType G_Timber = 0xA;
const GoodType G_Wine = 0x7;
const GoodType G_Oil = 0x8;
const GoodType G_Weapons = 0xD;
const GoodType G_Furniture = 0xe;
const GoodType G_Pottery = 0xf;
const GoodType G_Iron  = 0x9;

const int Overlay_None = 0;
const int W_MainMenu = 0;
const int W_SavedGame_LoadSaveDeleteDialog = 0xD;
const int W_DisplayOptionsDialog = 0xA;
const int W_SoundOptionsDialog = 0xB;
const int W_SpeedOptionsDialog = 0xC;
const int W_NumericInputDialog = 0xE;
const int W_SelectListDialog = 0xF;
const int W_Advisors = 0x7;
const int W_OverlaySelectionMenu = 0x8;
const int W_EmpireMap = 0x14;
const int W_EditorEditStartDate = 0x16;
const int W_EditorRequests = 0x17;
const int W_EditorEditRequest = 0x18;
const int W_EditorInvasions = 0x19;
const int W_EditorEditInvasion = 0x1A;
const int W_EditorAllowedBuildings = 0x1B;
const int W_EditorWinCriteria = 0x1C;
const int W_EditorSpecialEvents = 0x1D;
const int W_EditorDemandChanges = 0x1f;
const int W_EditorEditDemandChange = 0x20;
const int W_EditorPriceChanges = 0x21;
const int W_EditorEditPriceChange = 0x22;
const int W_EditorStartingConditions = 0x25;
const int W_ResourceSettingsDialog = 0x28;
const int W_TradePricesDialog = 0x29;
const int W_LaborPriorityDialog = 0x2A;
const int W_HoldFestivalDialog = 0x2B;
const int W_SetSalaryDialog = 0x2C;
const int W_DonateToCityDialog = 0x2D;
const int W_SendGiftDialog = 0x2e;
const int W_MissionBriefing = 0x35;
const int W_ResolutionConfirmDialog = 0x3E;
const int W_DifficultyOptionsDialog = 0x45;

enum GameTextGroup
{
  GAMETEXT_DIALOGS=5,
  GAMETEXT_LOADING=0xB
};

enum OverlayType
{
  Overlay_Fire=8,
  Overlay_Damage=9,
  Overlay_Crime=0xA,
  Overlay_Problems=0x1D,
  Overlay_Water=0x2
};

enum AdvisorType {
  Adv_Labor=1,
  Adv_Military=2,
  Adv_Imperial=3,
  Adv_Ratings=4,
  Adv_Trade=5,
  Adv_Population=6,
  Adv_Health=7,
  Adv_Education=8,
  Adv_Entertainment=9,
  Adv_Religion=0xA,
  Adv_Financial=0xB,
  Adv_Chief=0xC
};

enum FontType
{
  F_SmallBrown=0x430
};

enum TradeStatusType {
  Trade_None=0,
  Trade_Import=1,
  Trade_Export=2
};


#endif


