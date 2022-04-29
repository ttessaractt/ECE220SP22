#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  Shape(string name) {
  name_ = name;
	}
	
  string getName() {
    return name_;
	}
	
  virtual double getArea() const = 0;
  virtual double getVolume() const = 0;
	
private:
  string name_;

};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
    //Shape("Rectangle");
    width_ = width;
    length_ = length;
	}
	
  double getArea() const {
    return (double)(width_ * length_);     
	}
	
  double getVolume() const {
    return (double)0;
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
    Rectangle<T> rec3(width_, length_);
    rec3.width_ = max((T)0, (width_ + rec.getWidth()));
    rec3.length_ = max((T)0, (length_ + rec.getLength()));
    return rec3;
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
    Rectangle<T> rec3(width_, length_);
    rec3.width_ = max((T)0, (width_ - rec.getWidth()));
    rec3.length_ = max((T)0, (length_ - rec.getLength()));
    return rec3;
  }

	T getWidth() const { 
    return (T)width_;	
	}
	
	T getLength() const { 
    return (T)length_;	
	}
private:
	T width_;
  T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  Circle(double radius):Shape("Circle") {
    radius_ = radius;
	}
	
  double getArea() const{
    return ((radius_*radius_)*3.141592);
	}
	
 	double getVolume() const{
    return (double)0;
	}
	
  Circle operator + (const Circle& cir){
    Circle cir3(radius_);
    cir3.radius_ = radius_ + cir.getRadius();
    return cir3; 
	}
	
	Circle operator - (const Circle& cir){
    Circle cir3(radius_);
    cir3.radius_ = max((double)0, (radius_ - cir.getRadius()));
    return cir3; 	} 

	double getRadius() const { 
    return radius_;	
	}
	
private:
	double radius_;

};


//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  Sphere(double radius):Shape("Sphere") {
    radius_ = radius;
	}

  double getVolume() const {
    return ((4.0/3.0)*(radius_*radius_*radius_)*3.141592);
	}	
	
  double getArea() const {
    return (4*3.141592*(radius_*radius_));
	}

	Sphere operator + (const Sphere& sph) {
    Sphere sph3(radius_);
    sph3.radius_ = radius_ + sph.getRadius();
    return sph3; 
	}

	Sphere operator - (const Sphere& sph) {
    Sphere sph3(radius_);
    sph3.radius_ = max((double)0, (radius_ - sph.getRadius()));
    return sph3; 
	} 
	
	double getRadius() const {
    return radius_;	
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class RectPrism : public Shape{
public:
  RectPrism(double width, double length, double height):Shape("RectPrism") {
    length_ = length;
    width_ = width;
    height_ = height;
	}
	
  double getVolume() const {
    return (length_ * width_ * height_);
	}
  	
	double getArea() const {
    return (2*(length_ * width_ + length_ * height_ + width_ * height_));
	}
	
	RectPrism operator + (const RectPrism& rectp){
    RectPrism obj1(width_, length_, height_);
    obj1.length_ = length_ + rectp.getLength();
    obj1.width_ = width_ + rectp.getWidth();
    obj1.height_ = height_ + rectp.getHeight();
    return obj1;
	}
	
	RectPrism operator - (const RectPrism& rectp){
    RectPrism obj2(width_, length_, height_);
    obj2.length_ = max((double)0, (length_ - rectp.getLength()));
    obj2.width_ = max((double)0, (width_ - rectp.getWidth()));
    obj2.height_ = max((double)0, (height_ - rectp.getHeight()));
    return obj2;
	}	
	
	double getWidth() const { 
    return width_;
	}
	
	double getLength() const { 
    return length_;
	}
	
	double getHeight() const { 
    return height_;
	}
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
  list<Shape*> list1;
  int c;
  int i = 0;
  ifstream ifs(file_name, std::ifstream::in);
  ifs >> c;
  while (!ifs.eof()){
    if (i > c){
      break;
    }
    string name;
    ifs >> name;
    if (name == "Rectangle"){
      double w, l;
      ifs >> w >> l;
      Shape* shape_ptr = new Rectangle<double>(w, l);
      list1.push_back(shape_ptr);
      i++;
    }
    if (name == "Circle"){
      double r;
      ifs >> r;
      Shape* shape_ptr = new Circle(r);
      list1.push_back(shape_ptr);
      i++;
    }
    if (name == "Sphere"){
      double r;
      ifs >> r;
      Shape* shape_ptr = new Sphere(r);
      list1.push_back(shape_ptr);
      i++;
    }
    if (name == "RectPrism"){
      double w, l, h;
      ifs >> w >> l >> h;
      Shape* shape_ptr = new RectPrism(w, l, h);
      list1.push_back(shape_ptr);
      i++;
    }

  }
  ifs.close();
 
  return list1;
  //return list<Shape*>(0, NULL);;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
  for (list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
    Shape * shape = *it;
    max_area = max(max_area, shape->getArea());
  }
	
	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
  for (list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++){
    Shape * shape = *it;
    max_volume = max(max_volume, shape->getVolume());
  }
		
	return max_volume;
}
#endif

