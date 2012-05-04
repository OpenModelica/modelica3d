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

#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>

#include "operations.hpp"

using namespace proc3d;
using namespace osg;

typedef std::map<std::string, ref_ptr<PositionAttitudeTransform>> t_node_cache;
typedef std::map<std::string, ref_ptr<Material>> t_material_cache;

struct proc3d_osg_interpreter : boost::static_visitor<> {
private:
  const ref_ptr<Group> root;
public:
  t_node_cache& node_cache;
  t_material_cache& material_cache;

  proc3d_osg_interpreter(const ref_ptr<Group> r, t_node_cache& c, t_material_cache& m) : 
    root(r), node_cache(c), material_cache(m) {}

  void operator()(const CreateGroup& cmd) const {

  }

  void operator()(const LoadObject& cmd) const {

  }

  void operator()(const AddToGroup& cmd) const {
    
  }

  void operator()(const CreateMaterial& cmd) const {
    ref_ptr<Material> mat = new Material();
    mat->setName(cmd.name);
    material_cache[cmd.name] = mat;
  }

  void operator()(const ApplyMaterial& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl; 
      return;
    }

    if (material_cache.find(cmd.target) == material_cache.end()) {
      std::cout << "Inconsistent naming. Did not find material: " << cmd.target << std::endl; 
      return;
    }

    std::cout << "Apply material " << cmd.target << " on " << cmd.name << std::endl;    

    const ref_ptr<Material> mat = material_cache[cmd.target];

    ref_ptr<StateSet> stateSet = node_cache[cmd.name] -> getChild(0) -> getOrCreateStateSet();
    stateSet->setAttribute(mat.get());
  }

  void operator()(const CreateSphere& cmd) const {        
    const ref_ptr<ShapeDrawable> shape =
      new ShapeDrawable(new Sphere(Vec3(0,0,0), cmd.radius));
    const ref_ptr<Geode> geode = new Geode();
    geode->addDrawable(shape);

    const ref_ptr<PositionAttitudeTransform> trans = 
      new PositionAttitudeTransform();
    trans->addChild(geode);
    trans->setName(cmd.name);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }

  void operator()(const CreateBox& cmd) const {
    osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
    target.normalize();
    target *= cmd.height;
    const Vec3 center = target / 2.0;
    
    //Rotate to target
    const Vec3 z = osg::Vec3(0,0,1);    
    Quat q; q.makeRotate(z, target); 

    const ref_ptr<Box> shape = new Box(center, cmd.width, cmd.length, cmd.height);
    const ref_ptr<ShapeDrawable> draw = new ShapeDrawable(shape);
    
    const ref_ptr<Geode> geode = new Geode();
    geode->addDrawable(draw);

    const ref_ptr<PositionAttitudeTransform> trans = 
      new PositionAttitudeTransform();
    trans->addChild(geode);
    trans->setName(cmd.name);

    shape->setRotation(q);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }

  void operator()(const CreateCylinder& cmd) const {
    osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
    target.normalize();
    target *= cmd.height;
    const Vec3 center = target / 2.0;
    
    //Rotate to target
    const Vec3 z = osg::Vec3(0,0,1);    
    Quat q; q.makeRotate(z, target);    

    const ref_ptr<Cylinder> shape = new Cylinder(center, cmd.radius, cmd.height);
    const ref_ptr<ShapeDrawable> draw = new ShapeDrawable(shape);
      
    const ref_ptr<Geode> geode = new Geode();
    geode->addDrawable(draw);

    const ref_ptr<PositionAttitudeTransform> trans = 
      new PositionAttitudeTransform();
    trans->addChild(geode);
    trans->setName(cmd.name);
 
    shape->setRotation(q);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }

  void operator()(const CreateCone& cmd) const {
    osg::Vec3 target = osg::Vec3(cmd.at[0], cmd.at[1], cmd.at[2]);
    target.normalize();
    target *= cmd.height;
    
    //Rotate to target
    const Vec3 z = osg::Vec3(0,0,1);
    Quat q; q.makeRotate(z, target);
    
    const ref_ptr<Cone> shape = new Cone(Vec3d(0,0,0), cmd.radius, cmd.height);
    const ref_ptr<ShapeDrawable> draw = new ShapeDrawable(shape);
      
    const ref_ptr<Geode> geode = new Geode();
    geode->addDrawable(draw);

    const ref_ptr<PositionAttitudeTransform> trans = 
      new PositionAttitudeTransform();
    trans->addChild(geode);
    trans->setName(cmd.name);

    shape->setRotation(q);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }

  void operator()(const CreatePlane& cmd) const {
    const ref_ptr<Shape> shape = new Box(Vec3(0,0,0), cmd.width, cmd.length, 0.05);
    const ref_ptr<ShapeDrawable> draw = new ShapeDrawable(shape);
    
    const ref_ptr<Geode> geode = new Geode();
    geode->addDrawable(draw);

    const ref_ptr<PositionAttitudeTransform> trans = 
      new PositionAttitudeTransform();
    trans->addChild(geode);
    trans->setName(cmd.name);

    node_cache[cmd.name] = trans;
    root->addChild(trans);
  }

  void operator()(const Move& cmd) const {   
    if (node_cache.find(cmd.name) == node_cache.end()) {
      std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl; 
      return;
    }

    node_cache[cmd.name] -> setPosition(Vec3d(cmd.x, cmd.y, cmd.z));    
  }

  void operator()(const Scale& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl; 
      return;
    }

    node_cache[cmd.name] -> setScale(Vec3d(cmd.x, cmd.y, cmd.z));
  }

  void operator()(const RotateEuler& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl; 
      return;
    }
    
    Quat q(cmd.x, osg::Vec3(1,0,0), cmd.y, osg::Vec3(0,1,0), cmd.z, osg::Vec3(0,0,1));
    node_cache[cmd.name] -> setAttitude(q);
  }

  void operator()(const RotateMatrix& cmd) const {
    if (node_cache.find(cmd.name) == node_cache.end()) {
      std::cout << "Inconsistent naming. Did not find " << cmd.name << std::endl; 
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
      std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl; 
      return;
    }
    //no properties defined yet ...
  }

  static inline Vec4d vec4_from_array(const boost::array<double, 4>& arr) {
    return Vec4d(arr[0],arr[1],arr[2],arr[3]);
  }

  void operator()(const SetAmbientColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl; 
      return;
    }

    std::cout << "Setting ambient color on " << cmd.name << " at frame " << cmd.frame << std::endl;    
    material_cache[cmd.name]->setAmbient(Material::FRONT, vec4_from_array(cmd.color));
  }

  void operator()(const SetDiffuseColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl; 
      return;
    }
    
    material_cache[cmd.name]->setDiffuse(Material::FRONT, vec4_from_array(cmd.color));
  }

  void operator()(const SetSpecularColor& cmd) const {
    if (material_cache.find(cmd.name) == material_cache.end()) {
      std::cout << "Inconsistent naming. Did not find material: " << cmd.name << std::endl; 
      return;
    }
    
    material_cache[cmd.name]->setSpecular(Material::FRONT, vec4_from_array(cmd.color));
  }

};

