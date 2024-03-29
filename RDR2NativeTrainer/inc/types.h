/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

// 新增类型
typedef DWORD Volume;
typedef DWORD AnimScene;
typedef DWORD Prompt;
typedef DWORD ItemSet;
typedef DWORD PropSet;
typedef DWORD PopZone;
typedef DWORD PersChar;


#define ALIGN8 __declspec(align(8))

struct Vector3
{
	ALIGN8 float x;
	ALIGN8 float y;
	ALIGN8 float z;
};

static_assert(sizeof(Vector3) == 24, "");

enum eCrimeType : Hash
{
	CRIME_ACCOMPLICE = 0xAF074F6D,
	CRIME_ARSON = 0x68134DC7,
	CRIME_ASSAULT = 0x0BADC882,
	CRIME_ASSAULT_ANIMAL = 0x18DA55EE,
	CRIME_ASSAULT_CORPSE = 0x4E5F23F2,
	CRIME_ASSAULT_HORSE = 0xC4736181,
	CRIME_ASSAULT_LAW = 0xD7466D7C,
	CRIME_ASSAULT_LIVESTOCK = 0xCCE1CCBD,
	CRIME_BANK_ROBBERY = 0x6A1ADE3D,
	CRIME_BURGLARY = 0xA54C77E0,
	CRIME_CHEATING = 0xA2FF1145,
	CRIME_DISTURBANCE = 0x5011F613,
	CRIME_EXPLOSION = 0x3EBA7A37,
	CRIME_EXPLOSION_POISON = 0x91D0A0E1,
	CRIME_GRAVE_ROBBERY = 0x971EA5AF,
	CRIME_HASSLE = 0x58488776,
	CRIME_HIT_AND_RUN = 0xFF0A3CC4,
	CRIME_HIT_AND_RUN_LAW = 0x064814AF,
	CRIME_INTIMIDATION = 0x8319FBAB,
	CRIME_JACK_HORSE = 0x82F7E4A2,
	CRIME_JACK_VEHICLE = 0x6B981F4C,
	CRIME_JAIL_BREAK = 0x12C1D589,
	CRIME_KIDNAPPING = 0x98F908DB,
	CRIME_KIDNAPPING_LAW = 0xFD72A7EA,
	CRIME_LASSO_ASSAULT = 0x56EE5D5A,
	CRIME_LAW_IS_THREATENED = 0x1CB91DF0,
	CRIME_LOITERING = 0x6629D2F4,
	CRIME_LOOTING = 0x55AD2BEB,
	CRIME_MURDER = 0xE28ECE7E,
	CRIME_MURDER_ANIMAL = 0x48F59A66,
	CRIME_MURDER_HORSE = 0xC7261D79,
	CRIME_MURDER_LAW = 0x7797FCE7,
	CRIME_MURDER_LIVESTOCK = 0x9569C546,
	CRIME_MURDER_PLAYER = 0xF5ABD6C9,
	CRIME_MURDER_PLAYER_HORSE = 0xD55C6A79,
	CRIME_PROPERTY_DESTRUCTION = 0x533B003D,
	CRIME_RESIST_ARREST = 0xDF577BA5,
	CRIME_ROBBERY = 0xA3BEDE4C,
	CRIME_SELF_DEFENCE = 0xBD6A0AA3,
	CRIME_STAGECOACH_ROBBERY = 0xFC738E61,
	CRIME_STOLEN_GOODS = 0x9A949C79,
	CRIME_THEFT = 0x72ADE410,
	CRIME_THEFT_HORSE = 0xBE3A5838,
	CRIME_THEFT_LIVESTOCK = 0x85BA08FD,
	CRIME_THEFT_VEHICLE = 0x43A9ECA1,
	CRIME_THREATEN = 0x941C985A,
	CRIME_THREATEN_LAW = 0x7F908566,
	CRIME_TRAIN_ROBBERY = 0x647D2A5A,
	CRIME_TRAMPLE = 0x45DB39D8,
	CRIME_TRAMPLE_LAW = 0xF00F266B,
	CRIME_TRAMPLE_PLAYER = 0x75970C15,
	CRIME_TRESPASSING = 0xAEDE8E35,
	CRIME_UNARMED_ASSAULT = 0x5098CC5A,
	CRIME_VANDALISM = 0x80FDC759,
	CRIME_VANDALISM_VEHICLE = 0xF9E7ECE4,
	CRIME_VEHICLE_DESTRUCTION = 0x54A85DDC,
	CRIME_WANTED_LEVEL_UP_DEBUG_HIGH = 0x99C52FF5,
	CRIME_WANTED_LEVEL_UP_DEBUG_LOW = 0xD891890F
};

