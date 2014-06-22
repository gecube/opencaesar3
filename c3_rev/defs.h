#ifndef __C3_REV_INCLUDE__
#define __C3_REV_INCLUDE__

#include "windows.h"
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

struct C3EngIndexEntry
{
int offset;
int inUse;
};

struct C3EngHeader
{
char tag[16];
int numGroups ;
int numStrings ;
int unknown    ;
};

enum EmpireCityType
{
City_DistantRoman  = 0,
City_Ours        = 1,
City_Trade       = 2,
City_FutureTrade  = 3,
City_DistantForeign  = 4,
City_VulnerableRoman  = 5,
City_FutureRoman  = 6
};

enum EnemyType
{
 Enemy_Barbarian  = 0,
 Enemy_Greek      = 8,
 Enemy_Egyptian   = 9
};


enum BuildingType
{
 B_none_building  = 0,
 B_Menu_Farms     = 2,
 B_Menu_RawMaterials  = 3,
 B_Menu_Workshops  = 4,
 B_Road           = 5,
 B_Wall           = 6,
 B_ReservoirSimon  = 7,
 B_Aqueduct       = 8,
 B_ClearLand      = 9,
 B_HouseVacantLot  = 10,
 B_HouseLargeTent  = 11,
 B_HouseSmallShack  = 12,
 B_HouseLargeShack  = 13,
 B_HouseSmallHovel  = 14,
 B_HouseLargeHovel  = 15,
 B_HouseSmallCasa  = 16,
 B_HouseLargeCasa  = 17,
 B_HouseSmallInsula  = 18,
 B_HouseMediumInsula  = 19,
 B_HouseLargeInsula  = 20,
 B_HouseGrandInsula  = 21,
 B_HouseSmallVilla  = 22,
 B_HouseMediumVilla  = 23,
 B_HouseLargeVilla  = 24,
 B_HouseGrandVilla  = 25,
 B_HouseSmallPalace  = 26,
 B_HouseMediumPalace  = 27,
 B_HouseLargePalace  = 28,
 B_HouseLuxuryPalace  = 29,
 B_Amphitheater   = 30,
 B_Theater        = 31,
 B_Hippodrome     = 32,
 B_Colosseum      = 33,
 B_GladiatorSchool  = 34,
 B_LionHouse      = 35,
 B_ActorColony    = 36,
 B_ChariotMaker   = 37,
 B_Plaza          = 38,
 B_Gardens        = 39,
 B_FortLegionaries  = 40,
 B_SmallStatue    = 41,
 B_MediumStatue   = 42,
 B_LargeStatue    = 43,
 B_FortJavelin    = 44,
 B_FortMounted    = 45,
 B_Doctor         = 46,
 B_Hospital       = 47,
 B_Bathhouse      = 48,
 B_Barber         = 49,
 B_DistributionCenter_Unused  = 50,
 B_School         = 51,
 B_Academy        = 52,
 B_Library        = 53,
 B_FortGround     = 54,
 B_Prefecture     = 55,
 B_TriumphalArch  = 56,
 B_FortGround__   = 57,
 B_Gatehouse      = 58,
 B_Tower          = 59,
 B_SmallTempleCeres  = 60,
 B_SmallTempleNeptune  = 61,
 B_SmallTempleMercury  = 62,
 B_SmallTempleMars  = 63,
 B_SmallTempleVenus  = 64,
 B_LargeTempleCeres  = 65,
 B_LargeTempleNeptune  = 66,
 B_LargeTempleMercury  = 67,
 B_LargeTempleMars  = 68,
 B_LargeTempleVenus  = 69,
 B_Market         = 70,
 B_Granary        = 71,
 B_Warehouse      = 72,
 B_WarehouseSpace  = 73,
 B_Shipyard       = 74,
 B_Dock           = 75,
 B_Wharf          = 76,
 B_GovernorsHouse  = 77,
 B_GovernorsVilla  = 78,
 B_GovernorsPalace  = 79,
 B_MissionPost    = 80,
 B_EngineersPost  = 81,
 B_LowBridge      = 82,
 B_ShipBridge     = 83,
 B_Senate         = 84,
 B_SenateUpgraded  = 85,
 B_Forum          = 86,
 B_ForumUpgraded  = 87,
 B_NativeHut      = 88,
 B_NativeMeeting  = 89,
 B_Reservoir      = 90,
 B_Fountain       = 91,
 B_Well           = 92,
 B_NativeCrops    = 93,
 B_MilitaryAcademy  = 94,
 B_Barracks       = 95,
 B_Menu_SmallTemples  = 96,
 B_Menu_LargeTemples  = 97,
 B_Oracle         = 98,
 B_BurningRuin    = 99,
 B_WheatFarm      = 100,
 B_VegetableFarm  = 101,
 B_FruitFarm      = 102,
 B_OliveFarm      = 103,
 B_VinesFarm      = 104,
 B_PigFarm        = 105,
 B_MarbleQuarry   = 106,
 B_IronMine       = 107,
 B_TimberYard     = 108,
 B_ClayPit        = 109,
 B_WineWorkshop   = 110,
 B_OilWorkshop    = 111,
 B_WeaponsWorkshop  = 112,
 B_FurnitureWorkshop  = 113,
 B_PotteryWorkshop  = 114,
 B_invasion_type = 126,
};

