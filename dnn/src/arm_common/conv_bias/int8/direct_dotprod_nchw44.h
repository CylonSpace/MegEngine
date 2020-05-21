/**
 * \file dnn/src/arm_common/conv_bias/int8/direct_dotprod_nchw44.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2020 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#if __ARM_FEATURE_DOTPROD

#pragma once

#include "src/arm_common/conv_bias/opr_impl.h"

namespace megdnn {
namespace arm_common {
namespace direct_dotprod_nchw44 {

using BiasMode = ConvBiasForward::BiasMode;

/**
 * @brief : do direct conv with no side effect
 *          input buffer's size is  [ih, iw]
 *          output buffer's size is [oh, ow]
 *          filter layout is [OC/4, IC/4, FH, FW, 4, 4]
 *
 * @param : [output ptr] dst
 *          [input]      oh         -> dst rows
 *          [input]      ow         -> dst cols
 *          [input ptr]  src
 *          [input]      ih         -> rows of src used by this this kernel
 *          [input]      iw         -> src step in elements [iw2]
 *          [input ptr]  filter
 *          [input ptr]  bias
 *          [input]      oh_size    -> rows of result generated by this kernel
 *          [input]      oc         -> output channels
 *          [input]      ic         -> intput channels
 *          [input]      op         -> post process operator
 * @return  none
 */

#define KERN(filter_size)                                                     \
    template <typename dst_type, BiasMode bias_mode, typename Op, int stride> \
    void conv_direct_##filter_size##x##filter_size##_int8_nchw44(             \
            dst_type* dst, const int oh, const int ow, const int8_t* src,     \
            const int ih, const int iw, const int8_t* weight,                 \
            const int32_t* bias, const int oh_size, const int oc,             \
            const int ic, const Op& op)

KERN(2);
KERN(3);
KERN(5);
KERN(7);

#undef KERN
/**
 * @brief : copy data from src to dst for direct conv with no side effect
 * @param : [output ptr] dst
 *          [input]      dst_step   -> step of dst in numbers of elements
 *          [input ptr]  src
 *          [input]      src_step   -> step of src in numbers of elements
 *          [input]      ic         -> input channels
 *          [input]      ic_step    -> step of ic in numbers of elements
 *          [input]      ih         -> totle rows to copy
 *          [input]      pad_left   -> cols padding at left
 *          [input]      pad_right  -> cols padding at right
 *          [input]      pad_top    -> rows padding at top
 *          [input]      pad_bottom -> rows padding at bottom
 * @return  none
 */
template <int stride>
void copy_packed_src_int8_nchw44(int8_t* dst, const int dst_step,
                                 const int8_t* src, const int src_step,
                                 const int ic, const int ic_step, const int ih,
                                 const int pad_left, const int pad_right,
                                 const int pad_top, const int pad_bottom);

}  // namespace direct_dotprod_nchw44
}  // namespace arm_common
}  // namespace megdnn

#endif

//vim: syntax=cpp.doxygen