// attributeIndex 的值
enum ePedAttribute
{
	PA_HEALTH,    // 生命等级
	PA_STAMINA,    // 体力等级
	PA_SPECIALABILITY,    // 死神之眼等级
	PA_COURAGE,    // 勇气、胆量 
	PA_AGILITY,    // 敏捷 等级
	PA_SPEED,    // 速度 等级
	PA_ACCELERATION,    // 加速度
	PA_BONDING,    // 默契度
	SA_HUNGER,    // 饥饿值
	SA_FATIGUED,    // 疲劳值
	SA_INEBRIATED,    // 醉酒等级
	SA_POISONED,    // 中毒等级
	SA_BODYHEAT,    // 体温
	SA_BODYWEIGHT,    // 体重
	SA_OVERFED,
	SA_SICKNESS,    // 疾病
	SA_DIRTINESS,    // 肮脏
	SA_DIRTINESSHAT,    // 脏兮兮
	MTR_STRENGTH,    // 力量
	MTR_GRIT,
	MTR_INSTINCT,    // 直觉
	PA_UNRULINESS,    // 野性
	SA_DIRTINESSSKIN    // 脏皮
};

enum eExplosionTag
{
	EXP_TAG_DONTCARE = -1,	
	EXP_TAG_GRENADE,	// 手榴弹
	EXP_TAG_STICKYBOMB,	// 黏弹
	EXP_TAG_MOLOTOV,	
	EXP_TAG_MOLOTOV_VOLATILE,
	EXP_TAG_HI_OCTANE,
	EXP_TAG_CAR,		// 车
	EXP_TAG_PLANE,
	EXP_TAG_PETROL_PUMP,
	EXP_TAG_DIR_STEAM,
	EXP_TAG_DIR_FLAME,
	EXP_TAG_DIR_WATER_HYDRANT,
	EXP_TAG_BOAT,
	EXP_TAG_BULLET,		// 子弹
	EXP_TAG_SMOKEGRENADE,
	EXP_TAG_BZGAS,
	EXP_TAG_GAS_CANISTER,
	EXP_TAG_EXTINGUISHER,
	EXP_TAG_TRAIN,
	EXP_TAG_DIR_FLAME_EXPLODE,
	EXP_TAG_VEHICLE_BULLET,
	EXP_TAG_BIRD_CRAP,
	EXP_TAG_FIREWORK,
	EXP_TAG_TORPEDO,
	EXP_TAG_TORPEDO_UNDERWATER,
	EXP_TAG_LANTERN,
	EXP_TAG_DYNAMITE,
	EXP_TAG_DYNAMITESTACK,
	EXP_TAG_DYNAMITE_VOLATILE,
	EXP_TAG_RIVER_BLAST,
	EXP_TAG_PLACED_DYNAMITE,
	EXP_TAG_FIRE_ARROW,
	EXP_TAG_DYNAMITE_ARROW,
	EXP_TAG_PHOSPHOROUS_BULLET,
	EXP_TAG_LIGHTNING_STRIKE,
	EXP_TAG_TRACKING_ARROW,
	EXP_TAG_POISON_BOTTLE
};

