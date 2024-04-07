#pragma once
// From glfw3.h
#define AR_KEY_SPACE              32
#define AR_KEY_APOSTROPHE         39  /* ' */
#define AR_KEY_COMMA              44  /* , */
#define AR_KEY_MINUS              45  /* - */
#define AR_KEY_PERIOD             46  /* . */
#define AR_KEY_SLASH              47  /* / */
#define AR_KEY_0                  48
#define AR_KEY_1                  49
#define AR_KEY_2                  50
#define AR_KEY_3                  51
#define AR_KEY_4                  52
#define AR_KEY_5                  53
#define AR_KEY_6                  54
#define AR_KEY_7                  55
#define AR_KEY_8                  56
#define AR_KEY_9                  57
#define AR_KEY_SEMICOLON          59  /* ; */
#define AR_KEY_EQUAL              61  /* = */
#define AR_KEY_A                  65
#define AR_KEY_B                  66
#define AR_KEY_C                  67
#define AR_KEY_D                  68
#define AR_KEY_E                  69
#define AR_KEY_F                  70
#define AR_KEY_G                  71
#define AR_KEY_H                  72
#define AR_KEY_I                  73
#define AR_KEY_J                  74
#define AR_KEY_K                  75
#define AR_KEY_L                  76
#define AR_KEY_M                  77
#define AR_KEY_N                  78
#define AR_KEY_O                  79
#define AR_KEY_P                  80
#define AR_KEY_Q                  81
#define AR_KEY_R                  82
#define AR_KEY_S                  83
#define AR_KEY_T                  84
#define AR_KEY_U                  85
#define AR_KEY_V                  86
#define AR_KEY_W                  87
#define AR_KEY_X                  88
#define AR_KEY_Y                  89
#define AR_KEY_Z                  90
#define AR_KEY_LEFT_BRACKET       91  /* [ */
#define AR_KEY_BACKSLASH          92  /* \ */
#define AR_KEY_RIGHT_BRACKET      93  /* ] */
#define AR_KEY_GRAVE_ACCENT       96  /* ` */
#define AR_KEY_WORLD_1            161 /* non-US #1 */
#define AR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AR_KEY_ESCAPE             256
#define AR_KEY_ENTER              257
#define AR_KEY_TAB                258
#define AR_KEY_BACKSPACE          259
#define AR_KEY_INSERT             260
#define AR_KEY_DELETE             261
#define AR_KEY_RIGHT              262
#define AR_KEY_LEFT               263
#define AR_KEY_DOWN               264
#define AR_KEY_UP                 265
#define AR_KEY_PAGE_UP            266
#define AR_KEY_PAGE_DOWN          267
#define AR_KEY_HOME               268
#define AR_KEY_END                269
#define AR_KEY_CAPS_LOCK          280
#define AR_KEY_SCROLL_LOCK        281
#define AR_KEY_NUM_LOCK           282
#define AR_KEY_PRINT_SCREEN       283
#define AR_KEY_PAUSE              284
#define AR_KEY_F1                 290
#define AR_KEY_F2                 291
#define AR_KEY_F3                 292
#define AR_KEY_F4                 293
#define AR_KEY_F5                 294
#define AR_KEY_F6                 295
#define AR_KEY_F7                 296
#define AR_KEY_F8                 297
#define AR_KEY_F9                 298
#define AR_KEY_F10                299
#define AR_KEY_F11                300
#define AR_KEY_F12                301
#define AR_KEY_F13                302
#define AR_KEY_F14                303
#define AR_KEY_F15                304
#define AR_KEY_F16                305
#define AR_KEY_F17                306
#define AR_KEY_F18                307
#define AR_KEY_F19                308
#define AR_KEY_F20                309
#define AR_KEY_F21                310
#define AR_KEY_F22                311
#define AR_KEY_F23                312
#define AR_KEY_F24                313
#define AR_KEY_F25                314
#define AR_KEY_KP_0               320
#define AR_KEY_KP_1               321
#define AR_KEY_KP_2               322
#define AR_KEY_KP_3               323
#define AR_KEY_KP_4               324
#define AR_KEY_KP_5               325
#define AR_KEY_KP_6               326
#define AR_KEY_KP_7               327
#define AR_KEY_KP_8               328
#define AR_KEY_KP_9               329
#define AR_KEY_KP_DECIMAL         330
#define AR_KEY_KP_DIVIDE          331
#define AR_KEY_KP_MULTIPLY        332
#define AR_KEY_KP_SUBTRACT        333
#define AR_KEY_KP_ADD             334
#define AR_KEY_KP_ENTER           335
#define AR_KEY_KP_EQUAL           336
#define AR_KEY_LEFT_SHIFT         340
#define AR_KEY_LEFT_CONTROL       341
#define AR_KEY_LEFT_ALT           342
#define AR_KEY_LEFT_SUPER         343
#define AR_KEY_RIGHT_SHIFT        344
#define AR_KEY_RIGHT_CONTROL      345
#define AR_KEY_RIGHT_ALT          346
#define AR_KEY_RIGHT_SUPER        347
#define AR_KEY_MENU               348