enum MessageType
{
  messageBildingLimitReach=65
};

struct C3CustomButton
{
  unsigned short xStart;
  unsigned short yStart;
  unsigned short xEnd;
  unsigned short yEnd;
  void (*mouseClickHandler)(C3CustomButton*, unsigned int);
  void (*rightClickHandler)(int, unsigned char);
  unsigned char field_10;
  unsigned char field_11;
  unsigned char field_12;
  unsigned int type;
  unsigned int parameter;
  unsigned int parameter2;
};

struct C3ArrowButton
{
  unsigned short xOffset;
  unsigned short yOffset;
  unsigned short graphicId;
  unsigned short size;
  void (*mouseClickHandler)();
  unsigned char field_C;
  unsigned char field_D;
  unsigned char field_E;
  unsigned char field_F;
  unsigned int parameter;
  unsigned int parameter2;
};

struct C3MenuItem
{
  unsigned short yOffset;
  unsigned short textNumber;//2
  void (*handler)();//4
  char field_9;//8
  char cycleMouseHelp;//9
  short field_8;//10
  int parameter;//12
  char field_10;//16
  char toggleWarnings;//17
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
  void (*mouseClickHandler)();
  unsigned int rightClickHandler;
  unsigned char state_enabled;
  unsigned char field_19;
  unsigned char field_1A;
  unsigned char field_1B;
  unsigned int parameter1;
  unsigned int parameter2;
};

#define __TM_GD 100

static C3GraphicOffsetID topmenuGraphics640[__TM_GD];
static C3GraphicOffsetID topmenuGraphics800[__TM_GD];
static C3GraphicOffsetID topmenuGraphics1024[__TM_GD];

struct C3GameFilePart
{
 int compressed;
 int offset;
 int length;
};

enum DirectionType
{
Direction_North  = 0,
Direction_NorthEast  = 1,
Direction_East   = 2,
Direction_SouthEast  = 3,
Direction_South  = 4,
Direction_SouthWest  = 5,
Direction_West   = 6,
Direction_NorthWest  = 7,
Direction_None   = 8
};


const int Emp_City = 2;
const int Emp_UnusedValue2 = 3;
const int Emp_BattleIcon = 4;
const int Emp_LandTradeRoute =5;
const int Emp_SeaTradeRoute = 6;
const int Emp_DistantBattleRomanArmy = 7;
const int Emp_DistantBattleEnemyArmy = 8;

enum ClimateType
{
Climate_Central  = 0,
Climate_Northern  = 1,
Climate_Desert   = 2
};


enum GoodType
{
G_None           = 0,
G_Wheat          = 1,
G_Vegetables     = 2,
G_Fruit          = 3,
G_Olives         = 4,
G_Vines          = 5,
G_MeatFish       = 6,
G_Wine           = 7,
G_Oil            = 8,
G_Iron           = 9,
G_Timber         = 10,
G_Clay           = 11,
G_Marble         = 12,
G_Weapons        = 13,
G_Furniture      = 14,
G_Pottery        = 15,
G_Denarii        = 16,
G_Troops         = 17
};


