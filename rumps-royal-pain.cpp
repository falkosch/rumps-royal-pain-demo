#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

using namespace std;

#ifdef _DEBUG

// Here is just stuff that we need for the analysis of the order of "*", "+"
// and "-" terms of the equation below. See "main()" for the actual code.

template <typename T>
constexpr void displayTermAndValue(const string_view && term, const T & value) noexcept {
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

template <size_t N>
constexpr array<size_t, N> range() noexcept {
    array<size_t, N> r{};
    size_t i{ 0 };
    generate_n(r.begin(), N, [&i]() { return i++; });
    return r;
}

// generates a sequence of all possible combinations of given indices
template <size_t N>
constexpr array<array<size_t, N>, Factorial<N>::value> combinations(const array<size_t, N> & indices) noexcept {
    array<array<size_t, N>, Factorial<N>::value> collected{};
    auto collectedIt = collected.begin();

    const auto indicesBegin = indices.cbegin();
    const auto indicesEnd = indices.cend();
    for (auto indexIt = indicesBegin; indexIt != indicesEnd; indexIt++) {
        array<size_t, N - 1> nextIndices{};
        const auto nextIndicesIt = copy(indicesBegin, indexIt, nextIndices.begin());
        copy(indexIt + 1, indicesEnd, nextIndicesIt);

        const auto nextCombinations = combinations<N - 1>(nextIndices);
        for (auto && nextCombination : nextCombinations) {
            collectedIt->at(0) = *indexIt;
            copy(nextCombination.cbegin(), nextCombination.cend(), collectedIt->begin() + 1);
            collectedIt++;
        }
    }

    return collected;
}
template <>
constexpr array<array<size_t, 0>, 1> combinations<0>(const array<size_t, 0> &) noexcept {
    return {};
}

template <typename T, size_t N>
constexpr void displayAllPossibleTermCombinations(
    const string_view && opName,
    const function<T(T, T)> && op,
    const array<tuple<string_view, T>, N> && valueTerms,
    const tuple<string_view, T> && firstTerm = tuple<string_view, T>{ "", 0 }) noexcept {
    for (auto && termCombination : combinations(range<N>())) {
        auto opTerm = string{ get<0>(firstTerm) };
        auto opValue = get<1>(firstTerm);

        for (auto && termIndex : termCombination) {
            const auto valueTerm = valueTerms.at(termIndex);
            opTerm += (opTerm.size() > size_t{ 0 } ? opName : "");
            opTerm += get<0>(valueTerm);
            opValue = op(opValue, get<1>(valueTerm));
        }

        displayTermAndValue(opTerm, opValue);
    }

    cout << endl;
}

#else

template <typename T, size_t N>
constexpr void displayAllPossibleTermCombinations(
    const string_view &&,
    const function<T(T, T)>,
    const array<tuple<string_view, T>, N> &&,
    const tuple<string_view, T> && = tuple<string_view, T>{ "", {} }) noexcept {
}

#endif

int main() {
    typedef double ValueType;
    //typedef float ValueType;

    // Rump's Royal Pain
    // https://youtu.be/TTM_b7EJg5E?t=2071
    // -0.82739605994682136 ~= 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)
    cout << "'Rump's Royal Pain'" << setprecision(numeric_limits<ValueType>::max_digits10) << scientific << endl
         << "Your's computer solution to r = 333.75 * y^6 + x^2 * (11.0 * x^2 * y^2 - y^6 - 121.0 * y^4 - 2.0) + 5.5 * y^8 + x / (2.0 * y)" << endl
         << "    with x=77617.0 and y=33096.0" << endl;

    // SPOILER ALERT: You will not see much difference in the partial products
    // here, no matter in which order you chain them. All possible term
    // combinations have the product near "7.25867591160010424e+19".
    constexpr ValueType x{ 77617 };
    constexpr ValueType xPow2 = x * x;
    constexpr ValueType y{ 33096 };
    constexpr ValueType yPow2 = y * y;
    constexpr ValueType r211 = ValueType{ 11 } * xPow2 * yPow2;
    displayAllPossibleTermCombinations<ValueType, 3>(
        " * ",
        [](const auto l, const auto r) { return l * r; },
        { tuple<string_view, ValueType>{ "11", { 11 } },
            tuple<string_view, ValueType>{ "x^2", xPow2 },
            tuple<string_view, ValueType>{ "y^2", yPow2 } },
        tuple<string_view, ValueType>{ "", { 1 } });

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
    constexpr ValueType yPow4 = yPow2 * yPow2;
    constexpr ValueType yPow6 = yPow4 * yPow2;
    constexpr ValueType r212 = ValueType{ 121 } * yPow4;
    displayAllPossibleTermCombinations<ValueType, 3>(
        " - ",
        [](const auto l, const auto r) { return l - r; },
        { tuple<string_view, ValueType>{ "y^6", yPow6 },
            tuple<string_view, ValueType>{ "121*y^4", r212 },
            tuple<string_view, ValueType>{ "2", { 2 } } },
        tuple<string_view, ValueType>{ "11*x^2*y^2", r211 });

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

    constexpr ValueType r21 = r211 - yPow6 - r212 - ValueType{ 2 }; // = -1.31417460695797483e+27

    // Case 3 and 4: Similarly again, if "r1" is the last additive term in "r",
    // or "r1" is second last and the division "r4" is last, than "r" is
    // "9.70571177907080659e+20" or otherwise "1.18059162071741130e+21".
    // Still, both values are way off the expected result of "-0.827..." still.
    // Interestingly, the sign is now wrong too, and the both values differ a
    // little bit more than they "should".

    //constexpr ValueType r21 = r211 - r212 - yPow6 - ValueType{ 2 }; // = -1.31417460695797455e+27

    // Reduced precision cases: If "r21" is set to the less precise values (the
    // shared first digits) from above, than "r" will have various different
    // results with the special order of the additive terms as described above
    // still being important in the same way. Note the change in the exponent
    // for the float precision shared digits, it is "e+30" instead of "e+22".

    //constexpr ValueType r21 = ValueType{ -1.31417460695797e+27 }; // than "r" is either "2.95147905179352826e+22" or "2.93047700751249519e+22"
    //constexpr ValueType r21 = ValueType{ -1.314174382e+27 }; // than "r" is either "1.35523652854481337e+30" or "1.35523652833479307e+30"
    //constexpr ValueType r21 = ValueType{ -1.314174e+27 }; // than "r" is either "3.65655682702757530e+30" or "3.65655682681755500e+30"

    // The cases are for double precision computations. Changing the type "ValueType"
    // to float in all computations results in other results that are still off
    // and differ in the exponents way more compared to double precision
    // results. All results are incorrect, pretty interesting is only that the
    // IEEE floating points cannot compute a near correct result for "r" for
    // the given equation. It also shows that for IEEE floating point
    // computations the operations "+" and "-" are not commutative in all
    // cases. Pretty cool, heh.

    constexpr ValueType yPow8 = yPow4 * yPow4;
    constexpr ValueType r1 = ValueType{ 333.75 } * yPow6;
    constexpr ValueType r2 = xPow2 * r21;
    constexpr ValueType r3 = ValueType{ 5.5 } * yPow8;
    constexpr ValueType r4 = x / (ValueType{ 2 } * y);
    displayAllPossibleTermCombinations<ValueType, 4>(
        " + ",
        [](const auto l, const auto r) { return l + r; },
        { tuple<string_view, ValueType>{ "333.75*y^6", r1 },
            tuple<string_view, ValueType>{ "x^2*r21", r2 },
            tuple<string_view, ValueType>{ "5.5*y^8", r3 },
            tuple<string_view, ValueType>{ "x/(2*y)", r4 } });
    constexpr ValueType r = r1 + r2 + r3 + r4;

    // rTest is the result of the right side of the equation for case 1 of the
    // above analysis of the term orders. We just need it to check whether we
    // totally didn't mess up the code above somehow.
    constexpr ValueType rTest = ValueType{ 333.75 } * yPow6 + xPow2 * (ValueType{ 11 } * xPow2 * yPow2 - yPow6 - ValueType{ 121 } * yPow4 - ValueType{ 2 }) + ValueType{ 5.5 } * yPow8 + x / (ValueType{ 2 } * y);
    constexpr ValueType expectedNearAnalyticalAnswer = ValueType{ -0.82739605994682136 };
    constexpr ValueType offByInPercent = ValueType{ 100 } * (r - expectedNearAnalyticalAnswer) / expectedNearAnalyticalAnswer;
    cout << "is r=" << r << " | rTest=" << rTest << endl
         << "The correct analytical answer is near " << expectedNearAnalyticalAnswer << endl
         << "Your computer is off by " << offByInPercent << "%" << endl;

    if (signbit(r)) {
        cout << "At least it got the sign right!" << endl;
    }

    return offByInPercent < ValueType{ 2.5 };
}