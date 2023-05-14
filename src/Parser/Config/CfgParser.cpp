/*
** EPITECH PROJECT, 2023
** RayTracer
** File description:
** CfgParser.cpp
*/

#include "CfgParser.hpp"

#include <utility>

namespace Parser {

    CfgParser::CfgParser(std::string path)
    : _path(std::move(path))
    {
        try {
            _config.readFile(_path.c_str());
        } catch (libconfig::ParseException &e) {
            throw Parser::ParseFailureException(
                "couldn't parse '" + _path + "' line "
                + std::to_string(e.getLine()) + ": " + e.getError());
        } catch (libconfig::FileIOException &e) {
            throw Parser::OpenFailureException(
                "couldn't open '" + _path + "' line "
                + e.what());
        }
    }

    CfgParserData::Camera CfgParser::getCamera()
    {
        return _parser_wrapper("camera", [this]() -> CfgParserData::Camera {
            Matrix44f matrix;
            raytracer::Camera camera(
                matrix,
                _config.lookup("camera.resolution.width"),
                _config.lookup("camera.resolution.height"),
                _config.lookup("camera.fieldOfView")
            );
            Vec3f pos(
                _config.lookup("camera.position.x"),
                _config.lookup("camera.position.y"),
                _config.lookup("camera.position.z")
            );
            camera.translate(pos);
            camera.rotateAroundOriginX(_config.lookup("camera.rotation.x"));
            camera.rotateAroundOriginY(_config.lookup("camera.rotation.y"));
            camera.rotateAroundOriginZ(_config.lookup("camera.rotation.z"));
            return camera;
        })();
    }

    void CfgParser::_getObjectBase(primitive::Object &object,
        const libconfig::Setting &setting)
    {
        Vec3f pos(
            setting.lookup("position.x"),
            setting.lookup("position.y"),
            setting.lookup("position.z")
        );
        translate(object.objectToWorld, pos);
        object.worldtoObject = object.objectToWorld.inverse();
        object.kd = setting.lookup("kd");
        object.ks = setting.lookup("ks");
        object.ka = setting.lookup("ka");
        int material = setting.lookup("materialType");
        if (material < 0 || material >= primitive::NB_MATERIAL_TYPE)
            throw ParseFailureException("invalid material type");
        object.materialType = (primitive::MaterialType)material;
        int txt = setting.lookup("txtType");
        if (txt < 0 || txt >= primitive::NB_TXT_TYPE)
            throw ParseFailureException("invalid texture type");
        object.txtType = (primitive::Texture)txt;
        object.albedo.x = setting.lookup("albedo.r");
        object.albedo.y = setting.lookup("albedo.g");
        object.albedo.z = setting.lookup("albedo.b");
    }

