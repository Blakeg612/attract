/*
 *
 *  Attract-Mode frontend
 *  Copyright (C) 2013-15 Andrew Mickelson
 *
 *  This file is part of Attract-Mode.
 *
 *  Attract-Mode is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Attract-Mode is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Attract-Mode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "fe_blend.hpp"

sf::BlendMode FeBlend::get_blend_mode( int blend_mode )
{
	sf::BlendMode retval;

	switch( blend_mode )
	{
		case FeBlend::Add:
			retval = sf::BlendAdd;
			break;
		case FeBlend::Screen:
			retval = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcColor);
			break;
		case FeBlend::Multiply:
			retval = sf::BlendMode(sf::BlendMode::DstColor, sf::BlendMode::OneMinusSrcAlpha);
			break;
		case FeBlend::Overlay:
			retval = sf::BlendMode(sf::BlendMode::DstColor, sf::BlendMode::SrcColor);
			break;
		case FeBlend::Premultiplied:
			retval = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha);
			break;
		case FeBlend::None:
			retval = sf::BlendNone;
			break;
		case FeBlend::Alpha:
		default:
			retval = sf::BlendAlpha;
			break;
	}

	return retval;
}

sf::Shader FeBlend::default_shader_multiplied;
sf::Shader FeBlend::default_shader_overlay;
sf::Shader FeBlend::default_shader_premultiplied;

void FeBlend::load_default_shaders()
{
	// silently fail if shaders aren't available
	if ( sf::Shader::isAvailable() )
	{
		default_shader_multiplied.loadFromMemory( DEFAULT_SHADER_GLSL_MULTIPLIED, sf::Shader::Fragment );
		default_shader_overlay.loadFromMemory( DEFAULT_SHADER_GLSL_OVERLAY, sf::Shader::Fragment );
		default_shader_premultiplied.loadFromMemory( DEFAULT_SHADER_GLSL_PREMULTIPLIED, sf::Shader::Fragment );
	}
}

sf::Shader* FeBlend::get_default_shader( int blend_mode )
{
	switch( blend_mode )
	{
		case FeBlend::Alpha:
		case FeBlend::Add:
		case FeBlend::None:
			return NULL;
		case FeBlend::Screen:
		case FeBlend::Multiply:
			return &default_shader_multiplied;
		case FeBlend::Overlay:
			return &default_shader_overlay;
		case FeBlend::Premultiplied:
			return &default_shader_premultiplied;
		default:
			return NULL;
	}
}

const char *FeBlend::DEFAULT_SHADER_GLSL_MULTIPLIED = \
	"uniform sampler2D texture;" \
	"void main(){" \
	"vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
	"gl_FragColor = gl_Color * pixel;" \
	"gl_FragColor.xyz *= gl_FragColor.w;}";

const char *FeBlend::DEFAULT_SHADER_GLSL_OVERLAY = \
	"uniform sampler2D texture;" \
	"void main(){" \
	"vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
	"gl_FragColor = gl_Color * pixel;" \
	"gl_FragColor = mix(vec4(0.5,0.5,0.5,1.0), gl_FragColor, gl_FragColor.w);}";

const char *FeBlend::DEFAULT_SHADER_GLSL_PREMULTIPLIED = \
	"uniform sampler2D texture;" \
	"void main(){" \
	"vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
	"gl_FragColor = gl_Color * pixel;" \
	"gl_FragColor.xyz *= gl_Color.w * sign(pixel.w);}";
