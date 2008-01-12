(scene

 (camera
  (position 0 0 -5)
  (direction 0 0 1))

 (light
  (position 0 8 -1)
  (color .2 .2 .2)
  (power 30))

 (plane
  (position 0 -1 0))

 (sphere
  (position 0 0 0)))

; (for (y -.5 6.5)
;      (set green (/ (+ $y .5) 7))
;      (for (x -7 7)
;	   (set red (/ (+ $x 7) 14))
;	   (sphere
;	    (position $x $y 5)
;	    (radius 0.5)
;	    (color $red $green 0)))))
