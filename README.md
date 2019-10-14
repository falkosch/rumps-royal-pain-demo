# rumps-royal-pain

See https://youtu.be/TTM_b7EJg5E?t=2071
    
Consider the equation `-0.82739605994682136... = 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)`. Using IEEE floating points, computing the right side of the equation will not give reliable results that are even near the expected result `-0.827...`.

The code `rumps-royal-pain.cpp` shows of the issue and provides a little analysis of the order of the "+" or "-" terms, which demonstrates the cases in which "+" and "-" are not commutative and give varying though still incorrect results.