enum eCombatAttribute
{
	CA_USE_COVER,
	CA_USE_VEHICLE,
	CA_DO_DRIVEBYS,
	CA_LEAVE_VEHICLES,
	CA_STRAFE_BASED_ON_TARGET_PROXIMITY,
	CA_ALWAYS_FIGHT,	// 一直处于战斗状态
	CA_0xEAD985F1,
	CA_0xB58A9DDA,
	CA_ALLOW_STRAFE_BREAKUP,
	CA_0xF30168B6,
	CA_0x793BF941,
	CA_0x1368A79D,
	CA_0xEFD9F9EC,
	CA_COVER_SEARCH_IN_ARC_AWAY_FROM_TARGET,
	CA_CAN_INVESTIGATE,
	CA_CAN_USE_RADIO,
	CA_STRAFE_DUE_TO_BULLET_EVENTS,
	CA_ALWAYS_FLEE,
	CA_0x934F1825,
	CA_0x70F392F0,
	CA_CAN_TAUNT_IN_VEHICLE,
	CA_CAN_CHASE_TARGET_ON_FOOT,
	CA_WILL_DRAG_INJURED_PEDS_TO_SAFETY,
	CA_0x42843828,
	CA_USE_PROXIMITY_FIRING_RATE,
	CA_DISABLE_SECONDARY_TARGET,
	CA_0xD5F88323,
	CA_PERFECT_ACCURACY,	// 精准射击
	CA_CAN_USE_FRUSTRATED_ADVANCE,
	CA_0xCF7FBC49,
	CA_0xA21AD34D,
	CA_MAINTAIN_MIN_DISTANCE_TO_TARGET,
	CA_0xBC6BB720,
	CA_0x8D3F251D,
	CA_ALLOW_PROJECTILE_SWAPS_AFTER_REPEATED_THROWS,
	CA_DISABLE_PINNED_DOWN,
	CA_0xF66F7D95,
	CA_0xF9C0215F,
	CA_DISABLE_BULLET_REACTIONS,
	CA_CAN_BUST,
	CA_0xA7F5BA74,
	CA_CAN_COMMANDEER_VEHICLES,
	CA_CAN_FLANK,
	CA_SWITCH_TO_ADVANCE_IF_CANT_FIND_COVER,
	CA_SWITCH_TO_DEFENSIVE_IF_IN_COVER,	// 自动找掩体
	CA_0xF8653124,
	CA_CAN_FIGHT_ARMED_PEDS_WHEN_NOT_ARMED,
	CA_0xE3F1FF4D,
	CA_0x704826E2,
	CA_USE_ENEMY_ACCURACY_SCALING,
	CA_CAN_CHARGE,
	CA_0xC3F453BC,
	CA_USE_VEHICLE_ATTACK,	// 车内射击
	CA_0x5BB58BF0,
	CA_ALWAYS_EQUIP_BEST_WEAPON,
	CA_CAN_SEE_UNDERWATER_PEDS,
	CA_0xE518D692,
	CA_0x59DAB2D0,
	CA_DISABLE_FLEE_FROM_COMBAT,
	CA_0xD60B6FE0,
	CA_CAN_THROW_SMOKE_GRENADE,
	CA_0x750246A2,
	CA_0x1A208E35,
	CA_0x8E9279C2,
	CA_DISABLE_BLOCK_FROM_PURSUE_DURING_VEHICLE_CHASE,
	CA_DISABLE_SPIN_OUT_DURING_VEHICLE_CHASE,
	CA_DISABLE_CRUISE_IN_FRONT_DURING_BLOCK_DURING_VEHICLE_CHASE,
	CA_0xA706084B,
	CA_DISABLE_REACT_TO_BUDDY_SHOT,
	CA_0x48AE3089,
	CA_0x9468CEDD,
	CA_PERMIT_CHARGE_BEYOND_DEFENSIVE_AREA,
	CA_0xD66FA13C,
	CA_0x2D0BC583,
	CA_0xD7948D4C,
	CA_0xB53C7137,
	CA_0x606C9D77,
	CA_DISABLE_RESPONDED_TO_THREAT_BROADCAST,
	CA_0x74E5F994,
	CA_0x2738AF09,
	CA_0x6A1DB0D9,
	CA_FORCE_STRAFE,
	CA_0xCA0D114D,
	CA_0x2060C16F,
	CA_0x98669E6C,
	CA_0x6E44A6F2,
	CA_0xC6A191DB,
	CA_0x57C8EF37,
	CA_0xA265A9FC,
	CA_0xE3FA8ABB,
	CA_0x9AA00F0F,
	CA_USE_RANGE_BASED_WEAPON_SELECTION,
	CA_0x8AF8D68D,
	CA_PREFER_MELEE,
	CA_0xDADFB2F2,
	CA_0xADB958A6,
	CA_0x64BBB208,
	CA_0x625F4C52,
	CA_0x945B1F0C,
	CA_0xC064FDFD,
	CA_0x834383BB,
	CA_0x5BF8CFD3,
	CA_0x93C824C4,
	CA_0xEE37D992,
	CA_0x007F7E21,
	CA_0xEDA38A25,
	CA_0x8C033B1B,
	CA_0x6430B807,
	CA_0xA96F6CE3,
	CA_0x4699FC85,
	CA_0x875B82F3,
	CA_0x1CB77C49,
	CA_0x8EB01547,
	CA_USE_INFINITE_CLIPS,
	CA_CAN_EXECUTE_TARGET,
	CA_DISABLE_RETREAT_DUE_TO_TARGET_PROXIMITY,
	CA_PREFER_DUAL_WIELD,
	CA_WILL_CUT_FREE_HOGTIED_PEDS,
	CA_TRY_TO_FORCE_SURRENDER,
	CA_0x0136E7B6,
	CA_0x797D7A1A,
	CA_0x97B4A6E4,
	CA_0x1FAAD7AF,
	CA_0x492B880F,
	CA_0xBE151581,
	CA_QUIT_WHEN_TARGET_FLEES_INTERACTION_FIGHT,
	CA_0xAC5E5497,
	CA_0xE300164C,
	CA_0xC82D4787,
	CA_0x31E0808F,
	CA_0x0A9A7130,
	CA_PREVENT_UNSAFE_PROJECTILE_THROWS,
	CA_0xA55AD510,
	CA_DISABLE_BLANK_SHOTS,
	CA_0xA78BB3BD
};

