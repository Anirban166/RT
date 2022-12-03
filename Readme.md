Build the executable using `make` and run using: 
```c
./raytrace [ImageWidth] [ImageHeight] [InputSceneName] [OutputFileName].ppm
```

| Raytracing | Illumination | Raycasting |
|:--- | :--- | :--- |
| ![RaytracerImage1](https://user-images.githubusercontent.com/30123691/204080279-f1661178-fe8f-4b7e-842b-480031f6d78b.png) | ![Illuminate1](https://user-images.githubusercontent.com/30123691/204080559-64ceb6a7-9fb8-4a96-b30c-ede696c160e4.png) | ![RaycasterImage1](https://user-images.githubusercontent.com/30123691/204081109-5bfb7b79-c59f-449c-b5f3-ec30f6bc4890.png) |
| A sphere and a plane with reflection on both objects | Illuminating a sphere and a plane with a point light | Basic render of a sphere and plane |
| ![RaytracerImage2](https://user-images.githubusercontent.com/30123691/204077636-502cebb2-7552-421d-83e7-c4d8a6802a4a.png) | ![Illuminate2](https://user-images.githubusercontent.com/30123691/204080430-9e67c3fd-1626-433d-b257-52aa1b2a70c3.png) | ![RaycasterImage2](https://user-images.githubusercontent.com/30123691/204080759-0bd0ee31-37d8-4c3e-94c0-83ffb88100ab.png) |
| Three spheres and a plane with reflection for the sphere objects | Illuminating a cylinder along the y-axis with a point light | A lemon (the quadric is an ellipsoid along the y-axis) |
| ![RaytracerImage3](https://user-images.githubusercontent.com/30123691/204080201-d8f2c0de-89c3-43d0-af1f-588044d280cf.png) | ![Illuminate3](https://user-images.githubusercontent.com/30123691/204080600-04a948d9-a6da-4b59-a1d1-2412e6a5ff50.png) | ![RaycasterImage3](https://user-images.githubusercontent.com/30123691/204081337-644c1cb3-0f8e-4648-814f-e3f3dae706d4.png) |
| Three spheres and a plane with reflection for the two spheres behind and refraction for the one in front | Illuminating a sphere and a plane with a spot light | Rendering two spheres, a plane, and a cone along the y-axis |

Scene files used to render the above:
- Raytracing
```c
camera, width: 2.0, height: 2.0
sphere, radius: 2.0, diffuse_color: [1, 0, 0], specular_color: [1, 1, 1], position: [0, 1, -5], reflectivity: 0.85
plane, radius: 2.0, normal: [0, 1, 0], diffuse_color: [0, 1, 0], position: [0, -1, 0], reflectivity: 0.15
light, color: [4, 4, 4], theta: 0, radial-a2: 0.125, radial-a1: 0.125, radial-a0: 0.125, position: [1, 3, -1]

camera, width: 2.0, height: 2.0
sphere, radius: 2.0, diffuse_color: [1, 0, 0], specular_color: [1, 1, 1], position: [-3, 1, -5], reflectivity: 0.55
sphere, radius: 2.0, diffuse_color: [0, 1, 0], specular_color: [1, 1, 1], position: [0, 1, -12], reflectivity: 0.15
sphere, radius: 2.0, diffuse_color: [0, 0, 1], specular_color: [1, 1, 1], position: [3, 1, -7], reflectivity: 0.15
plane, normal: [0, 1, 0], diffuse_color: [0.3, 0.3, 0.3], position: [0, -1, 0], reflectivity: 0.0
light, color: [4, 4, 4], radial-a2: 0.01, radial-a1: 0.0125, radial-a0: 0.0125, position: [10, 10, -5]

camera, width: 2.0, height: 2.0
sphere, radius: 1.5, diffuse_color: [0, 0, 0], specular_color: [1, 1, 1], position: [0, 0, -3], refractivity: 0.8, reflectivity: 0.0, ior: 2.0
sphere, radius: 2.0, diffuse_color: [0, 1, 0], specular_color: [1, 1, 1], position: [0, 1, -12], reflectivity: 0.15
sphere, radius: 2.0, diffuse_color: [0, 0, 1], specular_color: [1, 1, 1], position: [3, 1, -7], reflectivity: 0.15
plane, normal: [0, 1, 0], diffuse_color: [0.3, 0.3, 0.3], position: [0, -1, 0], reflectivity: 0.0
light, color: [8, 8, 8], radial-a2: 0.01, radial-a1: 0.0125, radial-a0: 0.0125, position: [10, 10, -5]
```
- Illumination
```c
camera, width: 2.0, height: 2.0
plane, normal: [0, 1, 0], diffuse_color: [0, 1, 0], specular_color: [0, 0, 1], position: [0, -1, 0] 
sphere, radius: 2.0, diffuse_color: [1, 0, 0], specular_color: [1, 1, 1], position: [0, 1, -5]
light, color: [2, 2, 2], theta: 0, radial-a2: 0.125, radial-a1: 0.125, radial-a0: 0.125, position: [1, 3, -1]

camera, width: 3.0, height: 3.0
quadric, diffuse_color: [0.25, 0.25, 0.05], specular_color: [1, 1, 1], position: [0, 0, -2], constants: [1.0, 0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 8.0, 0.5]
plane, normal: [0, 1, 0], diffuse_color: [0.05, 0.25, 0.25], specular_color: [1, 1, 1], position: [0, -1, 0] 
light, color: [10, 10, 10], theta: 0, radial-a2: 0.125, radial-a1: 0.125, radial-a0: 0.125, position: [0, 0, 0]

camera, width: 2.0, height: 2.0
plane, normal: [0, 1, 0], diffuse_color: [0.3, 0, 0.8], specular_color: [0, 1, 1], position: [0, -1, 0] 
sphere, radius: 0.5, diffuse_color: [0, 0.5, 0.3], specular_color: [1, 1, 1], position: [-0.2, -0.5, -1.5]
light, color: [10, 10, 10], theta: 20, direction: [0, -1, -5], radial-a2: 1.125, radial-a1: 1.125, radial-a0: 1.125, position: -2, 0, -1]
```
- Raycasting
```c
camera, width: 2.0, height: 2.0
sphere, color: [0.8, 0.8, 0.0], position: [0.0, 1.0, -5.0], radius: 2.0
plane, color: [0.3, 0.3, 0.3], position: [0.0, -2.0, 0.0], normal: [0.0, 1.0, 0.0]

camera, width: 5.0, height: 5.0
sphere, color: [1.0, 1.0, 0.0], position: [-1.0, -3.0, -4.0], radius: 2
quadric, color: [0.0, 1.0, 0.0], position: [1.8, -1.9, -5.0], constants: [2.0, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0]
quadric, color: [0.8, 0.3, 0.3], position: [0.5, -1.5, -4.0], constants: [4.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.15]

camera, width: 2, height: 2
quadric, color: [0.3, 0.6, 0.5], position: [0.0, 0.0, -4.0], constants: [1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5]
plane, color: [0.6, 0.9, 0.6], position: [0.0, -4.0, 5.0], normal: [0.0, 2.0, 0.0]
sphere, color: [0.0, 0.5, 0.7], position: [0.0, 0.0, -5.0], radius: 0.5
```