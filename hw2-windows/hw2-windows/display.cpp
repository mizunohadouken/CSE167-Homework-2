/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"
#include "Geometry.h"

using namespace std; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  

// TODO remove test matrix
void print4x4Matrix(mat4 mPrintMe)
{
	std::cout << mPrintMe[0][0] << " ";
	std::cout << mPrintMe[1][0] << " ";
	std::cout << mPrintMe[2][0] << " ";
	std::cout << mPrintMe[3][0] << "\n";
	std::cout << mPrintMe[0][1] << " ";
	std::cout << mPrintMe[1][1] << " ";
	std::cout << mPrintMe[2][1] << " ";
	std::cout << mPrintMe[3][1] << "\n";
	std::cout << mPrintMe[0][2] << " ";
	std::cout << mPrintMe[1][2] << " ";
	std::cout << mPrintMe[2][2] << " ";
	std::cout << mPrintMe[3][2] << "\n";
	std::cout << mPrintMe[0][3] << " ";
	std::cout << mPrintMe[1][3] << " ";
	std::cout << mPrintMe[2][3] << " ";
	std::cout << mPrintMe[3][3] << "\n\n";
	return;
}


void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
	glm::vec4 inputvec(input[0], input[1], input[2], input[3]);

	glm::vec4 outputvec = modelview * inputvec;

	output[0] = outputvec[0];

	output[1] = outputvec[1];

	output[2] = outputvec[2];

	output[3] = outputvec[3];
}

void display() 
{
  glClearColor(0, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the camera view

  // Either use the built-in lookAt function or the lookAt implemented by the user.
  if (useGlu) {
    modelview = glm::lookAt(eye,center,up); 
  } else {
    modelview = Transform::lookAt(eye,center,up); 
  }

  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

  // Lights are transformed by current modelview matrix. 
  // The shader can't do this globally. 
  // So we need to do so manually.  
  if (numused) {
    glUniform1i(enablelighting,true);

	// TODO - Verify this CodePass light and color vector arrays to shader
    // YOUR CODE FOR HW 2 HERE.  
    // You need to pass the light positions and colors to the shader. 
    // glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
    // The lightransf[] array in variables.h and transformvec() might also be useful here.
    // Remember that light positions must be transformed by modelview.

	GLfloat in_temp_light[4], out_mv_light[4], temp_color[4]; // create temp arrays to store 4vectors from lightposn and lightcolor arrays

	for (int i = 0; i < numused; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			in_temp_light[j] = lightposn[(i * 4) + j];
			temp_color[j] = lightcolor[(i * 4) + j];
		}
		transformvec(in_temp_light, out_mv_light);
		lightransf[(i * 4)] = out_mv_light[0];
		lightransf[(i * 4) + 1] = out_mv_light[1];
		lightransf[(i * 4) + 2] = out_mv_light[2];
		lightransf[(i * 4) + 3] = out_mv_light[3];
	}
	
	glUniform4fv(lightpos, numused, lightransf);
	glUniform4fv(lightcol, numused, lightcolor);
	glUniform1i(numusedcol, numused);
	// end my code

  } else {
    glUniform1i(enablelighting,false); 
  }

  // Transformations for objects, involving translation and scaling 
  mat4 sc(1.0) , tr(1.0), transf(1.0); 
  sc = Transform::scale(sx,sy,1.0); 
  tr = Transform::translate(tx,ty,0.0); 

  // YOUR CODE FOR HW 2 HERE.  
  // You need to use scale, translate and modelview to 
  // set up the net transformation matrix for the objects.  
  // Account for GLM issues, matrix order (!!), etc.  


  // The object draw functions will need to further modify the top of the stack,
  // so assign whatever transformation matrix you intend to work with to modelview
  // rather than use a uniform variable for that.

  int test = 1;// TODO remove this test case variable, toggle between 1/0

  if (test == 1)
  {
	  transf = modelview*tr*sc;  // TODO Check these values
	  print4x4Matrix(transf);
	  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(transf)[0][0]); // TODO is this needed?
  }
  else if (test == 0)
  {
	  transf = tr*sc;  // TODO Check these values
	  modelview = modelview * transf;
	  print4x4Matrix(modelview);
	  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(modelview)[0][0]); // TODO is this needed?
  }

	// end my code

  
  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

	// TODO verify this is correct
    // YOUR CODE FOR HW 2 HERE. 
    // Set up the object transformations 
    // And pass in the appropriate material properties
    // Again glUniform() related functions will be useful

	if (test == 1)
	{
		print4x4Matrix(modelview);
		print4x4Matrix(obj->transform);
		modelview = transf * obj->transform;
		print4x4Matrix(modelview);
	}
	else if (test ==0)
	{
		print4x4Matrix(modelview);
		print4x4Matrix(obj->transform);
		modelview = modelview * obj->transform;
//		print4x4Matrix(modelview);
	}
//	glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &(transf)[0][0]);


	glUniform4fv(ambientcol, 1, obj->ambient); // TODO add each material property
	glUniform4fv(diffusecol, 1, obj->diffuse);
	glUniform4fv(specularcol, 1, obj->specular);
	glUniform4fv(emissioncol, 1, obj->emission);
	glUniform1f(shininesscol, obj->shininess);
	// end my code
	

    // Actually draw the object
    // We provide the actual drawing functions for you.  
    // Remember that obj->type is notation for accessing struct fields
    if (obj->type == cube) {
      solidCube(obj->size); 
    }
    else if (obj->type == sphere) {
      const int tessel = 20; 
      solidSphere(obj->size, tessel, tessel); 
    }
    else if (obj->type == teapot) {
      solidTeapot(obj->size); 
    }
	
  }
  
  glutSwapBuffers();
}
