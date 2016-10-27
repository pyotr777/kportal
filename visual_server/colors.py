def convert(r, g, b):
    r_ = r/255.
    g_ = g/255.
    b_ = b/255.
    return r_, g_, b_

r1, g1, b1 = convert( 55, 167, 255)
r2, g2, b2 = convert(255, 225, 224)
r3, g3, b3 = convert(127, 255, 201)

print "{} {} {}".format(r1, r2 ,r3)
print "{} {} {}".format(g1, g2 ,g3)
print "{} {} {}".format(b1, b2 ,b3)