enum WindowType
{
W_MainMenu       = 0,
W_Advisors       = 7,
W_OverlaySelectionMenu  = 8,
W_DisplayOptionsDialog  = 10,
W_SoundOptionsDialog  = 11,
W_SpeedOptionsDialog  = 12,
W_SavedGame_LoadSaveDeleteDialog  = 13,
W_NumericInputDialog  = 14,
W_SelectListDialog  = 15,
W_MessageList    = 17,
W_Video          = 19,
W_EmpireMap      = 20,
W_EditorEditStartDate  = 22,
W_EditorRequests  = 23,
W_EditorEditRequest  = 24,
W_EditorInvasions  = 25,
W_EditorEditInvasion  = 26,
W_EditorAllowedBuildings  = 27,
W_EditorWinCriteria  = 28,
W_EditorSpecialEvents  = 29,
W_EditorDemandChanges  = 31,
W_EditorEditDemandChange  = 32,
W_EditorPriceChanges  = 33,
W_EditorEditPriceChange  = 34,
W_EditorStartingConditions  = 37,
W_ResourceSettingsDialog  = 40,
W_TradePricesDialog  = 41,
W_LaborPriorityDialog  = 42,
W_HoldFestivalDialog  = 43,
W_SetSalaryDialog  = 44,
W_DonateToCityDialog  = 45,
W_SendGiftDialog  = 46,
W_MissionBriefing  = 53,
W_NewCampaignDialog  = 54,
W_CckSelection   = 61,
W_ResolutionConfirmDialog  = 62,
W_GeneralConfirmDialog  = 63,
W_VideoIntro     = 64,
W_VideoCredits   = 65,
W_VideoLogo      = 68,
W_DifficultyOptionsDialog  = 69
};

enum GameTextGroup
{
  GAMETEXT_MENU_FILE  = 1,
  GAMETEXT_MENU_OPTIONS  = 2,
  GAMETEXT_MENU_HELP  = 3,
  GAMETEXT_MENU_ADVISORS  = 4,
  GAMETEXT_DIALOGS  = 5,
  GAMETEXT_MENUEXTRA  = 6,
  GAMETEXT_EDITOR_MENU_FILE  = 7,
  GAMETEXT_LOADING  = 11,
  GAMETEXT_ORIENTATION  = 17,
  GAMETEXT_WARNING  = 19,
  GAMETEXT_MONTHS  = 25,
  GAMETEXT_OVERLAY_INFO  = 66,
  GAMETEXT_MOUSEOVER_INFO  = 68

};

