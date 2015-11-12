#include "ObjectDOM.h"

///Constructor
ObjectDOM::ObjectDOM()
{
    nombreTag = "";;
    id = "";
    clase = "";
    value = "";
    focus = true;
}

///Getters
SDL_Rect ObjectDOM::getArea()
{
    return ObjectDOM::area;
}

string ObjectDOM::getNombreTag()
{
    return ObjectDOM::nombreTag;
}

string ObjectDOM::getId()
{
    return ObjectDOM::id;
}

string ObjectDOM::getClase()
{
    return ObjectDOM::clase;
}

string ObjectDOM::getValue()
{
    return ObjectDOM::value;
}

bool ObjectDOM::getFocus()
{
    return ObjectDOM::focus;
}

///Setters
void ObjectDOM::setArea(SDL_Rect x)
{
    ObjectDOM::area = x;
}

void ObjectDOM::setNombreTag(string x)
{
    ObjectDOM::nombreTag = x;
}

void ObjectDOM::setId(string x)
{
    ObjectDOM::id = x;
}

void ObjectDOM::setClase(string x)
{
    ObjectDOM::clase = x;
}

void ObjectDOM::setValue(string x)
{
    ObjectDOM::value = x;
}

void ObjectDOM::setFocus(bool x)
{
    ObjectDOM::focus = x;
}
