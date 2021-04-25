// Copyright (C) 2019 The Ohio State University
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "collaborate/util.h"

namespace osse {
namespace collaborate {
namespace util {

double MinimumRoot(const Polynomial& _coefficients) {
  Polynomial reverse = _coefficients;
  std::reverse(reverse.begin(), reverse.end());
  Polynomial roots = ComplexRoots(reverse);
  double minimum = std::numeric_limits<double>::max();
  for (auto root : roots) {
    double real_root = root.real();
    if ((real_root > 0) && (real_root < minimum)) {
      minimum = real_root;
    }
  }
  return minimum;
}

Polynomial ComplexRoots(const Polynomial& _coefficients) {
  constexpr bool kPolish = true;
  constexpr double kEpsilon = 0.000002;
  Polynomial roots_ = {0.0, 0.0, 0.0, 0.0, 0.0};
  Polynomial poly = _coefficients;
  int length = poly.size();
  for (int i = length - 1; i >= 1; --i) {
    Complex root;
    Laguerre(poly, i, &root);
    if (std::abs(root.imag()) <= (2.0 * kEpsilon * std::abs(root.real()))) {
      root = {root.real(), 0.0};
    }
    roots_.at(i) = root;
    Complex current = poly[i];
    for (int j = i - 1; j >= 0; --j) {
      Complex temporary = poly[j];
      poly[j] = current;
      current = (root * current) + temporary;
    }
  }
  if (kPolish) {
    for (int i = 1; i < length; ++i) {
      Laguerre(_coefficients, length-1, &(roots_.at(i)));
    }
  }
  return roots_;
}

void Laguerre(const Polynomial& _coefficients,
              const int& _degree,
              Complex* _value) {
  Polynomial copy;
  for (int i = 0; i <= _degree; ++i) {
    copy.push_back(_coefficients.at(i));
  }
  constexpr double kEpsilon = 0.00004;
  constexpr int kMaxSomething = 8;
  constexpr int kMaxAnother = 10;
  constexpr int kMaxIterations = kMaxSomething * kMaxAnother;
  std::vector<double> frac {0.0, 0.5, 0.25, 0.75, 0.13, 0.38, 0.62, 0.88, 1.0};
  Complex d {0, 0};
  Complex f {0, 0};
  int iteration = 1;
  bool flag = false;
  while (!flag && iteration <= kMaxIterations) {
    Complex b = copy.at(_degree);
    double err = std::abs(b);
    d = {0.0, 0.0};
    f = {0.0, 0.0};
    double abx = std::abs(*_value);
    for (int i = _degree-1; i >= 0; --i) {
      f = ((*_value) * f) + d;
      d = ((*_value) * d) + b;
      b = ((*_value) * b) + copy.at(i);
      err = std::abs(b) + (abx * err);
    }
    err *= kEpsilon;
    if (std::abs(b) <= err) {
      flag = true;
    } else {
      // The generic case: use Laguerre's formula.
      Complex g = d / b;
      Complex g2 = g * g;
      Complex h = g2 - (2.0 * (f / b));
      Complex sq = std::sqrt((_degree - 1.0)
                             * ((static_cast<double>(_degree) * h) - g2));
      Complex gp = g + sq;
      Complex gm = g - sq;
      double abp = std::abs(gp);
      double abm = std::abs(gm);
      if (static_cast<float>(abp) < static_cast<float>(abm)) {
        gp = gm;
      }
      Complex dx;
      if (std::max(abp, abm) > 0.0) {
        dx = Complex(_degree, 0.0) / gp;
      } else {
        dx = (1 + abx) * Complex(std::cos(iteration), std::sin(iteration));
      }
      Complex x1 = ((*_value) - dx);
      if ((*_value).real() == x1.real() && (*_value).imag() == x1.imag()) {
        // Converged.
        flag = true;
      } else {
        if (iteration % kMaxAnother) {
          (*_value) = x1;
        } else {
          (*_value) = ((*_value)
                       - (frac.at(static_cast<int>(iteration/kMaxAnother))
                          * dx));
        }
      }
    }
    ++iteration;
  }
  if (iteration == kMaxIterations) {
    // ERROR
  }
}

}  // namespace util
}  // namespace collaborate
}  // namespace osse
