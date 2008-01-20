/*funray - yet another raytracer
  Copyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
  Simon Goller (neosam@gmail.com).

  This program is free software; you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published 
  by the Free Software Foundation; either version 3 of the License, 
  or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License along 
  with this program; if not, see <http://www.gnu.org/licenses/>. */

#include <cmath>

#include "camera.h"
#include "vector.h"

Camera::Camera(const vec &pos, 
	       const vec &dir, 
	       const vec &up,
	       float hlen, 
	       float vlen)
    : pos(pos), 
      dir(dir.normal()), 
      up(up.normal()),
      hlen(hlen), 
      vlen(vlen) 
{
}

Camera::~Camera()
{
}

const vec Camera::dirVecFor(int x, int y, int width, int height) const {
    vec v = vec(-(hlen / 2) + (hlen / width)  * x,
		(vlen / 2) - (vlen / height) * y,
		1.333).normal();

    vec a = vec(0.0, 0.0, 1.0);
    vec b = dir;

    // angle between vector a and b:
    float ct = a.dot(b);
    float st = sin(acos(ct));

    // calculate perpendicular vector from a and b
    vec n = xproduct(a, b).normal();

    // multiply vector v with rotation matrix
    vec r = vec( v.x * (n.x * n.x * (1 - ct) + ct) +
		 v.y * (n.x * n.y * (1 - ct) - n.z * st) +
		 v.z * (n.x * n.z * (1 - ct) + n.y * st),

		 v.x * (n.x * n.y * (1 - ct) + n.z * st) +
		 v.y * (n.y * n.y * (1 - ct) + ct) +
		 v.z * (n.y * n.z * (1 - ct) - n.x * st),

		 v.x * (n.x * n.z * (1 - ct) - n.y * st) +
		 v.y * (n.y * n.z * (1 - ct) + n.x * st) +
		 v.z * (n.z * n.z * (1 - ct) + ct) );

    return r;
};
