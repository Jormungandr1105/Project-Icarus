# Project-Icarus
Attempt at Machine Learning for Airfoil Design and Export

Current Progress:
- Creating 3D Objects from Point Clouds
  - Current Shapes: Sphere, Cube, Rectangle - Achieved
  - Working on creating shapes from input files with basic information about their geometries - Achieved
  - Ability to Make Complex Shapes - Achieved  
  - Ability to Create Holes in Shapes - Current Focus
- Creating shapes from piece-wise functions (Probably Just Gonna Take Pre-Generated Point Clouds)

In the Future:
- Generate Wing Shapes
- Run Aerodynamics and Strengths Equations on Wings
- Automate Above Equations to Get Desired Wing Geometry and Design as Good as Possible


# Usage
Am planning on updating the cmake files to make it easier to build on its own, but currently, the building process invloves using g++ to turn all of the c++ files in the STL_GENERATOR folder into an executable.

Then, run that file from the command-line followed by the name of the expected stl file, without ".stl", and then the name of the input file from the Input_files folder.
