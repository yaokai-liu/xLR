/* License
 *
 * ${PROJ_DESCRIPTION}
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
 * Module Name: test
 * Filename: test.c
 * Creator: Yaokai Liu
 * Create Date: 2025-06-03
 * Copyright (c) 2025 Yaokai Liu. All rights reserved.
 **/

#include "tokenize/test-tokenize.h"


int main() {
  uint32_t n_failed = 0;

  n_failed += test_action_tokenize();
  n_failed += test_pattern_tokenize();

  return (int) n_failed;
}
