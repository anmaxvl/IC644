Computer Graphics.
Fall 2012

An Maksim

Programming Assignment #3 (Ray tracing)

What is done:
	- Added new primitive Cube, which is a subclass of Primitive. 
	- Added a new virtual function to Primitive class setTransformationMatrix(float trMatrix[3][3]) so it is easier to control Cube orientation.
	- In Engine class added haze coefficient setter and haze-color getter (attached screenshots for different values of "ro").
	

On the scene:
	- One reflective, refractive and diffuse cube.

Current (maybe) Problem:
	I think that the reflection and diffuse cubes are OK, but the refraction cube seems a bit "not realistic", but I couldn't fix it. But may be it only seems that way.
