# Simulation Of Flock Boids

Simulation made for student project, use for movement of large NPC group, this is a global test for understanding and re-use.

WIP : one of my objective with this project was to auto select leader boid to lead other.

# Result
![](https://github.com/Gauthier-Damien/FlockBoid/assets/172384330/24abb735-8656-4036-bc2a-f57aec021967.gif)


# How To Use It

On scene, drop A boidSpawnerBP in Content/BP

![image](https://github.com/user-attachments/assets/06c2405d-c9da-471b-9d3d-5bfe2868e5ef)


Here, under Spawner Section, You can choose the number of bird you want to spawn, the actor to spawn (BoidBP) and if they spawn in the same position or not (Center in parameter).

![image](https://github.com/user-attachments/assets/4e06a4e7-ea9c-4da2-bbae-6257fa179dfd)

All the boid will spawn as Sbire, you can change their parameters in the BoidDataSbire (Content/BP/Assets).

![image](https://github.com/user-attachments/assets/85561e94-1847-4857-a2ad-8e5f3b4b411d)

# Parameters :

  Global (parameters for basic behavior)
  -  Speed , Control speed of boids
  - Speed Variation, control the speed offset each boid can get from other attraction
  - Flock radius (WIP)
  - Bound Size (WIP)
  - Time Begofe Leader Switch (WIP)
  - Number for Leader Flock (WIP)
  - Center, the center of map, no Z count
  - Snap to 0, true if you want the boid to ingnore Z axis
 Debug (parameter for trail):
  - Draw Debug (deperecated)
  - Default trail color, trail color for spawn

 Boid :
 - Separation Radius, push away with [Separation Strength] other boids within this radius
 - Separation Strenghth, Strenght apply on other bois within [Separation Radius]
 - Alignement Radius, ajust trajectory with [Alignement Strength] other boids within this radius
 - Alignement Strenghth, Strenght apply on other bois within [Alignement Radius]
 - Coherence Radius, attracts with [Coherence Strength] other boids within this radius
 - Coherence Strenghth, Strenght apply on other bois within [Coherence Radius]

# Sources

Flock Boid, Craig Reynolds, 1986 
Source : https://fr.wikipedia.org/wiki/Boids / https://medium.com/fragmentblog/simulating-flocking-with-the-boids-algorithm-92aef51b9e00 / https://www.youtube.com/@turingcode
