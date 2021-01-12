#pragma once
enum class eDirection { eUndefined, eLeft, eDown, eRight, eUp };

enum class Button
{
    eUndefined,
    eUp,
    eDown,
    eRight,
    eLeft
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
    default:
        break;
    }

    return ret;
}