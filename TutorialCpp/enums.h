#pragma once
enum class eDirection { eUndefined, eLeft, eDown, eRight, eUp };

enum class Button
{
    eUndefined,
    eUp,
    eDown,
    eRight,
    eLeft,
    eRestart,
    eX, //algorithm BFS find path
    eD, //algorithm Dijkstra find path
    eQ,
    eLeftUpCorner, // установить в левый угол текущую позицию кнопка ]ъ
    eTilda
};

enum class DeveloperCommand
{
    eLoad
};

eDirection ButtoToDir(Button button)
{
    eDirection dir = eDirection::eUndefined;
    switch (button)
    {
    case Button::eUp:
        dir = eDirection::eUp;
        break;
    case Button::eDown:
        dir = eDirection::eDown;
        break;
    case Button::eRight:
        dir = eDirection::eRight;
        break;
    case Button::eLeft:
        dir = eDirection::eLeft;
        break;
    default:
        break;
    }

    return dir;
}

Button ToButton(int key)
{
    Button ret = Button::eUndefined;
    switch (key)
    {
    case 72:
        ret = Button::eUp;
        break;
    case 80:
        ret = Button::eDown;
        break;
    case 75:
        ret = Button::eLeft;
        break;
    case 77:
        ret = Button::eRight;
        break;
    case 120:
        ret = Button::eX;
        break;
    case 122:
        ret = Button::eRestart;
        break;
    case 113:
        ret = Button::eQ; 
        break;
    case 93:
    case 234:
        ret = Button::eLeftUpCorner;
        break;
    case 100:
    case 162:
        ret = Button::eD;
        break;
    case 96:
    case 241:
        ret = Button::eTilda;
        break;
    default:
        break;
    }

    return ret;
}