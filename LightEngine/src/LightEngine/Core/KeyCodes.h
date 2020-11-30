#pragma once

namespace LightEngine
{
    typedef enum class KeyCode : uint16_t
    {
        // From glfw3.h
        Space               = 32,
        Apostrophe          = 39, /* ' */
        Comma               = 44, /* , */
        Minus               = 45, /* - */
        Period              = 46, /* . */
        Slash               = 47, /* / */

        D0                  = 48, /* 0 */
        D1                  = 49, /* 1 */
        D2                  = 50, /* 2 */
        D3                  = 51, /* 3 */
        D4                  = 52, /* 4 */
        D5                  = 53, /* 5 */
        D6                  = 54, /* 6 */
        D7                  = 55, /* 7 */
        D8                  = 56, /* 8 */
        D9                  = 57, /* 9 */

        Semicolon           = 59, /* ; */
        Equal               = 61, /* = */

        A                   = 65,
        B                   = 66,
        C                   = 67,
        D                   = 68,
        E                   = 69,
        F                   = 70,
        G                   = 71,
        H                   = 72,
        I                   = 73,
        J                   = 74,
        K                   = 75,
        L                   = 76,
        M                   = 77,
        N                   = 78,
        O                   = 79,
        P                   = 80,
        Q                   = 81,
        R                   = 82,
        S                   = 83,
        T                   = 84,
        U                   = 85,
        V                   = 86,
        W                   = 87,
        X                   = 88,
        Y                   = 89,
        Z                   = 90,

        LeftBracket         = 91,  /* [ */
        Backslash           = 92,  /* \ */
        RightBracket        = 93,  /* ] */
        GraveAccent         = 96,  /* ` */

        World1              = 161, /* non-US #1 */
        World2              = 162, /* non-US #2 */

        /* Function keys */
        Escape              = 256,
        Enter               = 257,
        Tab                 = 258,
        Backspace           = 259,
        Insert              = 260,
        Delete              = 261,
        Right               = 262,
        Left                = 263,
        Down                = 264,
        Up                  = 265,
        PageUp              = 266,
        PageDown            = 267,
        Home                = 268,
        End                 = 269,
        CapsLock            = 280,
        ScrollLock          = 281,
        NumLock             = 282,
        PrintScreen         = 283,
        Pause               = 284,
        F1                  = 290,
        F2                  = 291,
        F3                  = 292,
        F4                  = 293,
        F5                  = 294,
        F6                  = 295,
        F7                  = 296,
        F8                  = 297,
        F9                  = 298,
        F10                 = 299,
        F11                 = 300,
        F12                 = 301,
        F13                 = 302,
        F14                 = 303,
        F15                 = 304,
        F16                 = 305,
        F17                 = 306,
        F18                 = 307,
        F19                 = 308,
        F20                 = 309,
        F21                 = 310,
        F22                 = 311,
        F23                 = 312,
        F24                 = 313,
        F25                 = 314,

        /* Keypad */
        KP0                 = 320,
        KP1                 = 321,
        KP2                 = 322,
        KP3                 = 323,
        KP4                 = 324,
        KP5                 = 325,
        KP6                 = 326,
        KP7                 = 327,
        KP8                 = 328,
        KP9                 = 329,
        KPDecimal           = 330,
        KPDivide            = 331,
        KPMultiply          = 332,
        KPSubtract          = 333,
        KPAdd               = 334,
        KPEnter             = 335,
        KPEqual             = 336,

        LeftShift           = 340,
        LeftControl         = 341,
        LeftAlt             = 342,
        LeftSuper           = 343,
        RightShift          = 344,
        RightControl        = 345,
        RightAlt            = 346,
        RightSuper          = 347,
        Menu                = 348
    }Key;

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<int32_t>(keyCode);
        return os;
    }
}

