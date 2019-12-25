// Copyright (c) 2012, 2013 Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include "types.h"

namespace liquibook
{
namespace book
{

template <class OrderPtr, template <typename...> typename Multimap>
class OrderBook;

// Callback events
//   New order accept
//     - order accept
//     - fill (2) and/or quote (if not complete)
//     - depth/bbo ?
//   New order reject
//     - order reject
//   Order fill
//     - fill (2)
//     - trade
//     - quote (2)
//     - depth/bbo ?
//   Order cancel
//     - order cancel
//     - quote
//     - depth/bbo ?
//   Order cancel reject
//     - order cancel reject
//   Order replace
//     - order replace
//     - fill (2) and/or quote (if not complete)
//     - depth/bbo ?
//   Order replace reject
//     - order replace reject

/// @brief notification from OrderBook of an event
template <typename OrderPtr, template <typename...> typename Multimap>
class Callback
{
public:
  typedef OrderBook<OrderPtr, Multimap> TypedOrderBook;

  enum CbType
  {
    cb_unknown,
    cb_order_pre_accept,
    cb_order_accept,
    cb_order_reject,
    cb_order_fill,
    cb_order_cancel,
    cb_order_cancel_reject,
    cb_order_replace,
    cb_order_replace_reject,
    cb_book_update
  };

  enum FillFlags
  {
    ff_neither_filled = 0,
    ff_inbound_filled = 1,
    ff_matched_filled = 2,
    ff_both_filled = 4
  };

  Callback();

  /// @brief create a new accept callback
  static Callback<OrderPtr, Multimap> accept(const OrderPtr &order, Quantity quantit);
  static Callback<OrderPtr, Multimap> pre_accept(const OrderPtr &order);
  /// @brief create a new reject callback
  static Callback<OrderPtr, Multimap> reject(const OrderPtr &order,
                                             const char *reason);
  /// @brief create a new fill callback
  static Callback<OrderPtr, Multimap> fill(const OrderPtr &inbound_order,
                                           const OrderPtr &matched_order,
                                           const Quantity &fill_qty,
                                           const Price &fill_price,
                                           FillFlags fill_flags);
  /// @brief create a new cancel callback
  static Callback<OrderPtr, Multimap> cancel(const OrderPtr &order,
                                             const Quantity &open_qty);
  /// @brief create a new cancel reject callback
  static Callback<OrderPtr, Multimap> cancel_reject(const OrderPtr &order,
                                                    const char *reason);
  /// @brief create a new replace callback
  static Callback<OrderPtr, Multimap> replace(const OrderPtr &order,
                                              const Quantity &curr_open_qty,
                                              const int32_t &size_delta,
                                              const Price &new_price);
  /// @brief create a new replace reject callback
  static Callback<OrderPtr, Multimap> replace_reject(const OrderPtr &order,
                                                     const char *reason);

  static Callback<OrderPtr, Multimap> book_update(const TypedOrderBook *book = nullptr);
  CbType type;
  OrderPtr order;
  OrderPtr matched_order;
  Quantity quantity;
  Price price;
  uint8_t flags;
  int32_t delta;
  const char *reject_reason;
};

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap>::Callback()
    : type(cb_unknown),
      order(nullptr),
      matched_order(nullptr),
      quantity(0),
      price(0),
      flags(0),
      delta(0),
      reject_reason(nullptr)
{
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::accept(
    const OrderPtr &order, Quantity quantity)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_accept;
  result.order = order;
  result.quantity = quantity;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::pre_accept(
    const OrderPtr &order)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_pre_accept;
  result.order = order;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::reject(
    const OrderPtr &order,
    const char *reason)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_reject;
  result.order = order;
  result.reject_reason = reason;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::fill(
    const OrderPtr &inbound_order,
    const OrderPtr &matched_order,
    const Quantity &fill_qty,
    const Price &fill_price,
    FillFlags fill_flags)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_fill;
  result.order = inbound_order;
  result.matched_order = matched_order;
  result.quantity = fill_qty;
  result.price = fill_price;
  result.flags = fill_flags;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::cancel(
    const OrderPtr &order,
    const Quantity &open_qty)
{
  // TODO save the open qty
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_cancel;
  result.order = order;
  result.quantity = open_qty;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::cancel_reject(
    const OrderPtr &order,
    const char *reason)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_cancel_reject;
  result.order = order;
  result.reject_reason = reason;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::replace(
    const OrderPtr &order,
    const Quantity &curr_open_qty,
    const int32_t &size_delta,
    const Price &new_price)
{
  // TODO save the order open qty
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_replace;
  result.order = order;
  result.quantity = curr_open_qty;
  result.delta = size_delta;
  result.price = new_price;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap> Callback<OrderPtr, Multimap>::replace_reject(
    const OrderPtr &order,
    const char *reason)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_order_replace_reject;
  result.order = order;
  result.reject_reason = reason;
  return result;
}

template <typename OrderPtr, template <typename...> typename Multimap>
Callback<OrderPtr, Multimap>
Callback<OrderPtr, Multimap>::book_update(const OrderBook<OrderPtr, Multimap> *book)
{
  Callback<OrderPtr, Multimap> result;
  result.type = cb_book_update;
  return result;
}

} // namespace book
} // namespace liquibook