enum OverlayType
{
  Overlay_None=0,
  Overlay_Fire=8,
  Overlay_Damage=9,
  Overlay_Crime=0xA,
  Overlay_Problems=0x1D,
  Overlay_Desirability=0x1A,
  Overlay_Entertainment=11,
  Overlay_Education=16,
  Overlay_Theater=12,
  Overlay_FoodStocks=25,
  Overlay_Religion=4,
  Overlay_Native =0x1C,
  Overlay_Water=0x2,
  Overlay_Barber=20,
  Overlay_School=17,
  Overlay_Library=18,
  Overlay_Bathhouse=21,
  Overlay_TaxIncome=24,
  Overlay_Amphiheater=13,
  Overlay_Academy  = 19,
  Overlay_Hospital  = 23,
  Overlay_Colosseum=14,
  Overlay_Hippodrome=15,
  Overlay_Clinic=22,
  Overlay_Workers  = 27
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

enum HelpType
{
Help_Manual      = 0,
Help_About       = 1,
Help_Message     = 2,
Help_Mission     = 3
};

enum HelpMessageType
{
HelpMessage_General  = 0,
HelpMessage_Disaster  = 1,
HelpMessage_Imperial  = 2,
HelpMessage_Emigration  = 3,
HelpMessage_Tutorial  = 4,
HelpMessage_TradeChange  = 5,
HelpMessage_PriceChange  = 6,
HelpMessage_Invasion  = 7
};


enum MessageAdvisorType {
    MessageAdvisor_None  = 0,
    MessageAdvisor_Labor  = 1,
    MessageAdvisor_Trade  = 2,
    MessageAdvisor_Population  = 3,
    MessageAdvisor_Imperial  = 4,
    MessageAdvisor_Military  = 5,
    MessageAdvisor_Health  = 6,
    MessageAdvisor_Religion  = 7
};


enum FontType
{
 F_NormalPlain    = 0,
 F_NormalBlack    = 134,
 F_NormalWhite    = 268,
 F_NormalRed      = 402,
 F_LargePlain     = 536,
 F_LargeBlack     = 670,
 F_LargeBrown     = 804,
 F_SmallPlain     = 938,
 F_SmallBrown     = 1072,
 F_SmallBlack     = 1206
};

enum TradeStatusType {
  Trade_None=0,
  Trade_Import=1,
  Trade_Export=2
};

enum TerrainType {
  T_Tree           = 1,
 T_Rock           = 2,
 T_Water          = 4,
 T_Building       = 8,
 T_Scrub          = 0x10,
 T_Garden         = 0x20,
 T_Road           = 0x40,
 T_ReservoirRange  = 0x80,
 T_Aqueduct       = 0x100,
 T_Elevation      = 0x200,
 T_AccessRamp     = 0x400,
 T_Meadow         = 0x800,
 T_FountainRange  = 0x2000,
 T_Wall           = 0x4000,
 T_Gatehouse      = 0x8000

};

enum WalkerType
{
Walker_Immigrant  = 1,
Walker_Emigrant  = 2,
Walker_Homeless  = 3,
Walker_CartPusher  = 4,
Walker_LaborSeeker  = 5,
Walker_Explosion  = 6,
Walker_TaxCollector  = 7,
Walker_Engineer  = 8,
Walker_Warehouseman  = 9,
Walker_Prefect   = 10,
Walker_FortJavelin  = 11,
Walker_FortMounted  = 12,
Walker_FortLegionary  = 13,
Walker_FortStandard  = 14,
Walker_Actor     = 15,
Walker_Gladiator  = 16,
Walker_LionTamer  = 17,
Walker_Charioteer  = 18,
Walker_TradeCaravan  = 19,
Walker_TradeShip  = 20,
Walker_TradeCaravanDonkey  = 21,
Walker_Protestor  = 22,
Walker_Criminal  = 23,
Walker_Rioter    = 24,
Walker_FishingBoat  = 25,
Walker_MarketTrader  = 26,
Walker_Priest    = 27,
Walker_SchoolChild  = 28,
Walker_Teacher   = 29,
Walker_Librarian  = 30,
Walker_Barber    = 31,
Walker_BathhouseWorker  = 32,
Walker_Doctor    = 33,
Walker_Surgeon   = 34,
Walker_MapFlag   = 36,
Walker_Flotsam   = 37,
Walker_Dockman   = 38,
Walker_MarketBuyer  = 39,
Walker_Patrician  = 40,
Walker_IndigenousNative  = 41,
Walker_TowerSentry  = 42,
Walker_Enemy43   = 43,
Walker_Enemy44   = 44,
Walker_Enemy45   = 45,
Walker_Enemy46   = 46,
Walker_Enemy47   = 47,
Walker_Enemy48   = 48,
Walker_Enemy49   = 49,
Walker_Enemy50   = 50,
Walker_Enemy51   = 51,
Walker_Enemy52   = 52,
Walker_Enemy53   = 53,
Walker_Enemy54   = 54,
Walker_Enemy55   = 55,
Walker_Enemy56   = 56,
Walker_Enemy57   = 57,
Walker_NativeTrader  = 58,
Walker_Arrow     = 59,
Walker_Javelin   = 60,
Walker_Bolt      = 61,
Walker_Ballista  = 62,
Walker_Creature  = 63,
Walker_Missionary  = 64,
Walker_FishGulls  = 65,
Walker_DeliveryBoy  = 66,
Walker_Shipwreck  = 67,
Walker_Sheep     = 68,
Walker_Wolf      = 69,
Walker_Zebra     = 70,
Walker_Spear     = 71,
Walker_HippodromeMiniHorses  = 72
};

enum LaborCategory
{
LaborCategory_IndustryCommerce  = 0,
LaborCategory_FoodProduction  = 1,
LaborCategory_Engineering  = 2,
LaborCategory_Water  = 3,
LaborCategory_Prefectures  = 4,
LaborCategory_Military  = 5,
LaborCategory_Entertainment  = 6,
LaborCategory_HealthEducation  = 7,
LaborCategory_GovernanceReligion  = 8
};

#define _DWORD unsigned int
#define _BYTE unsigned char
#define _WORD unsigned short

enum GodType
{
God_Ceres        = 0,
God_Neptune      = 1,
God_Mercury      = 2,
God_Mars         = 3,
God_Venus        = 4
};

struct PKToken
{
char* inputData       ;
int field_4        ;
int inputLength    ;
char* compressedData  ;
int field_10       ;
int compressedLength;
char* destData       ;
int field_1C       ;
int destLength     ;
int field_24       ;
int hashcode        ;
int field_2C       ;
int stop            ;
};

struct PKCompBuffer
{
int copyOffset      ;
int outputPtr       ;
int currentOutputBitsUsed;
int windowSize     ;
int copyOffsetExtraMask;
int hasLiteralEncoding ;
int dictionarySize ;
char copyOffsetBits[64];
char copyOffsetCode[64];
char literalBits[256];
char copyLengthBits[518];
short literalValues[254];
short copyLengthValues[519];
short copyLengthEof ;
short field_9AE       ;
PKToken* token          ;
int (*inputFunc)(char*, int*, struct PKToken *);
void (*outputFunc)(char*, int*, struct PKToken *);
short field_9BC[518];
short analyze1[2304];
short field_1FC8     ;
char outputData[2050];
char inputData[8708];
char analyze2[8708];
};

struct PKDecompBuffer
{
int field_0        ;
int hasLiteralEncoding ;
int outputBufferPtr;
int windowSize     ;
int dictionarySize ;
int currentInputByte ;
int currentInput_bitsAvailable ;
int inputBufferPtr ;
int inputBufferEnd  ;
PKToken *token;
int (__cdecl *inputFunc)(char *, int *, struct PKToken *);
void (__cdecl *outputFunc)(char *, int *, struct PKToken *);
char outputBuffer[8708];//    db 8708 dup(?)
char inputBuffer[2048];//     db 2048 dup(?)
char copyOffsetTable[256];// db 256 dup(?)
char copyLengthTable[256];// db 256 dup(?)
char literalDecode1[256];//  db 256 dup(?)
char literalDecode2[256];//  db 256 dup(?)
char literalDecode3[128];//  db 128 dup(?)
char literalDecode4[256];//  db 256 dup(?)
char literalDecode5[256];//  db 256 dup(?)
char copyOffsetBits[64];//  db 64 dup(?)
char copyLengthBaseBits[16];// db 16 dup(?)
char copyLengthExtraBits[16];// db 16 dup(?)
char copyLengthBaseValue[32];// db 32 dup(?)
char field_3134[23715];//      db 23715 dup(?)
char end;
};


struct C3MapSize
{
int field_0;
int field_4;
int field_8;
int field_C;
short field_10;
short field_12;
short field_14;
short field_16;
};

static const int actionWalkerDie =-107;


struct C3Warning
{
short warningTextId;
short middle        ;
short top          ;
short inUse        ;
short textLength   ;
short warningboxWidth ;
int time           ;
short unused         ;
char text[102]        ;
};


struct WindowsBitmap
{
  BITMAPFILEHEADER header;
  char info[41];
};

#define _LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte
#define _LOWORD(x)   (*((_WORD*)&(x)))   // low word
#define _LODWORD(x)  (*((_DWORD*)&(x)))  // low dword
#define _HIBYTE(x)   (*((_BYTE*)&(x)+1))
#define _HIWORD(x)   (*((_WORD*)&(x)+1))
#define _HIDWORD(x)  (*((_DWORD*)&(x)+1))
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)
#endif


