#include "game/Map.hpp"
#include "tinyxml2.h"

//namespace ygg {

//bool Map::loadFromXML( const char * fileName ) {
//    assert(fileName != nullptr);

//    tinyxml2::XMLDocument doc;
//    tinyxml2::XMLError error = doc.LoadFile(fileName);

//    if(error)
//        return false;

//    /*tinyxml2::XMLPrinter printer;
//    doc.Print();*/

//    tinyxml2::XMLElement * map = doc.FirstChildElement();
//    m_PlayerIndex = map->IntAttribute("player");

//    for(tinyxml2::XMLElement * chain = map->FirstChildElement(); chain != nullptr; chain = chain->NextSiblingElement()){
//        StaticChain sc;

//        sc.m_bLoop = chain->BoolAttribute("loop");

//        for(tinyxml2::XMLElement * vertex = chain->FirstChildElement(); vertex != nullptr; vertex = vertex->NextSiblingElement())
//            sc.m_Vertices.push_back(glm::vec2(vertex->FloatAttribute("x"), vertex->FloatAttribute("y")));

//        m_StaticChains.push_back(sc);
//    }

//    return true;
//}

//}

