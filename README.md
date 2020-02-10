# rumps-royal-pain

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=rumps-royal-pain-demo&metric=alert_status)](https://sonarcloud.io/dashboard?id=rumps-royal-pain-demo)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=rumps-royal-pain-demo&metric=ncloc)](https://sonarcloud.io/dashboard?id=rumps-royal-pain-demo)

See https://youtu.be/TTM_b7EJg5E?t=2071

Rump's Royal Pain investigates numerical computation issues of IEEE 754 floating points and how _unums_ can solve them. (See "THE END OF NUMERICAL ERROR" by John L. Gustafson, Ph.D; https://web.archive.org/web/20180729032331/http://arith22.gforge.inria.fr/slides/06-gustafson.pdf)

Consider the equation `-0.82739605994682136... = 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)` with `x=77617` and `y=33096`. Using IEEE floating points, computing the right side of the equation will not give reliable results that are even near the expected result `-0.827...`.

Have a look at a [3D plot](https://www.wolframalpha.com/input/?i=plot+333.75y%5E6%2Bx%5E2*%2811x%5E2*y%5E2-y%5E6-121y%5E4-2%29%2B5.5y%5E8%2Bx%2F%282y%29%2C+x%3D77616.9..77617.1%2C+y%3D33095.9..33096.1) of the right side of the above equation as a function `z=f(x,y)` in the range of `x in [77616.9, 77617.1]` and `y in [33095.9, 33096.1]`. The plot looks a little bumpy, however you can clearly see that the curve of the function intersects with the zero level of the z-plane. You can also see that the scale of the z-axis indicates that gradients in that plot are pretty large. Concludingly, even tiny differences in intermediate values, e.g. rounding errors in the computation of the `z=f(x,y)`, result in large deviations of the value of z.

The code `rumps-royal-pain.cpp` shows of the issue. It also provides a little analysis of the order of the "+" or "-" terms, which demonstrates that "+" and "-" (i.e. `(a - b) - c` and `(a - c) - b`) terms are not associative due to rounding errors.

Application of _unums_ is not demonstrated.
