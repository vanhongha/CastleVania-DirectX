#define SAFE_DELETE(p) {if(p) { delete(p); p = NULL;} } 

#define CLASS_NAME	"Castle Vania"
#define FULLSCREEN	false
#define GAME_TITLE	"Castle Vania"

#define GAME_WIDTH	640
#define GAME_HEIGHT	480
#define FRAME_RATE			30
#define FPS					60
#define MAX_TIME_PER_FRAME	1000 / FPS

#define GAME_FONT			"Resource\\font\\prstart.ttf"
#define TITLE_SCREEN		"Resource\\sprites\\TITLE_SCREEN.bmp"
#define TITLE_ANIMATION		"Resource\\sprites\\TitleAnimation.png"
#define SIMON_SPRITE		"Resource\\sprites\\Simon\\Simon_ver_editted.png"
#define WHIP_SPRITE			"Resource\\sprites\\Weapons\\WHIP.png"
#define CANDLE_SPRITE		"Resource\\sprites\\Ground\\1.png"
#define FIREBALL_SPRITE		"Resource\\sprites\\Enemies\\FIREBALL.png"
#define HOLY_WATER_SPRITE	"Resource\\sprites\\Sub_weapons\\HOLY_WATER.png"
#define DOOR_SPRITE			"Resource\\sprites\\Ground\\Door.png"
#define CASTLE_SPRITE		"Resource\\sprites\\Castle-lv1.png"
#define NULL_SPRITE			"Resource\\sprites\\Ground\\NULL.png"

#define DEAD_SPRITE				"Resource\\sprites\\Effect\\DEAD.png"
#define COLLIDE_EFFECT_SPRITE	"Resource\\sprites\\Effect\\0.png"
#define WATER_SPRITE			"Resource\\sprites\\Effect\\2.png"

/*ENEMY SPRITE*/
#define ZOMBIE_SPRITE		"Resource\\sprites\\Enemies\\ZOMBIE.png"
#define PANTHER_SPRITE		"Resource\\sprites\\Enemies\\PANTHER.png"
#define BATMAN_SPRITE		"Resource\\sprites\\Bosses\\VAMPIRE_BAT.png"
#define MEDUSA_SPRITE		"Resource\\sprites\\Bosses\\MEDUSA.png"
#define MERMAN_SPRITE		"Resource\\sprites\\Enemies\\MERMAN.png"
#define BAT_SPRITE			"Resource\\sprites\\Enemies\\BAT.png"
#define BLUE_BAT_SPRITE		"Resource\\sprites\\Enemies\\BLUE_BAT.png"
#define BONE_PILLAR_SPRITE		"Resource\\sprites\\Enemies\\BONE_PILLAR.png"
#define MEDUSA_HEAD_SPRITE			"Resource\\sprites\\Enemies\\MEDUSA_HEAD.png"
#define GHOST_SPRITE				"Resource\\sprites\\Enemies\\GHOST.png"
#define SURFACE_SPRITE		"Resource\\sprites\\Ground\\7.png"
#define ROPE_SPRITE			"Resource\\sprites\\Ground\\rope.png"
#define SNAKE_SPRITE		"Resource\\sprites\\Enemies\\SNAKE.png"

/*SUB WEAPON SPRITE*/
#define HOLY_WATER_SPRITE_ACTION		"Resource\\sprites\\Sub_weapons\\HOLY_WATER_ACTION.png"
#define CROSS_SPRITE_ACTION			"Resource\\sprites\\Sub_weapons\\CROSS_ACTION.png"
#define AXE_ACTION					"Resource\\sprites\\Sub_weapons\\AXE_ACTION.png"
#define KNIFE_ACTION				"Resource\\sprites\\Sub_weapons\\KNIFE_ACTION.png"
#define STOP_WATCH_ACTION			"Resource\\sprites\\Sub_weapons\\STOP_WATCH.png"

#define INTRO_SCENE_BG			"Resource\\sprites\\intro.s"
#define	INTRO_SCENE_MATRIX_BG	"Resource\\sprites\\intro.b"

#define LEVEL_ONE_BG		"Resource\\sprites\\lv1.s"
#define	LEVEL_ONE_MATRIX_BG	"Resource\\sprites\\lv1.b"

#define LEVEL_TWO_BG		"Resource\\sprites\\lv2.s"
#define	LEVEL_TWO_MATRIX_BG	"Resource\\sprites\\lv2.b"

