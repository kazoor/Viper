#pragma once

/* Printable keys */
#define VIPER_KEY_SPACE              32
#define VIPER_KEY_APOSTROPHE         39  /* ' */
#define VIPER_KEY_COMMA              44  /* , */
#define VIPER_KEY_MINUS              45  /* - */
#define VIPER_KEY_PERIOD             46  /* . */
#define VIPER_KEY_SLASH              47  /* / */
#define VIPER_KEY_0                  48
#define VIPER_KEY_1                  49
#define VIPER_KEY_2                  50
#define VIPER_KEY_3                  51
#define VIPER_KEY_4                  52
#define VIPER_KEY_5                  53
#define VIPER_KEY_6                  54
#define VIPER_KEY_7                  55
#define VIPER_KEY_8                  56
#define VIPER_KEY_9                  57
#define VIPER_KEY_SEMICOLON          59  /* ; */
#define VIPER_KEY_EQUAL              61  /* = */
#define VIPER_KEY_A                  65
#define VIPER_KEY_B                  66
#define VIPER_KEY_C                  67
#define VIPER_KEY_D                  68
#define VIPER_KEY_E                  69
#define VIPER_KEY_F                  70
#define VIPER_KEY_G                  71
#define VIPER_KEY_H                  72
#define VIPER_KEY_I                  73
#define VIPER_KEY_J                  74
#define VIPER_KEY_K                  75
#define VIPER_KEY_L                  76
#define VIPER_KEY_M                  77
#define VIPER_KEY_N                  78
#define VIPER_KEY_O                  79
#define VIPER_KEY_P                  80
#define VIPER_KEY_Q                  81
#define VIPER_KEY_R                  82
#define VIPER_KEY_S                  83
#define VIPER_KEY_T                  84
#define VIPER_KEY_U                  85
#define VIPER_KEY_V                  86
#define VIPER_KEY_W                  87
#define VIPER_KEY_X                  88
#define VIPER_KEY_Y                  89
#define VIPER_KEY_Z                  90
#define VIPER_KEY_LEFT_BRACKET       91  /* [ */
#define VIPER_KEY_BACKSLASH          92  /* \ */
#define VIPER_KEY_RIGHT_BRACKET      93  /* ] */
#define VIPER_KEY_GRAVE_ACCENT       96  /* ` */
#define VIPER_KEY_WORLD_1            161 /* non-US #1 */
#define VIPER_KEY_WORLD_2            162 /* non-US #2 */

/* FunctVIPERkeys */
#define VIPER_KEY_ESCAPE             256
#define VIPER_KEY_ENTER              257
#define VIPER_KEY_TAB                258
#define VIPER_KEY_BACKSPACE          259
#define VIPER_KEY_INSERT             260
#define VIPER_KEY_DELETE             261
#define VIPER_KEY_RIGHT              262
#define VIPER_KEY_LEFT               263
#define VIPER_KEY_DOWN               264
#define VIPER_KEY_UP                 265
#define VIPER_KEY_PAGE_UP            266
#define VIPER_KEY_PAGE_DOWN          267
#define VIPER_KEY_HOME               268
#define VIPER_KEY_END                269
#define VIPER_KEY_CAPS_LOCK          280
#define VIPER_KEY_SCROLL_LOCK        281
#define VIPER_KEY_NUM_LOCK           282
#define VIPER_KEY_PRINT_SCREEN       283
#define VIPER_KEY_PAUSE              284
#define VIPER_KEY_F1                 290
#define VIPER_KEY_F2                 291
#define VIPER_KEY_F3                 292
#define VIPER_KEY_F4                 293
#define VIPER_KEY_F5                 294
#define VIPER_KEY_F6                 295
#define VIPER_KEY_F7                 296
#define VIPER_KEY_F8                 297
#define VIPER_KEY_F9                 298
#define VIPER_KEY_F10                299
#define VIPER_KEY_F11                300
#define VIPER_KEY_F12                301
#define VIPER_KEY_F13                302
#define VIPER_KEY_F14                303
#define VIPER_KEY_F15                304
#define VIPER_KEY_F16                305
#define VIPER_KEY_F17                306
#define VIPER_KEY_F18                307
#define VIPER_KEY_F19                308
#define VIPER_KEY_F20                309
#define VIPER_KEY_F21                310
#define VIPER_KEY_F22                311
#define VIPER_KEY_F23                312
#define VIPER_KEY_F24                313
#define VIPER_KEY_F25                314
#define VIPER_KEY_KP_0               320
#define VIPER_KEY_KP_1               321
#define VIPER_KEY_KP_2               322
#define VIPER_KEY_KP_3               323
#define VIPER_KEY_KP_4               324
#define VIPER_KEY_KP_5               325
#define VIPER_KEY_KP_6               326
#define VIPER_KEY_KP_7               327
#define VIPER_KEY_KP_8               328
#define VIPER_KEY_KP_9               329
#define VIPER_KEY_KP_DECIMAL         330
#define VIPER_KEY_KP_DIVIDE          331
#define VIPER_KEY_KP_MULTIPLY        332
#define VIPER_KEY_KP_SUBTRACT        333
#define VIPER_KEY_KP_ADD             334
#define VIPER_KEY_KP_ENTER           335
#define VIPER_KEY_KP_EQUAL           336
#define VIPER_KEY_LEFT_SHIFT         340
#define VIPER_KEY_LEFT_CONTROL       341
#define VIPER_KEY_LEFT_ALT           342
#define VIPER_KEY_LEFT_SUPER         343
#define VIPER_KEY_RIGHT_SHIFT        344
#define VIPER_KEY_RIGHT_CONTROL      345
#define VIPER_KEY_RIGHT_ALT          346
#define VIPER_KEY_RIGHT_SUPER        347
#define VIPER_KEY_MENU               348

