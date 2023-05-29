/* 
 * This file is part of Funzel.
 * Copyright (c) 2022 Yannick Pflanzer.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "Module.hpp"

namespace funzel
{
namespace nn
{

class FUNZEL_API Linear : public Module
{
public:
	FUNZEL_MODULE(Linear)
	
	Linear(size_t in, size_t out, bool bias = true);

	Tensor forward(const Tensor& input) override;
	Tensor backward(const Tensor& input) override;

	Tensor& bias() { return m_parameters.at(1); }
	Tensor& weights() { return m_parameters.at(0); }
};

}
}
