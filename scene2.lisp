(scene

 (camera
  (position 28 24 -30)
  (direction -1 -1 1))

 (light
  (position 0 8 -8)
  (color .2 .2 .2)
  (power 50))

 (plane
  (position 0 0 0)
  (normal 0 1 0))

 (set pi 3.14159265
      pi2 (* $pi 2)
      acount 20
      icount 3)

 (for (i 1 $icount)
      (set c (* $i $acount))
      (for (a 1 $c)
	   (set xsin (sin (* (/ $pi2 $c) $a))
		zcos (cos (* (/ $pi2 $c) $a)))
	   (sphere
	    (position (* $xsin (* 6 $i))
		      1 
		      (* $zcos (* 6 $i)))
	    (color $xsin 
		   (- (/ $i $icount) (/ 1.0 $icount))
		   $zcos)))))


