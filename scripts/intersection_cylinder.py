import cmath
import numpy as np
from sklearn.preprocessing import normalize

rd = np.array([1, 0.5, 1])
ro = np.array([0, 0, 0])

rd = rd / np.linalg.norm(rd)

def point_at(ro, rd, t):
	return ro + rd * t

def intersect_cylinder(ro, rd):
    a = rd[0]**2 + rd[2]**2
    b = 2 * rd[0] * ro[0] + 2 * rd[2] * ro[2]
    c = ro[0]**2 + ro[2]**2 - 1

    d = (b**2) - (4 * a * c)

    if d > 0:
    	t1 = (-b + cmath.sqrt(d)) / (2 * a)
    	t2 = (-b - cmath.sqrt(d)) / (2 * a)
    else:
    	return -1, -1

    return t1.real, t2.real

t1, t2 = intersect_cylinder(ro, rd)

print(t1, t2)

if t1 > t2:
	print(point_at(ro, rd, t1))
elif t2 > t1:
	print(point_at(ro, rd, t2))