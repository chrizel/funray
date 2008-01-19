(scene

 (camera
  (position 18 18 -8)
  (direction -1 -1 1))

 (light
  (position 0 8 -8)
  (color .2 .2 .2)
  (power 50))

 (set count 10)
 (for (z 1 $count)
      (for (y 1 $count)
	   (for (x 1 $count)
		(sphere
		 (position $x $y $z)
		 (radius .5))))))




