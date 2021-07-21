#ifndef NURI_EVALUATOR_HPP
#define NURI_EVALUATOR_HPP

#include <cstddef>
#include <utility>

// nuri evaluator
namespace ne {

namespace index {

namespace details {

template <std::size_t A, std::size_t CurrentIndex, std::size_t... Seqs>
struct dropOneByOne {

  static constexpr auto make(std::index_sequence<CurrentIndex, Seqs...>) noexcept {
    return dropOneByOne<A, Seqs...>::make(std::index_sequence<Seqs...>());
  }
  
};

template <std::size_t A, std::size_t... Seqs>
struct dropOneByOne<A, A, Seqs...>  {
  static constexpr auto make(std::index_sequence<A, Seqs...>) noexcept {
    return std::index_sequence<A, Seqs...>();
  }
};

template <std::size_t A, std::size_t... Seqs>
static constexpr auto callDropper(std::index_sequence<Seqs...> is) noexcept {
  return dropOneByOne<A, Seqs...>::make(is);
}

} // ns details

template <std::size_t A, std::size_t B>
struct __fromAtoB {

  static constexpr auto get(void) noexcept {
    return details::callDropper<A>(std::make_index_sequence<B>());
  }

};

template <std::size_t B>
struct __fromAtoB<0, B> {
  static constexpr auto get(void) noexcept {
    return std::make_index_sequence<B>();
  }
};

template <std::size_t A, std::size_t B>
static constexpr auto fromAtoB(void) noexcept {
  return __fromAtoB<A, B>::get();
}

} // ns index

namespace ch {

static constexpr auto isSpace(const char32_t ch) noexcept {
  return ch == ' ';
}

} // ns ch

namespace str {

template <std::size_t N>
static constexpr auto getEndCharacter(const char32_t (&arr)[N]) noexcept {
  // arr[N - 1] = '\0'
  return arr[N - 2];
}

namespace details {

template <bool If>
struct __doIf {

  template <std::size_t I, typename Predicate, std::size_t N>
  static constexpr auto make(const char32_t (&arr)[N], Predicate && pred) noexcept {
    return __doIf<pred(I)>::make<I + 1>(arr, std::forward(pred));
  }

};

template <typename Predicate, std::size_t N>
static constexpr auto doIf(const char32_t (&arr)[N], Predicate && pred) noexcept {
  return __doIf<Predicate(0)>::make<pred(), 0>(arr, std::forward(pred));
}

template <typename Behavior, typename Continual, std::size_t N, std::size_t CurrentIndex, std::size_t... Seq>
static constexpr auto __splitUntilSpace(const char32_t (&arr)[N], std::index_sequence<CurrentIndex, Seq...> seq, Continual && funcContinual) noexcept {
  doIf(arr, )
}

} // ns details

template <typename Behavior, typename Continual, std::size_t N>
static constexpr auto splitUntilSpace(const char32_t (&arr)[N], Behavior && funcSplit, Continual && funcContinual = details::__splitUntilSpace) noexcept {
  return details::__splitUntilSpace(arr, std::make_index_sequence<N - 1>(), std::forward(funcSplit), std::forward(funcContinual));
}

} // ns str

template <std::size_t N>
static constexpr auto eval(const char32_t (&arr)[N]) noexcept {
  return str::getEndCharacter(arr);
}

} // ns ne

#endif