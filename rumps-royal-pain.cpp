#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

#ifdef _DEBUG

// Here is just stuff that we need for the analysis of the order of "*", "+"
// and "-" terms of the equation below. See "main()" for the actual code.

template <typename T>
void dbgTerm(const string & term, const T value) {
    cout << term << " = " << value << endl;
}

template <size_t N>
struct Factorial
{
    static constexpr size_t value = N * Factorial<N - 1>::value;
};
template <>
struct Factorial<0>
{
    static constexpr size_t value{ 1 };
};
template <>
struct Factorial<1>
{
    static constexpr size_t value{ 1 };
};

template <size_t N>
const array<size_t, N> range() {
    array<size_t, N> r{};
    size_t i{ 0 };
    std::generate_n(r.begin(), N, [&i]() { return i++; });
    return r;
}
template <>
const array<size_t, 0> range<0>() {
    return {};
}

template <size_t N>
array<array<size_t, N>, Factorial<N>::value> combinations(const array<size_t, N> & indices) {
    array<array<size_t, N>, Factorial<N>::value> collected{};
    auto collectedIt = collected.begin();

    auto indicesBegin = indices.cbegin();
    auto indicesEnd = indices.cend();
    for (auto indexIt = indicesBegin; indexIt != indicesEnd; indexIt++) {
        array<size_t, N - 1> nextIndices{};
        auto nextIndicesIt = copy(indicesBegin, indexIt, nextIndices.begin());
        copy(indexIt + 1, indicesEnd, nextIndicesIt);

        auto nextCombinations = combinations<N - 1>(nextIndices);
        for (auto && nextCombination : nextCombinations) {
            collectedIt->at(0) = *indexIt;
            copy(nextCombination.cbegin(), nextCombination.cend(), collectedIt->begin() + 1);
            collectedIt++;
        }
    }

    return collected;
}
template <>
array<array<size_t, 0>, 1> combinations<0>(const array<size_t, 0> &) {
    return {};
}
template <>
array<array<size_t, 1>, 1> combinations<1>(const array<size_t, 1> & indices) {
    return { array<size_t, 1>{ indices.at(0) } };
}
template <>
array<array<size_t, 2>, 2> combinations<2>(const array<size_t, 2> & indices) {
    return {
        array<size_t, 2>{ indices.at(0), indices.at(1) },
        array<size_t, 2>{ indices.at(1), indices.at(0) }
    };
}

template <typename T, size_t N>
void dbgOpValuesAnyOrder(const string && opName, const function<T(T, T)> op, const array<tuple<string, T>, N> && valueTerms, const tuple<string, T> && firstTerm = tuple<string, T>{ "", T{} }) {
    for (auto && termCombination : combinations(range<N>())) {
        string opTerm = get<0>(firstTerm);
        T opValue = get<1>(firstTerm);

        for (auto && termIndex : termCombination) {
            auto valueTerm = valueTerms.at(termIndex);
            opTerm += (opTerm.size() > 0 ? opName : "") + get<0>(valueTerm);
            opValue = op(opValue, get<1>(valueTerm));
        }

        dbgTerm(opTerm, opValue);
    }

    cout << endl;
}

#else

template <typename T, size_t N>
void dbgOpValuesAnyOrder(const string &&, const function<T(T, T)>, const array<tuple<string, T>, N> &&, const tuple<string, T> && = tuple<string, T>{ "", T{} }) {
}

#endif