#define LEVEL_THREE_BG		"Resource\\sprites\\lv3.s"
#define	LEVEL_THREE_MATRIX_BG	"Resource\\sprites\\lv3.b"

#define GROUND_TEST		"Resource\\sprites\\ground_test.png"

// Animations' status
#define STAND				0
#define MOVE				1
#define	SIT					2
#define JUMP				3
#define HIT_STAND			4
#define HIT_SIT				5
#define FALL_DOWN			6
#define UP_STAIR			7
#define DOWN_STAIR			8
#define HURTED				9
#define STANDING_UP			10
#define STANDING_DOWN		11
#define FIGHT				12
#define THROW_SUB_WEAPON	13
#define HIT_UP_STAIR		14
#define HIT_DOWN_STAIR		15
#define DEAD				16
#define INVISIBLE			17
#define EAT_ITEM			18
#define STAND_BACK			19

#define JUMP_FORCE		570
#define GRAVITY			800
#define SPEED			150
#define MAX_VELOCITY	200
#define EARTH_ACCELERATION -2000

#define SIDE_ABOVE		1.0f
#define SIDE_BOTTOM		-1.0f
#define SIDE_LEFT		-1.0f
#define SIDE_RIGHT		1.0f

#define STAIR_ON_TOP_DOWN_LEFT_TO_RIGHT		-10	// Dang o tren cau thang, di xuong ve phi ben phai
#define STAIR_ON_TOP_DOWN_RIGHT_TO_LEFT		-11	// Dang o tren cau thang, di xuong ve phi ben trai
#define STAIR_ON_BOTTOM_UP_LEFT_TO_RIGHT	-12	// Dang o duoi chan cau thang, di len ve phi ben phai
#define STAIR_ON_BOTTOM_UP_RIGHT_TO_LEFT	-13	// Dang o duoi chan cau thang, di len ve phi ben trai
#define STAIR_MIDLE							-14 // Dang o gia than cau thang
#define STAIR_NULL							-15	// Khong lien quan den cau thang

#define TAG_NULL				-100

#define TAG_GROUND_EDGE			-2
#define TAG_CHANGE_STATE		-1
#define TAG_WALL				0

#define TAG_ZOMBIE				1
#define TAG_PANTHER				2
#define TAG_MERMAN				3
#define TAG_PROJECTILE			-3
#define TAG_BAT					4
#define TAG_SPEAR_GUARD			5
#define TAG_MEDUSA_HEAD			6
#define TAG_GHOST				7
#define TAG_BONE_PILLAR			8

#define TAG_CANDLE				11
#define TAG_GROUND				12
#define TAG_GROUND_LEVEL_TWO	-12
#define TAG_STAIR				13
#define TAG_STAIR_				-13
#define TAG_STAIR_TOP			14
#define TAG_STAIR_TOP_			-14
#define TAG_CRUSHER				17
#define TAG_MOVEABLE_GROUND		19
#define TAG_DOOR				20
#define TAG_LARGE_CANDLE		10

#define TAG_CHECK_POINT		21
#define TAG_CHECK_STAIR		23

#define TAG_SIMON			31
#define TAG_HOLY_WATER		33
#define TAG_CROSS			34
#define TAG_WHIP			35
#define TAG_AXE				35
#define TAG_KNIFE			37

#define GAME_MENU_STATE			-1
#define GAME_INTRO_SCENE		0
#define GAME_LOSE_LAYER			4
#define	GAME_PLAY_STATE_ONE		1
#define GAME_PLAY_STATE_TWO		2
#define GAME_PLAY_STATE_THREE	3

// MOSTLY USED IN ITEM CLASS //////
#define ITEM_SMALL_HEART	1000
#define ITEM_BIG_HEART		1001
#define ITEM_ROAST			1002
#define ITEM_WHIP			1003
#define ITEM_AXE			1004
#define ITEM_CROSS			1005
#define ITEM_HOLY_WATER		1006
#define ITEM_KNIFE			1007
#define ITEM_STOP_WATCH		1008
#define ITEM_INVISIBLE		1009
#define ITEM_MONEY			1010
#define ITEM_BALL			1011
#define ITEM_UPGRADE_1		1012
#define ITEM_UPGRADE_2		1013
#define ITEM_ROSARY			1014