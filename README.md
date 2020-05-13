# Oxford Radar RobotCar LiDAR visualiser
This is a project to visualise LiDAR data from the Oxford RobotCar 
dataset. See the GitHub page from one of their team for the data set
[RobotCar Dataset](https://dbarnes.github.io/radar-robotcar-dataset/). 

This project was implemented in C++. and OpenGL. There are lots 
of examples of this being done in Python, so I thought I would try 
something different. The data is loaded from the binary files and 
not the raw data in the .png files. It is noisy, and has not been 
cleaned in anyway. These are all things for the future. 

You may need to zoom into the .gif below, but you can clearly see 
pedestrians walking on the pavement, and a truck, car and bus drive 
past. As well as definition on the buildings to the left of the 
car (top of the screen) such as bay windows, a common feature of 
UK buildings. The car is fitted with two LiDAR scanners and the below 
.gif shows only the left hand one. At the bottom of the screen 
you can see it is detecting some of the pulses from the second LiDAR
 scanner on the right hand side of the car. 


![](RobotCarDemo.gif)


