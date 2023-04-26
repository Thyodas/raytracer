/*
** EPITECH PROJECT, 2023
** raytracer_obj
** File description:
** Parser.hpp
*/

#pragma once

#include "Parser.hpp"

namespace Parser {

    namespace ObjParserData {
        struct Vertices {
            double x;
            double y;
            double z;
            double w = 1.0;
        };

        struct Object {
            std::string name = "Default Object";
            std::vector<Vertices> vertices;
        };

        struct Data {
            std::vector<Object> objects;
            std::size_t obj_index = 0;
        };
    }





    class ObjParser : public AParser<ObjParserData::Data> {
        public:
            ObjParser();
    };

} // Parser

