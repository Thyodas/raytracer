/*
** EPITECH PROJECT, 2023
** raytracer_obj
** File description:
** Parser.hpp
*/

#pragma once

#include "Parser.hpp"
#include "Object.hpp"
#include "../../../shared/math/Vectors/Vec3.hpp"
#include "../../../shared/math/Vectors/Vec2.hpp"

namespace raytracer {
    class Core;
}

namespace Parser {

    namespace ObjParserData {

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
            std::vector<Vec3f> tmpNormals;
            std::vector<Vec2f> tmpSt;
            std::size_t obj_index = 0;
        };

        struct TransformationsOptions {
            Vec3f pos;
            Vec3f color = 0.18;
            float scaleFactorX = 1;
            float scaleFactorY = 1;
            float scaleFactorZ = 1;
            float rotateXAxis = 0;
            float rotateYAxis = 0;
            float rotateZAxis = 0;
            float kd = 0.8;
            float ks = 0.2;
            float ka = 0.2;
            primitive::MaterialType materialType = primitive::MaterialType::DIFFUSE_AND_GLOSSY;
        };

        typedef std::vector<std::shared_ptr<primitive::MeshTriangle>> MeshTriangles;
    }

    class ObjParser : public AParser<ObjParserData::Data> {
        public:
            ObjParser();
            void fillCore(raytracer::Core &core, Parser::ObjParserData::TransformationsOptions &opt);
            ObjParserData::MeshTriangles getMeshTriangles(Parser::ObjParserData::TransformationsOptions &opt);
    };

} // Parser

