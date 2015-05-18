Maksim An
Programming Assignment #2

1. I modified some source files from previous programming assignment, so in the project there aren't too many header and source files:
   - Everything considering robot I put in two files Robot.h and Robot.cpp
   - MotionEngine.h and MotionEngine.cpp were modified slightly in order to add hand/leg/body movement
2. Couple of new types and Classes:
   - t_Point is a structure to represent 3D points/vectors:
       - I overloaded some operations to simplify calculations: + and * operators. "+" adds two points/vectors and "*" multiplies point/vector by a scalar.
         And also there are some functions like inner and vector product.
   - class BezierCurve. Description in BezierCurve.h, implimentation in BezierCurve.cpp. This class is for storage of a bezier curve.
   - class Loop. Description in Loop.h, implimentation in Loop.cpp. Class for storing a loop, consisted with several bezier curves. C1 continuity is implemented.

3. Now about controls. By default the loop consists of 3 Bezier Curves and the order of a BC is 3 (cubic BC).
   Press "PLAY" on the left to start the roller coaster
   Press "STOP" on the left to stop the roller coaster
   Press "+" to zoom in
   Press "-" to zoom out

   "F1" - increases the number of BC in the loop (no upper limit is set, so you may increase as much as the hardware and software will allow you untill crush)
   "F2" - decreases the number of BC in the loop (minimum is 2)
   "s"  - show/hid control points and lines
   "l"  - turn lighting on/off
   "ENTER" - generate new random loop
   "UP" - increase the velocity of the moving robot
   "DOWN" decrease the velocity of the moving robot

4. Additional notes:
   - Now the rotation is possible only horizontaly, i.e. dragging left/right. I disabled the up/down dragging, because it has some bugs :(
   - The BezierCurve and Loop classes are universal, i.e. Bezier Curve can be any degree and Loop can contain any number (>=2) of BC in it. I tried to make as least
   "hard coding" as possible. 
   - I also wanted to include the controls for increasing/decreasing the degree of Bezier Curve (which is actually easy, because of my
   implementation), but thought that it's not necessary. 
   - I also messed up something with the lighting, I think, because compared to the PA#1 it looks ugly...
   - I tried to impliment the "first-person look", but unfortunately couldn't make it :( 