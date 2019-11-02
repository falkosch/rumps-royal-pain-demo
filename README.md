# rumps-royal-pain

See https://youtu.be/TTM_b7EJg5E?t=2071

Rump's Royal Pain is investigating numerical computation issues of IEEE 754 floating points and how _unums_ can solve it. (See "THE END OF NUMERICAL ERROR" by John L. Gustafson, Ph.D; https://web.archive.org/web/20180729032331/http://arith22.gforge.inria.fr/slides/06-gustafson.pdf)

Consider the equation `-0.82739605994682136... = 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)` with `x=77617` and `y=33096`. Using IEEE floating points, computing the right side of the equation will not give reliable results that are even near the expected result `-0.827...`.

The code `rumps-royal-pain.cpp` shows of the issue and provides a little analysis of the order of the "+" or "-" terms, which demonstrates the cases in which "+" and "-" are not commutative and give varying though still incorrect results.

Application of _unums_ is not demonstrated.