enum eFleeAttribute
{
	FA_FORCE_EXIT_VEHICLE = (1 << 16),
	FA_DISABLE_MOUNT_USAGE = (1 << 20),
	FA_DISABLE_ENTER_VEHICLES = (1 << 22),
};


enum componentHash
{

	boot_accessories = 0x18729F39,
	pants = 0x1D4C528A,
	cloaks = 0x3C1A74CD,
	hats = 0x9925C067,
	vests = 0x485EE834,
	chaps = 0x3107499B,
	shirts_full = 0x2026C46D,
	badges = 0x3F7F3587,
	masks = 0x7505EF42,
	spats = 0x514ADCEA,
	neckwear = 0x5FC29285,
	boots = 0x777EC6EF,
	accessories = 0x79D7DF96,
	jewelry_rings_right = 0x7A6BBD0B,
	jewelry_rings_left = 0xF16A1D23,
	jewelry_bracelets = 0x7BC10759,
	gauntlets = 0x91CE9B20,
	neckties = 0x7A96FACA,
	holsters_knife = 0x7BE77792,
	talisman_holster = 0x7C00A8F0,
	loadouts = 0x83887E88,
	suspenders = 0x877A2CF7,
	talisman_satchel = 0x8EFB276A,
	satchels = 0x94504D26,
	gunbelts = 0x9B2C8B89,
	belts = 0xA6D134C6,
	belt_buckles = 0xFAE9107F,
	holsters_left = 0xB6B6122D,
	holsters_right = 0xB9E2FA01,
	ammo_rifles = 0xDA0E2C55,
	talisman_wrist = 0xDB64A390,
	coats = 0xE06D30CE,
	coats_closed = 0x662AC34,
	ponchos = 0xAF14310B,
	armor = 0x72E6EF74,
	eyewear = 0x5E47CA6,
	gloves = 0xEABE0032,
	talisman_belt = 0x1AECF7DC,
	ammo_pistols = 0x3F1F01E5,
	holsters_crossdraw = 0x49C89D9B,
	aprons = 0x76F0E272,
	skirts = 0xA0E3AB7F,
	MASKS_LARGE = 0x4A73515C,
	heads = 0x378AD10C,
	hair = 0x864B03AE,
	BODIES_LOWER = 0x823687F5,
	teeth = 0x96EDAE5C,
	BODIES_UPPER = 0xB3966C9,
	eyes = 0xEA24B45E,
	beards_chin = 0x15D3C7F2,
	beards_chops = 0xB6B63737,
	beards_mustache = 0xECC8B25A,
	gunbelt_accs = 0xF1542D11,
	hair_accessories = 0x8E84A2AA,
	dresses = 0xA2926F9B,
	horse_saddles = 0xBAA7E618,
	horse_bedrolls = 0xEFB31921,
	horse_blankets = 0x17CEB41A,
	saddle_stirrups = 0xDA6DADCA,
	saddle_horns = 0x5447332,
	horse_manes = 0xAA0217AB,
	HORSE_SADDLEBAGS = 0x80451C25,
	HORSE_TAILS = 0xA63CAE10,
	horse_shoes = 0xFACFC3C0
};