enum Key {
    /* PRINTABLE KEYS */
    SPACE = VIPER_KEY_SPACE,
    APOSTROPHE = VIPER_KEY_APOSTROPHE, /* ' */
    COMMA = VIPER_KEY_COMMA, /* , */
    MINUS = VIPER_KEY_MINUS, /* - */
    PERIOD = VIPER_KEY_PERIOD, /* . */
    SLASH = VIPER_KEY_SLASH, /* / */
    SEMICOLON = VIPER_KEY_SEMICOLON,
    NUM_0 = VIPER_KEY_0,
    NUM_1 = VIPER_KEY_1,
    NUM_2 = VIPER_KEY_2,
    NUM_3 = VIPER_KEY_3,
    NUM_4 = VIPER_KEY_4,
    NUM_5 = VIPER_KEY_5,
    NUM_6 = VIPER_KEY_6,
    NUM_7 = VIPER_KEY_7,
    NUM_8 = VIPER_KEY_8,
    NUM_9 = VIPER_KEY_9,
    A = VIPER_KEY_A,
    B = VIPER_KEY_B,
    C = VIPER_KEY_C,
    D = VIPER_KEY_D,
    E = VIPER_KEY_E,
    F = VIPER_KEY_F,
    G = VIPER_KEY_G,
    H = VIPER_KEY_H,
    I = VIPER_KEY_I,
    J = VIPER_KEY_J,
    K = VIPER_KEY_K,
    L = VIPER_KEY_L,
    M = VIPER_KEY_M,
    N = VIPER_KEY_N,
    O = VIPER_KEY_O,
    P = VIPER_KEY_P,
    Q = VIPER_KEY_Q,
    R = VIPER_KEY_R,
    S = VIPER_KEY_S,
    T = VIPER_KEY_T,
    U = VIPER_KEY_U,
    V = VIPER_KEY_V,
    W = VIPER_KEY_W,
    X = VIPER_KEY_X,
    Y = VIPER_KEY_Y,
    Z = VIPER_KEY_Z,
    LEFT_BRACKET  = VIPER_KEY_LEFT_BRACKET, /* [ */
    RIGHT_BRACKET  = VIPER_KEY_RIGHT_BRACKET, /* ] */
    BACKSLASH  = VIPER_KEY_BACKSLASH, /* \ */
    ACCENT  = VIPER_KEY_GRAVE_ACCENT, /* ` */
    WORLD_1  = VIPER_KEY_WORLD_1, /* non-US #1 */
    WORLD_2  = VIPER_KEY_WORLD_2, /* non-US #2 */
    /* PRINTABLE KEYS */

