# Pi Parking System
This Repository contains the source code and report for the Pi Parking System.

### Overview:
A real-time parking system prototype that uses 
- GPIO for gear simulation, OpenCV to display a 15FPS camera feed in reverse gear, and a proximity sensor with motor to apply brakes within 300ms of obstacle detection in forward gear
- POSIX APIs to create services for the camera, sensor, motor, and scheduler, and used semaphores for synchronization among them
