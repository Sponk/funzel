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

#include "../Tensor.hpp"

namespace funzel
{
namespace nn
{

typedef std::vector<Tensor> Parameters;

#define FUNZEL_STRINGIFY(s) FUNZEL_STRINGIFY_IMPL(s)
#define FUNZEL_STRINGIFY_IMPL(s) #s

#define FUNZEL_MODULE(modname)\
virtual const char* name() const { return FUNZEL_STRINGIFY(modname); }\
static const char* Name() { return FUNZEL_STRINGIFY(modname); }

class FUNZEL_API Module
{
public:

	FUNZEL_MODULE(Module)

	virtual ~Module() = default;
	virtual Tensor forward(const Tensor& input) = 0;
	virtual Tensor backward(const Tensor& input) = 0;

	virtual void defaultInitialize();
	virtual void to(const std::string& device);
	
	Parameters& parameters() { return m_parameters; }
	const Parameters& parameters() const { return m_parameters; }

	Tensor operator()(const Tensor& input) { return forward(input); }

protected:
	Parameters m_parameters;
};

typedef std::shared_ptr<Module> ModuleRef;

}
}