int main() {
    typedef double T;
    //typedef float T;

    // Rump's Royal Pain
    // https://youtu.be/TTM_b7EJg5E?t=2071
    // -0.82739605994682136 ~= 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)
    cout << "'Rump's Royal Pain'" << setprecision(numeric_limits<T>::max_digits10) << scientific << endl
         << "Your's computer solution to r = 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)" << endl
         << "    with x=77617.0 and y=33096.0" << endl;

    // SPOILER ALERT: You will not see much difference in the partial products
    // here, no matter in which order you chain them. All possible term
    // combinations have the product near "7.25867591160010424e+19".
    T x{ 77617 };
    T xPow2 = x * x;
    T y{ 33096 };
    T yPow2 = y * y;
    T r211 = T{ 11 } * xPow2 * yPow2;
    dbgOpValuesAnyOrder<T, 3>(
        " * ",
        [](auto l, auto r) { return l * r; },
        { tuple<string, T>{ "11", T{ 11 } },
            tuple<string, T>{ "x^2", xPow2 },
            tuple<string, T>{ "y^2", yPow2 } },
        tuple<string, T>{ "", T{ 1 } });

    // SPOILER ALERT: The numbers are too large so that the term "2.0" has no
    // significance and does not contribute in any combination of the terms for
    // "r21" below. However, it matters in which order term ". - y^6" and
    // ". - 121*y^4" are applied. If "121*y^4" is subtracted first, the value
    // of "r21" is "-1.31417460695797455e+27". However, "r21" is
    // "-1.31417460695797483e+27" if "y^6" is subtracted first. Though, the
    // difference between the two values is relatively small and they both even
    // share many first digits "-1.31417460695797e+27". When the computation
    // precision is lowered from double to float, the results look very similar
    // with "-1.314174382e+27" as shared first digits or "-1.314174e+27" as
    // least common digits of both precisions.
    T yPow4 = yPow2 * yPow2;
    T yPow6 = yPow4 * yPow2;
    T r212 = T{ 121 } * yPow4;
    dbgOpValuesAnyOrder<T, 3>(
        " - ",
        [](auto l, auto r) { return l - r; },
        { tuple<string, T>{ "y^6", yPow6 },
            tuple<string, T>{ "121*y^4", r212 },
            tuple<string, T>{ "2", T{ 2 } } },
        tuple<string, T>{ "11*x^2*y^2", r211 });

    // SPOILER ALERT: Here it gets interesting. Depending on what "r21" is and
    // in which order the outer additive terms are chained, we can get
    // surprising results.

    // Case 1 and 2: If "r21" is "-1.31417460695797483e+27" due to first
    // subtracting term "y^6", than "r" is either "-1.18059162071741130e+21" or
    // "-1.39061206352774195e+21" depending on the order in which the additive
    // terms are chained. If "r1" is the last additive term in "r", or "r1" is
    // second last and the division "r4" is last, than "r" is
    // "-1.39061206352774195e+21". Though, both values are way off the
    // expected result of "-0.827...".

    T r21 = r211 - yPow6 - r212 - T{ 2 }; // = -1.31417460695797483e+27

    // Case 3 and 4: Similarly again, if "r1" is the last additive term in "r",
    // or "r1" is second last and the division "r4" is last, than "r" is
    // "9.70571177907080659e+20" or otherwise "1.18059162071741130e+21".
    // Still, both values are way off the expected result of "-0.827..." still.
    // Interestingly, the sign is now wrong too, and the both values differ a
    // little bit more than they "should".

    //T r21 = r211 - r212 - yPow6 - T{ 2 }; // = -1.31417460695797455e+27

    // Reduced precision cases: If "r21" is set to the less precise values (the
    // shared first digits) from above, than "r" will have various different
    // results with the special order of the additive terms as described above
    // still being important in the same way. Note the change in the exponent
    // for the float precision shared digits, it is "e+30" instead of "e+22".

    //T r21 = T{ -1.31417460695797e+27 }; // than "r" is either "2.95147905179352826e+22" or "2.93047700751249519e+22"
    //T r21 = T{ -1.314174382e+27 }; // than "r" is either "1.35523652854481337e+30" or "1.35523652833479307e+30"
    //T r21 = T{ -1.314174e+27 }; // than "r" is either "3.65655682702757530e+30" or "3.65655682681755500e+30"

    // The cases are for double precision computations. Changing the type "T"
    // to float in all computations results in other results that are still off
    // and differ in the exponents way more compared to double precision
    // results. All results are incorrect, pretty interesting is only that the
    // IEEE floating points cannot compute a near correct result for "r" for
    // the given equation. It also shows that for IEEE floating point
    // computations the operations "+" and "-" are not commutative in all
    // cases. Pretty cool, heh.

    T yPow8 = yPow4 * yPow4;
    T r1 = T{ 333.75 } * yPow6;
    T r2 = xPow2 * r21;
    T r3 = T{ 5.5 } * yPow8;
    T r4 = x / (T{ 2 } * y);
    dbgOpValuesAnyOrder<T, 4>(
        " + ",
        [](auto l, auto r) { return l + r; },
        { tuple<string, T>{ "333.75*y^6", r1 },
            tuple<string, T>{ "x^2*r21", r2 },
            tuple<string, T>{ "5.5*y^8", r3 },
            tuple<string, T>{ "x/(2*y)", r4 } });
    T r = r1 + r2 + r3 + r4;

    // rTest is the result of the right side of the equation for case 1 of the
    // above analysis of the term orders. We just need it to check whether we
    // totally didn't mess up the computating code somehow.
    T rTest = T{ 333.75 } * yPow6 + xPow2 * (T{ 11 } * xPow2 * yPow2 - yPow6 - T{ 121 } * yPow4 - T{ 2 }) + T{ 5.5 } * yPow8 + x / (T{ 2 } * y);
    T analyticalAnswer = T{ -0.82739605994682136 }; // actually "-0.82739605994682136..."
    cout << "is r=" << r << " | " << rTest << endl
         << "The correct analytical answer is near " << analyticalAnswer << endl
         << "Your computer is off by " << T{ 100 } * (r - analyticalAnswer) / analyticalAnswer << "%" << endl;

    if (std::signbit(r)) {
        std::cout << "At least it got the sign right!" << std::endl;
    }

    return r != analyticalAnswer;
}