enum KeyCode
{
	AR_SPACE        =32,
	AR_APOSTROPHE   =39,
	AR_COMMA        =44,
	AR_MINUS        =45,
	AR_PERIOD       =46,
	AR_SLASH        =47,
	AR_0            =48,
	AR_1            =49,
	AR_2            =50,
	AR_3            =51,
	AR_4            =52,
	AR_5            =53,
	AR_6            =54,
	AR_7            =55,
	AR_8            =56,
	AR_9            =57,
	AR_SEMICOLON    =59,
	AR_EQUAL        =61,
	AR_A            =65,
	AR_B            =66,
	AR_C            =67,
	AR_D            =68,
	AR_E            =69,
	AR_F            =70,
	AR_G            =71,
	AR_H            =72,
	AR_I            =73,
	AR_J            =74,
	AR_K            =75,
	AR_L            =76,
	AR_M            =77,
	AR_N            =78,
	AR_O            =79,
	AR_P            =80,
	AR_Q            =81,
	AR_R            =82,
	AR_S            =83,
	AR_T            =84,
	AR_U            =85,
	AR_V            =86,
	AR_W            =87,
	AR_X            =88,
	AR_Y            =89,
	AR_Z            =90,
	AR_LEFT_BRACKET =91,
	AR_BACKSLASH    =92,
	AR_RIGHT_BRACKET=93,
	AR_GRAVE_ACCENT =96,
	AR_WORLD_1      =161,
	AR_WORLD_2      =162,

	//Function Key		  
	AR_ESCAPE         =256,
	AR_ENTER          =257,
	AR_TAB            =258,
	AR_BACKSPACE      =259,
	AR_INSERT         =260,
	AR_DELETE         =261,
	AR_RIGHT          =262,
	AR_LEFT           =263,
	AR_DOWN           =264,
	AR_UP             =265,
	AR_PAGE_UP        =266,
	AR_PAGE_DOWN      =267,
	AR_HOME           =268,
	AR_END            =269,
	AR_CAPS_LOCK      =280,
	AR_SCROLL_LOCK    =281,
	AR_NUM_LOCK       =282,
	AR_PRINT_SCREEN   =283,
	AR_PAUSE          =284,
	AR_F1             =290,
	AR_F2             =291,
	AR_F3             =292,
	AR_F4             =293,
	AR_F5             =294,
	AR_F6             =295,
	AR_F7             =296,
	AR_F8             =297,
	AR_F9             =298,
	AR_F10            =299,
	AR_F11            =300,
	AR_F12            =301,
	AR_F13            =302,
	AR_F14            =303,
	AR_F15            =304,
	AR_F16            =305,
	AR_F17            =306,
	AR_F18            =307,
	AR_F19            =308,
	AR_F20            =309,
	AR_F21            =310,
	AR_F22            =311,
	AR_F23            =312,
	AR_F24            =313,
	AR_F25            =314,
	AR_KP_0           =320,
	AR_KP_1           =321,
	AR_KP_2           =322,
	AR_KP_3           =323,
	AR_KP_4           =324,
	AR_KP_5           =325,
	AR_KP_6           =326,
	AR_KP_7           =327,
	AR_KP_8           =328,
	AR_KP_9           =329,
	AR_KP_DECIMAL     =330,
	AR_KP_DIVIDE      =331,
	AR_KP_MULTIPLY    =332,
	AR_KP_SUBTRACT    =333,
	AR_KP_ADD         =334,
	AR_KP_ENTER       =335,
	AR_KP_EQUAL       =336,
	AR_LEFT_SHIFT     =340,
	AR_LEFT_CONTROL   =341,
	AR_LEFT_ALT       =342,
	AR_LEFT_SUPER     =343,
	AR_RIGHT_SHIFT    =344,
	AR_RIGHT_CONTROL  =345,
	AR_RIGHT_ALT      =346,
	AR_RIGHT_SUPER    =347,
	AR_MENU           =348
};						  