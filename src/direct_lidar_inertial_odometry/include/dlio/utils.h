/***********************************************************
 *                                                         *
 * Copyright (c)                                           *
 *                                                         *
 * The Verifiable & Control-Theoretic Robotics (VECTR) Lab *
 * University of California, Los Angeles                   *
 *                                                         *
 * Authors: Kenny J. Chen, Ryan Nemiroff, Brett T. Lopez   *
 * Contact: {kennyjchen, ryguyn, btlopez}@ucla.edu         *
 *                                                         *
 ***********************************************************/

#pragma once

#include "rclcpp/rclcpp.hpp"

namespace dlio {

    template <typename T>
    struct identity { using type = T; };

    template <typename T>
    void declare_param(
        rclcpp::Node* node,
        const std::string& param_name,
        T& param,
        const typename identity<T>::type& default_value)
    {
        try {
            // 이미 YAML에서 선언된 파라미터면 여기서 예외가 남
            node->declare_parameter<T>(param_name, default_value);
        } catch (const rclcpp::exceptions::ParameterAlreadyDeclaredException&) {
            // 이미 선언된 경우는 그냥 무시
        }

        // 실제 값 읽기 (없으면 default 사용)
        if (!node->get_parameter(param_name, param)) {
            param = default_value;
        }
    }

}  // namespace dlio