    /* FUNCTION KEYS */
    ESCAPE = VIPER_KEY_ESCAPE,
    ENTER = VIPER_KEY_ENTER,
    TAB = VIPER_KEY_TAB,
    BACKSPACE = VIPER_KEY_BACKSPACE,
    INSERT = VIPER_KEY_INSERT,
    DELETE = VIPER_KEY_DELETE,
    RIGHT_ARROW = VIPER_KEY_RIGHT,
    LEFT_ARROW = VIPER_KEY_LEFT,
    DOWN_ARROW = VIPER_KEY_DOWN,
    UP_ARROW = VIPER_KEY_UP,
    PAGE_UP = VIPER_KEY_PAGE_UP,
    PAGE_DOWN = VIPER_KEY_PAGE_DOWN,
    HOME = VIPER_KEY_HOME,
    END = VIPER_KEY_END,
    CAPSLOCK = VIPER_KEY_CAPS_LOCK,
    SCROLL_LOCK = VIPER_KEY_SCROLL_LOCK,
    NUM_LOCK = VIPER_KEY_NUM_LOCK,
    PRINTSCREEN = VIPER_KEY_PRINT_SCREEN,
    PAUSE = VIPER_KEY_PAUSE,
    F1 = VIPER_KEY_F1,
    F2 = VIPER_KEY_F2,
    F3 = VIPER_KEY_F3,
    F4 = VIPER_KEY_F4,
    F5 = VIPER_KEY_F5,
    F6 = VIPER_KEY_F6,
    F7 = VIPER_KEY_F7,
    F8 = VIPER_KEY_F8,
    F9 = VIPER_KEY_F9,
    F10 = VIPER_KEY_F10,
    F11 = VIPER_KEY_F11,
    F12 = VIPER_KEY_F12,
    F13 = VIPER_KEY_F13,
    F14 = VIPER_KEY_F14,
    F15 = VIPER_KEY_F15,
    F16 = VIPER_KEY_F16,
    F17 = VIPER_KEY_F17,
    F18= VIPER_KEY_F18,
    F19 = VIPER_KEY_F19,
    F20 = VIPER_KEY_F20,
    F21 = VIPER_KEY_F21,
    F22 = VIPER_KEY_F22,
    F23 = VIPER_KEY_F23,
    F24 = VIPER_KEY_F24,
    F25 = VIPER_KEY_F25,
    F26 = VIPER_KEY_KP_0,
    KP_1 = VIPER_KEY_KP_1,
    KP_2 = VIPER_KEY_KP_2,
    KP_3 = VIPER_KEY_KP_3,
    KP_4 = VIPER_KEY_KP_4,
    KP_5 = VIPER_KEY_KP_5,
    KP_6 = VIPER_KEY_KP_6,
    KP_7 = VIPER_KEY_KP_7,
    KP_8 = VIPER_KEY_KP_8,
    KP_9 = VIPER_KEY_KP_9,
    KP_DECIMAL = VIPER_KEY_KP_DECIMAL,
    KP_DIVIDE = VIPER_KEY_KP_DIVIDE,
    KP_MULTIPLY = VIPER_KEY_KP_MULTIPLY,
    KP_SUBTRACT = VIPER_KEY_KP_SUBTRACT,
    KP_ADD = VIPER_KEY_KP_ADD,
    KP_ENTER = VIPER_KEY_KP_ENTER,
    KP_EQUAL = VIPER_KEY_KP_EQUAL,
    LSHIFT = VIPER_KEY_LEFT_SHIFT,
    LCTRL = VIPER_KEY_LEFT_CONTROL,
    LALT = VIPER_KEY_LEFT_ALT,
    LEFT_WINDOWS = VIPER_KEY_LEFT_SUPER,
    RSHIFT = VIPER_KEY_RIGHT_SHIFT,
    RCTRL = VIPER_KEY_RIGHT_CONTROL,
    RALT = VIPER_KEY_RIGHT_ALT,
    RIGHT_WINDOWS = VIPER_KEY_RIGHT_SUPER,
    MENU = VIPER_KEY_MENU
};