#define LE_KEY_SPACE           ::LightEngine::Key::Space
#define LE_KEY_APOSTROPHE      ::LightEngine::Key::Apostrophe    /* ' */
#define LE_KEY_COMMA           ::LightEngine::Key::Comma         /* , */
#define LE_KEY_MINUS           ::LightEngine::Key::Minus         /* - */
#define LE_KEY_PERIOD          ::LightEngine::Key::Period        /* . */
#define LE_KEY_SLASH           ::LightEngine::Key::Slash         /* / */
#define LE_KEY_0               ::LightEngine::Key::D0
#define LE_KEY_1               ::LightEngine::Key::D1
#define LE_KEY_2               ::LightEngine::Key::D2
#define LE_KEY_3               ::LightEngine::Key::D3
#define LE_KEY_4               ::LightEngine::Key::D4
#define LE_KEY_5               ::LightEngine::Key::D5
#define LE_KEY_6               ::LightEngine::Key::D6
#define LE_KEY_7               ::LightEngine::Key::D7
#define LE_KEY_8               ::LightEngine::Key::D8
#define LE_KEY_9               ::LightEngine::Key::D9
#define LE_KEY_SEMICOLON       ::LightEngine::Key::Semicolon     /* ; */
#define LE_KEY_EQUAL           ::LightEngine::Key::Equal         /* = */
#define LE_KEY_A               ::LightEngine::Key::A
#define LE_KEY_B               ::LightEngine::Key::B
#define LE_KEY_C               ::LightEngine::Key::C
#define LE_KEY_D               ::LightEngine::Key::D
#define LE_KEY_E               ::LightEngine::Key::E
#define LE_KEY_F               ::LightEngine::Key::F
#define LE_KEY_G               ::LightEngine::Key::G
#define LE_KEY_H               ::LightEngine::Key::H
#define LE_KEY_I               ::LightEngine::Key::I
#define LE_KEY_J               ::LightEngine::Key::J
#define LE_KEY_K               ::LightEngine::Key::K
#define LE_KEY_L               ::LightEngine::Key::L
#define LE_KEY_M               ::LightEngine::Key::M
#define LE_KEY_N               ::LightEngine::Key::N
#define LE_KEY_O               ::LightEngine::Key::O
#define LE_KEY_P               ::LightEngine::Key::P
#define LE_KEY_Q               ::LightEngine::Key::Q
#define LE_KEY_R               ::LightEngine::Key::R
#define LE_KEY_S               ::LightEngine::Key::S
#define LE_KEY_T               ::LightEngine::Key::T
#define LE_KEY_U               ::LightEngine::Key::U
#define LE_KEY_V               ::LightEngine::Key::V
#define LE_KEY_W               ::LightEngine::Key::W
#define LE_KEY_X               ::LightEngine::Key::X
#define LE_KEY_Y               ::LightEngine::Key::Y
#define LE_KEY_Z               ::LightEngine::Key::Z
#define LE_KEY_LEFT_BRACKET    ::LightEngine::Key::LeftBracket   /* [ */
#define LE_KEY_BACKSLASH       ::LightEngine::Key::Backslash     /* \ */
#define LE_KEY_RIGHT_BRACKET   ::LightEngine::Key::RightBracket  /* ] */
#define LE_KEY_GRAVE_ACCENT    ::LightEngine::Key::GraveAccent   /* ` */
#define LE_KEY_WORLD_1         ::LightEngine::Key::World1        /* non-US #1 */
#define LE_KEY_WORLD_2         ::LightEngine::Key::World2        /* non-US #2 */

