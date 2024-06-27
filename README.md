# Robotarm
Bachelor's degree code for the roboticarm

- Uses MATLAB to create an UI for user, solves the inverse kinematics and creates a "pathing list".
- In the UI user is able to see 3d render of the MeARM-robot, create and modify paths for the robot to move through and to change different settings (COM-port, automatic pathing etc.)
- MATLAB uses a Jacobian Pseudo-inverse algorithm in order to solve inverse kinematics problem.
- MATLAB communicates with Arduino via Serial Port and sends a "pathing list" through serial communication.
- The Arduino code handles the parsing of the "pathing list" and moves each joint of the robot accordingly.

Example of the project setup:
![image](https://github.com/Fazsu/Robotarm/assets/89864966/07df2a2b-c992-45c6-80bf-254f0a2a5df0)

An example video of the system working (with finnish explanation) can be seen here: 
[Youtube link](https://www.youtube.com/watch?v=jDWK1_aLBoo)