    std::shared_ptr<primitive::Sphere> CfgParser::_getSphere(const libconfig::Setting &setting)
    {
        Matrix44f identity = Matrix44f(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        std::shared_ptr<primitive::Sphere> sphere =
            std::make_shared<primitive::Sphere>(identity, setting.lookup("radius"));
        _getObjectBase(*sphere, setting.lookup("object"));
        sphere->center.x = setting.lookup("center.x");
        sphere->center.y = setting.lookup("center.y");
        sphere->center.z = setting.lookup("center.z");
        sphere->radius = setting.lookup("radius");
        //multVecMatrix(Vec3f(0), center);
        return sphere;
    }

    std::shared_ptr<primitive::Plane> CfgParser::_getPlane(const libconfig::Setting &setting)
    {
        Matrix44f identity = Matrix44f(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        //math::translate(identity, Vec3f(1, -1, 2));
        Vec3f point(
            setting.lookup("point.x"),
            setting.lookup("point.y"),
            setting.lookup("point.z")
        );

        Vec3f normal(
            setting.lookup("normal.x"),
            setting.lookup("normal.y"),
            setting.lookup("normal.z")
        );
        std::shared_ptr<primitive::Plane> plane =
            std::make_shared<primitive::Plane>(identity, normal, point);
        _getObjectBase(*plane, setting.lookup("object"));
        return plane;
    }

    std::vector<std::shared_ptr<primitive::MeshTriangle>> CfgParser::_getObj(const libconfig::Setting &setting)
    {
        ObjParser parser;
        parser.parse(setting.lookup("path"));

        Vec3f pos(
            setting.lookup("object.position.x"),
            setting.lookup("object.position.y"),
            setting.lookup("object.position.z")
        );

        Vec3f color(
            setting.lookup("object.albedo.r"),
            setting.lookup("object.albedo.g"),
            setting.lookup("object.albedo.b")
        );

        int material = setting.lookup("object.materialType");
        if (material < 0 || material >= primitive::NB_MATERIAL_TYPE)
            throw ParseFailureException("invalid material type");

        int txt = setting.lookup("object.txtType");
        if (txt < 0 || txt >= primitive::NB_TXT_TYPE)
            throw ParseFailureException("invalid texture type");

        ObjParserData::TransformationsOptions opt = {
            .pos = pos,
            .color = color,
            .scaleFactorX = setting.lookup("object.scale.x"),
            .scaleFactorY = setting.lookup("object.scale.y"),
            .scaleFactorZ = setting.lookup("object.scale.z"),
            .rotateXAxis = setting.lookup("object.rotation.x"),
            .rotateYAxis = setting.lookup("object.rotation.y"),
            .rotateZAxis = setting.lookup("object.rotation.z"),
            .kd = setting.lookup("object.kd"),
            .ks = setting.lookup("object.ks"),
            .ka = setting.lookup("object.ka"),
            .materialType = static_cast<primitive::MaterialType>(material),
            .txtType = static_cast<primitive::Texture>(txt),
        };

        auto meshes = parser.getMeshTriangles(opt);
        return meshes;
    }

    CfgParserData::Primitives CfgParser::getPrimitives()
    {
        return _parser_wrapper("primitives", [this]() -> CfgParserData::Primitives {
            libconfig::Setting &root = _config.getRoot();
            CfgParserData::Primitives result;

            const libconfig::Setting &spheres = root["primitives"]["spheres"];
            for (const auto &item: spheres)
                result.push_back(_getSphere(item));

            const libconfig::Setting &planes = root["primitives"]["planes"];
            for (const auto &item: planes)
                result.push_back(_getPlane(item));

            const libconfig::Setting &objects = root["primitives"]["obj"];
            for (const auto &item: objects)
                for (const auto &meshes : _getObj(item))
                    result.push_back(meshes);

            return result;
        })();
    }

    std::shared_ptr<physics::DistantLight> CfgParser::_getDistantLight(const libconfig::Setting &setting)
    {
        Matrix44f l2w;
        Vec3f pos(
            setting.lookup("position.x"),
            setting.lookup("position.y"),
            setting.lookup("position.z")
        );
        Vec3f color(
            setting.lookup("color.r"),
            setting.lookup("color.g"),
            setting.lookup("color.b")
        );
        auto light = std::make_shared<physics::DistantLight>(
            l2w, color, setting.lookup("intensity")
        );
        light->dir = pos;
        return light;
    }

    std::shared_ptr<physics::PointLight> CfgParser::_getPointLight(const libconfig::Setting &setting)
    {
        Matrix44f l2w;
        Vec3f pos(
            setting.lookup("position.x"),
            setting.lookup("position.y"),
            setting.lookup("position.z")
        );
        translate(l2w, pos);
        Vec3f color(
            setting.lookup("color.r"),
            setting.lookup("color.g"),
            setting.lookup("color.b")
        );
        auto light = std::make_shared<physics::PointLight>(
            l2w, color, setting.lookup("intensity")
        );
        return light;
    }

    CfgParserData::Lights CfgParser::getLights()
    {
        return _parser_wrapper("lights", [this]() -> CfgParserData::Lights {
            libconfig::Setting &root = _config.getRoot();
            CfgParserData::Lights result;

            const libconfig::Setting &distant = root["lights"]["distant"];
            for (const auto &item: distant)
                result.push_back(_getDistantLight(item));

            const libconfig::Setting &point = root["lights"]["point"];
            for (const auto &item: point)
                result.push_back(_getPointLight(item));

            return result;
        })();
    }
} // Parser
