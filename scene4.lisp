(scene

 (camera
  (position 0 1 -20)
  (direction 0 0 1))

 (light
  (position 5 8 -5)
  (color .2 .2 .2)
  (power 30))

 (plane
  (position 0 0 0)
  (normal 0 1 0))

 (for (i -5 5 2)
      (sphere
       (position $i 1 0)))

