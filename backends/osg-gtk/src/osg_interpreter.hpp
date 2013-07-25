/*
  Copyright (c) 2012 Christoph Höger, All Rights Reserved

  This file is part of modelica3d
  (https://mlcontrol.uebb.tu-berlin.de/redmine/projects/modelica3d-public).

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cmath>

#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Node>
#include <osg/StateSet>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile> // LoadNodeFile-Operator
#include <osg/Node> // LoadNodeFile-Operator
#include <osg/Program>
#include <osg/Shader>

#include "operations.hpp"

using namespace proc3d;
using namespace osg;

typedef std::map<std::string, ref_ptr<PositionAttitudeTransform>> t_node_cache;
typedef std::map<std::string, ref_ptr<PositionAttitudeTransform>> t_shape_cache;
typedef std::map<std::string, ref_ptr<Material>> t_material_cache;

struct proc3d_osg_interpreter : boost::static_visitor<> {
private:
  const ref_ptr<Group> root;
public:
  t_node_cache& node_cache;
  t_shape_cache & shape_cache;
  t_material_cache& material_cache;

  proc3d_osg_interpreter(const ref_ptr<Group> r, t_node_cache& c, t_shape_cache & s, t_material_cache& m) :
    root(r), node_cache(c), shape_cache(s), material_cache(m) {}

  void operator()(const CreateGroup& cmd) const {

  }

  void operator()(const AddToGroup& cmd) const {

  }

  void operator()(const CreateMaterial& cmd) const {
    ref_ptr<Material> mat = new Material();
    mat->setName(cmd.name);
    material_cache[cmd.name] = mat;
  }

  void operator()(const ApplyMaterial& cmd) const {

  // do not apply material to file objects (where target name begins with "file")
  // a better idea is needed -> are the colors specified in modelica to be ignored or not?
  const std::string FILE("file");
  if (cmd.name.compare(0, FILE.length(), FILE) == 0) return;

    if (node_cache.find(cmd.name) == node_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl;
      return;
    }

    if (material_cache.find(cmd.target) == material_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find material: " << cmd.target << std::endl;
      return;
    }

    //std::cout << "Apply material " << cmd.target << " on " << cmd.name << std::endl;

    const ref_ptr<Material> mat = material_cache[cmd.target];

    ref_ptr<StateSet> stateSet = node_cache[cmd.name] -> getChild(0) -> getOrCreateStateSet();
    stateSet->setAttribute(mat.get());
  }

  void operator()(const CreateShape& cmd) const {

    //std::cout 
      //<< "name: " << cmd.name << "\n"
      //<< "descr: " << cmd.descr << "\n"
      //<< "length: " << cmd.length << "\n"
      //<< "width: " << cmd.width << "\n"
      //<< "height: " << cmd.height << "\n"
      //<< "x: " << cmd.at[0] << "\n"
      //<< "y: " << cmd.at[1] << "\n"
      //<< "z: " << cmd.at[2] << "\n"
      //<< "extra: " << cmd.extra << "\n"
      //<< std::endl;

    // generic setup
    const char * descr = cmd.descr.c_str();
    const ref_ptr<Geode> geode = new Geode();
    const ref_ptr<PositionAttitudeTransform> shape = 
      new PositionAttitudeTransform();
    shape->addChild(geode);
    const ref_ptr<PositionAttitudeTransform> trans =
      new PositionAttitudeTransform();
    trans->addChild(shape);
    trans->setName(cmd.name);

    // shape specific
    if (!strcmp(descr, "cylinder")) {
      // create geometry
      geode->addDrawable(new ShapeDrawable(new Cylinder));

      // set size
      shape->setScale(osg::Vec3(cmd.width, cmd.width, cmd.height));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // translate to center
      shape->setPosition(target / 2.0);

      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else if (!strcmp(descr, "cone")) {
      // create geometry
      geode->addDrawable(new ShapeDrawable(new Cone));

      // set size
      shape->setScale(osg::Vec3(cmd.width, cmd.width, cmd.height));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else if (!strcmp(descr, "sphere")) {
      // create geometry
      geode->addDrawable(new ShapeDrawable(new Sphere));

      // translate to center
      shape->setPosition(Vec3(cmd.width/2, 0, 0));

      // null rotation
      shape->setAttitude(Quat(0, Vec3(1,0,0)));

      // set size
      shape->setScale(osg::Vec3(cmd.width/2, cmd.width/2, cmd.width/2));

    } else if (!strcmp(descr, "box")) { 
      // create geometry
      geode->addDrawable(new ShapeDrawable(new Box));

      // set size
      shape->setScale(osg::Vec3(cmd.length/2, cmd.width/2, cmd.height/2));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // translate to center
      shape->setPosition(target / 2.0);

      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else if (!strcmp(descr, "plane")) { 
      // create geometry
      geode->addDrawable(new ShapeDrawable(new Box));

      // set size
      shape->setScale(osg::Vec3(cmd.length/2, cmd.width/2, 0.01));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else {
      std::cerr << "shape: " << descr << " not implemented" << std::endl;
      return;
    }

    // add to caches and scene
    node_cache[cmd.name] = trans;
    shape_cache[cmd.name] = shape;
    root->addChild(trans);
  }

  void operator()(const UpdateShape& cmd) const {
    //std::cout 
      //<< "name: " << cmd.name << "\n"
      //<< "descr: " << cmd.descr << "\n"
      //<< "time: " << cmd.time << "\n"
      //<< "length: " << cmd.length << "\n"
      //<< "width: " << cmd.width << "\n"
      //<< "height: " << cmd.height << "\n"
      //<< "x: " << cmd.at[0] << "\n"
      //<< "y: " << cmd.at[1] << "\n"
      //<< "z: " << cmd.at[2] << "\n"
      //<< "extra: " << cmd.extra << "\n"
      //<< std::endl;

    // get shape
    if (shape_cache.find(cmd.name) == shape_cache.end()) {
      std::cout << "Could not find shape for update shape: " << cmd.name << std::endl;
      return;
    }
    osg::PositionAttitudeTransform * shape = shape_cache[cmd.name];

    const char * descr = cmd.descr.c_str();

    if (!strcmp(descr, "cylinder")) {
      // set size
      shape->setScale(Vec3d(cmd.width/2, cmd.width/2, cmd.length));
      
      // translate to center
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.length;
      const Vec3 center = target / 2.0;
      shape->setPosition(center);

      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);
    
    } else if (!strcmp(descr, "cone")) { 
      // set size
      shape->setScale(osg::Vec3(cmd.width/2, cmd.width/2, cmd.height));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
 
      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else if (!strcmp(descr, "sphere")) { 
      // set size
      shape->setScale(osg::Vec3(cmd.width/2, cmd.width/2, cmd.width/2));

    } else if (!strcmp(descr, "box")) { 
      // set size
      shape->setScale(osg::Vec3(cmd.length/2, cmd.width/2, cmd.height/2));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // translate to center
      shape->setPosition(target / 2.0);

      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else if (!strcmp(descr, "plane")) { 
      // set size
      shape->setScale(osg::Vec3(cmd.length, cmd.width, 0.01));

      // target vector
      osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
      target.normalize();
      target *= cmd.height;
      
      // rotate to target
      const Vec3 z = osg::Vec3(0,0,1);
      Quat q; q.makeRotate(z, target);
      shape->setAttitude(q);

    } else {
      std::cerr << "shape: " << descr << " not implemented" << std::endl;
    }

  }

  void operator()(const Move& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl;
      return;
    }

    node_cache[cmd.name] -> setPosition(Vec3d(cmd.x, cmd.y, cmd.z));
  }

  void operator()(const Scale& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl;
      return;
    }

    node_cache[cmd.name] -> setScale(Vec3d(cmd.x, cmd.y, cmd.z));
  }

  void operator()(const RotateEuler& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl;
      return;
    }

    Quat q(cmd.x, osg::Vec3(1,0,0), cmd.y, osg::Vec3(0,1,0), cmd.z, osg::Vec3(0,0,1));
    node_cache[cmd.name] -> setAttitude(q);
  }

  void operator()(const RotateMatrix& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl;
      return;
    }

    Quat q;
    const auto& m = cmd.m;

    q.set(osg::Matrixd(m(0,0), m(0,1), m(0,2), 0.0,
                       m(1,0), m(1,1), m(1,2), 0.0,
                       m(2,0), m(2,1), m(2,2), 0.0,
                       0.0, 0.0, 0.0, 1.0));

    node_cache[cmd.name] -> setAttitude(q);
  }

  void operator()(const SetMaterialProperty& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl;
      return;
    }
    //no properties defined yet ...
  }

  static inline Vec4d vec4_from_array(const boost::array<double, 4>& arr) {
    return Vec4d(arr[0],arr[1],arr[2],arr[3]);
  }

  void operator()(const SetAmbientColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl;
      return;
    }

    //std::cout << "Setting ambient color on " << cmd.name << " at t= " << cmd.time << std::endl;
    material_cache[cmd.name]->setAmbient(Material::FRONT, vec4_from_array(cmd.color));
  }

  void operator()(const SetDiffuseColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl;
      return;
    }

    material_cache[cmd.name]->setDiffuse(Material::FRONT, vec4_from_array(cmd.color));
  }

  void operator()(const SetSpecularColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      //std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl;
      return;
    }

    material_cache[cmd.name]->setSpecular(Material::FRONT, vec4_from_array(cmd.color));
  }

  // LoadObject
  void operator()(const LoadObject& cmd) const {
    ref_ptr<Node> node = osgDB::readNodeFile(cmd.fileName); // fileName better be absolute
    if(!node.valid()){
      std::cout << "Cannot open File: " << cmd.fileName << std::endl;
      return;
    }

    // Shader Quellcode
    std::string vs =
      "varying vec3 vNormal;"
      "varying vec3 ecPosition3;"
      "void main(void)"
      "{"
      "  gl_Position = ftransform();"
      "  vNormal = gl_NormalMatrix * gl_Normal;"
      "  vec4 ecPosition4 = gl_ModelViewMatrix * gl_Vertex;"
      "  ecPosition3 =(vec3(ecPosition4)) / ecPosition4.w;"
      "}";

    std::string fs =
      "varying vec3 vNormal;"
      "varying vec3 ecPosition3;"
      "void main(void)"
      "{"
      "  vec3 L = normalize(ecPosition3);"
      "  vec3 N = normalize(vNormal);"
      "  float D = abs(dot(N,L));"
      "  vec3 ambient = gl_FrontMaterial.ambient * D;"
      "  vec3 diffuse = gl_FrontMaterial.diffuse * D;"
      "  vec3 color = diffuse;"
      "  gl_FragColor = vec4(color,1.0);"
      "}";

    // OSG-Shader
    osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX,vs);  
    osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT,fs);

    // Shader-Program-Objekt
    osg::Program* sProgram = new osg::Program;
    sProgram->addShader(vShader);
    sProgram->addShader(fShader);

    ref_ptr<StateSet> ss = node->getOrCreateStateSet();
    ss->setAttributeAndModes( sProgram, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    const ref_ptr<PositionAttitudeTransform> trans = new PositionAttitudeTransform();
    trans->addChild(node);
    trans->setName(cmd.name);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }
};

// vim:ts=2:sw=2:expandtab
