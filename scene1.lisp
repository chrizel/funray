(scene

 (camera
  (position 10 10 -6)
  (direction -1 -1 1))

 (light
  (position 5 8 -5)
  (color .2 .2 .2)
  (power 30))

 (plane
  (position 0 0 0)
  (normal 0 1 0))

;;;  (sphere
;;;   (position -1 1 0))

;;;  (sphere
;;;   (position 1.5 1 0))

 (set y .5)
 (for (y -.5 6.5)
      (set green (/ (+ $y .5) 7))
      (for (x -7 7)
	   (set red (/ (+ $x 7) 14))
	   (sphere
	    (position $x $y 3)
	    (radius 0.5)
	    (color $red $green 0)))))
