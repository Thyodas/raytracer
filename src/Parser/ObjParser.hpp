/*
** EPITECH PROJECT, 2023
** raytracer_obj
** File description:
** Parser.hpp
*/

#pragma once

#include "Parser.hpp"
#include "../../shared/math/Vectors/Vec3.hpp"
#include "../../shared/math/Vectors/Vec2.hpp"

namespace raytracer {
    class Core;
}

namespace Parser {

    namespace ObjParserData {

        struct VertexInfo {
            Vec3f vertice;
            Vec2f st;
            Vec3f normal;

            VertexInfo() : st(INFINITY), normal(INFINITY) {}
        };

        struct Object {
            std::string name = "Default Object";
            std::vector<Vec3f> vertexArray;
            std::vector<int> faceIndex;
            std::vector<uint32_t> vertexIndex;
            std::vector<Vec3f> normals;
            std::vector<Vec2f> st;
        };

        struct Data {
            std::vector<Object> objects;
            std::vector<Vec3f> tmpVertices;
            std::vector<Vec3f> tmpNormals;
            std::vector<Vec2f> tmpSt;
            std::size_t obj_index = 0;
        };

        struct transformationsOptions {
            Vec3f pos;
            float scaleFactorX = 1;
            float scaleFactorY = 1;
            float scaleFactorZ = 1;
            float rotateXAxis = 0;
            float rotateYAxis = 0;
            float rotateZAxis = 0;
        };
    }





    class ObjParser : public AParser<ObjParserData::Data> {
        public:
            ObjParser();
    };

} // Parser