#define LE_KEY_ESCAPE          ::LightEngine::Key::Escape
#define LE_KEY_ENTER           ::LightEngine::Key::Enter
#define LE_KEY_TAB             ::LightEngine::Key::Tab
#define LE_KEY_BACKSPACE       ::LightEngine::Key::Backspace
#define LE_KEY_INSERT          ::LightEngine::Key::Insert
#define LE_KEY_DELETE          ::LightEngine::Key::Delete
#define LE_KEY_RIGHT           ::LightEngine::Key::Right
#define LE_KEY_LEFT            ::LightEngine::Key::Left
#define LE_KEY_DOWN            ::LightEngine::Key::Down
#define LE_KEY_UP              ::LightEngine::Key::Up
#define LE_KEY_PAGE_UP         ::LightEngine::Key::PageUp
#define LE_KEY_PAGE_DOWN       ::LightEngine::Key::PageDown
#define LE_KEY_HOME            ::LightEngine::Key::Home
#define LE_KEY_END             ::LightEngine::Key::End
#define LE_KEY_CAPS_LOCK       ::LightEngine::Key::CapsLock
#define LE_KEY_SCROLL_LOCK     ::LightEngine::Key::ScrollLock
#define LE_KEY_NUM_LOCK        ::LightEngine::Key::NumLock
#define LE_KEY_PRINT_SCREEN    ::LightEngine::Key::PrintScreen
#define LE_KEY_PAUSE           ::LightEngine::Key::Pause
#define LE_KEY_F1              ::LightEngine::Key::F1
#define LE_KEY_F2              ::LightEngine::Key::F2
#define LE_KEY_F3              ::LightEngine::Key::F3
#define LE_KEY_F4              ::LightEngine::Key::F4
#define LE_KEY_F5              ::LightEngine::Key::F5
#define LE_KEY_F6              ::LightEngine::Key::F6
#define LE_KEY_F7              ::LightEngine::Key::F7
#define LE_KEY_F8              ::LightEngine::Key::F8
#define LE_KEY_F9              ::LightEngine::Key::F9
#define LE_KEY_F10             ::LightEngine::Key::F10
#define LE_KEY_F11             ::LightEngine::Key::F11
#define LE_KEY_F12             ::LightEngine::Key::F12
#define LE_KEY_F13             ::LightEngine::Key::F13
#define LE_KEY_F14             ::LightEngine::Key::F14
#define LE_KEY_F15             ::LightEngine::Key::F15
#define LE_KEY_F16             ::LightEngine::Key::F16
#define LE_KEY_F17             ::LightEngine::Key::F17
#define LE_KEY_F18             ::LightEngine::Key::F18
#define LE_KEY_F19             ::LightEngine::Key::F19
#define LE_KEY_F20             ::LightEngine::Key::F20
#define LE_KEY_F21             ::LightEngine::Key::F21
#define LE_KEY_F22             ::LightEngine::Key::F22
#define LE_KEY_F23             ::LightEngine::Key::F23
#define LE_KEY_F24             ::LightEngine::Key::F24
#define LE_KEY_F25             ::LightEngine::Key::F25

/* Keypad */
#define LE_KEY_KP_0            ::LightEngine::Key::KP0
#define LE_KEY_KP_1            ::LightEngine::Key::KP1
#define LE_KEY_KP_2            ::LightEngine::Key::KP2
#define LE_KEY_KP_3            ::LightEngine::Key::KP3
#define LE_KEY_KP_4            ::LightEngine::Key::KP4
#define LE_KEY_KP_5            ::LightEngine::Key::KP5
#define LE_KEY_KP_6            ::LightEngine::Key::KP6
#define LE_KEY_KP_7            ::LightEngine::Key::KP7
#define LE_KEY_KP_8            ::LightEngine::Key::KP8
#define LE_KEY_KP_9            ::LightEngine::Key::KP9
#define LE_KEY_KP_DECIMAL      ::LightEngine::Key::KPDecimal
#define LE_KEY_KP_DIVIDE       ::LightEngine::Key::KPDivide
#define LE_KEY_KP_MULTIPLY     ::LightEngine::Key::KPMultiply
#define LE_KEY_KP_SUBTRACT     ::LightEngine::Key::KPSubtract
#define LE_KEY_KP_ADD          ::LightEngine::Key::KPAdd
#define LE_KEY_KP_ENTER        ::LightEngine::Key::KPEnter
#define LE_KEY_KP_EQUAL        ::LightEngine::Key::KPEqual

#define LE_KEY_LEFT_SHIFT      ::LightEngine::Key::LeftShift
#define LE_KEY_LEFT_CONTROL    ::LightEngine::Key::LeftControl
#define LE_KEY_LEFT_ALT        ::LightEngine::Key::LeftAlt
#define LE_KEY_LEFT_SUPER      ::LightEngine::Key::LeftSuper
#define LE_KEY_RIGHT_SHIFT     ::LightEngine::Key::RightShift
#define LE_KEY_RIGHT_CONTROL   ::LightEngine::Key::RightControl
#define LE_KEY_RIGHT_ALT       ::LightEngine::Key::RightAlt
#define LE_KEY_RIGHT_SUPER     ::LightEngine::Key::RightSuper
#define LE_KEY_MENU            ::LightEngine::Key::Menu