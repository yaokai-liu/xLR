/* License
 *
 * xLR - Dynamic LR(1) Grammar Parser
 * Copyright (C) 2025 Yaokai Liu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * Project Name: xLR
 * Module Name: 
 * Filename: LRParser.h
 * Creator: Yaokai Liu
 * Create Date: 2025-05-21
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#ifndef XLR_LR_PARSER_H
#define XLR_LR_PARSER_H

#include "LRContext.h"
#include "regex.h"

uint32_t LRRule_from_regex(LRContext *context, REFER(void) target, const Regex *regexp);

#endif //XLR_LR_PARSER_H
