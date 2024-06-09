# Pi Parking System
This Repository contains the source code and report for the Pi Parking System.

### Overview:
A real-time parking system prototype that uses 
- GPIO for gear simulation, OpenCV to display a 15FPS camera feed in reverse gear, and a proximity sensor with motor to apply brakes within 300ms of obstacle detection in forward gear
- POSIX APIs to create services for the camera, sensor, motor, and scheduler, and used semaphores for synchronization among them

The demo video is available at https://o365coloradoedu-my.sharepoint.com/:v:/g/personal/krsu6520_colorado_edu/EXgLJrOD-BVKk5QX4JEsTfQBGJeGE91JdyALSpkgoQOenw?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=8J9eK8
