ORGB Color Space Application
1. Evironment.
- MS visual studio 2015.
- Window 7, 10.
- Compile/run in win32 mode.
2. External library.
 All below libs are placed in 'libs' folder.
- CImg: using for load/save/display image.
- libjpeg: using for load/save jpg format.
- eigen: using for matrix/vector calculation.
4. Source Structure.
- ORGBSpace.cpp: is main class to convert from RGB to ORGB and vice versa, the adjusment is also handled in this.
- Utility.cpp: includes some helper functions to manipulate on CImg such as decode RGB component, save image...
- Main.cpp: hanldes for command line, and using ORGBSpace object to apply color adjustment.
3. How it works:
- Using command line interface to interact with user.
- User enter the image file to be adjusted color and wait for the process completed. Note that currently this program supports only *.jpg format.
- After adjusting process completed, a composed image will be display, and the same result file is also saved to "Result" folder.
