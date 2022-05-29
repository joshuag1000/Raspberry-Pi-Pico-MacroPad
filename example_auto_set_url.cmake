#  The MIT License (MIT)
#
#  Copyright (c) 2022 Joshua Glass (SuperNinja_4965)
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#

set(PICO_EXAMPLE_URL_BASE "https://github.com/joshuag1000/Raspberry-Pi-Pico-MacroPad/tree/HEAD")
macro(example_auto_set_url TARGET)
    file(RELATIVE_PATH URL_REL_PATH "${PICO_EXAMPLES_PATH}" "${CMAKE_CURRENT_LIST_DIR}")
    pico_set_program_url(${TARGET} "${PICO_EXAMPLE_URL_BASE}/${URL_REL_PATH}")
endmacro()
