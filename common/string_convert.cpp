/****************************************************************************
Copyright (c) 2016 moon

http://blog.csdn.net/greatchina01

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "string_convert.h"
#include <locale>
#include <codecvt>
#include <iostream>

static mbstate_t in_cvt_state;
static mbstate_t out_cvt_state;

std::wstring moon::string_to_wstring(const std::string& s)
{
    std::locale sys_loc("");

    const size_t BUFFER_SIZE = s.size() + 1;

	std::wstring intern_buffer(BUFFER_SIZE, 0);

    const char* extern_from_next = 0;
    wchar_t* intern_to_next = 0;

    typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;

    CodecvtFacet::result cvt_rst =
        std::use_facet<CodecvtFacet>(sys_loc).in(
            in_cvt_state,
			s.c_str(),
			s.c_str() + s.size(),
			extern_from_next,
			&intern_buffer[0],
			&intern_buffer[0] + intern_buffer.size(),
			intern_to_next);

    if (cvt_rst != CodecvtFacet::ok) {
        switch(cvt_rst) {
            case CodecvtFacet::partial:
                std::cerr << "partial";
                break;
            case CodecvtFacet::error:
                std::cerr << "error";
                break;
            case CodecvtFacet::noconv:
                std::cerr << "noconv";
                break;
            default:
                std::cerr << "unknown";
        }
        std::cerr    << ", please check in_cvt_state."
                    << std::endl;
    }
    std::wstring result = intern_buffer;
    return std::move(intern_buffer);
}

std::string moon::wstring_to_string(const std::wstring& ws)
{
    std::locale sys_loc("");

    const wchar_t* src_wstr = ws.c_str();
    const size_t MAX_UNICODE_BYTES = 4;
    const size_t BUFFER_SIZE =
                ws.size() * MAX_UNICODE_BYTES + 1;

    std::string extern_buffer(BUFFER_SIZE,0);

    const wchar_t* intern_from = src_wstr;
    const wchar_t* intern_from_end = intern_from + ws.size();
    const wchar_t* intern_from_next = 0;
    char* extern_to = &extern_buffer[0];
    char* extern_to_end = extern_to + BUFFER_SIZE;
    char* extern_to_next = 0;

    typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;

    CodecvtFacet::result cvt_rst =
        std::use_facet<CodecvtFacet>(sys_loc).out(
            out_cvt_state,
            intern_from, intern_from_end, intern_from_next,
            extern_to, extern_to_end, extern_to_next);
    if (cvt_rst != CodecvtFacet::ok) {
        switch(cvt_rst) {
            case CodecvtFacet::partial:
                std::cerr << "partial";
                break;
            case CodecvtFacet::error:
                std::cerr << "error";
                break;
            case CodecvtFacet::noconv:
                std::cerr << "noconv";
                break;
            default:
                std::cerr << "unknown";
        }
        std::cerr    << ", please check out_cvt_state."
                    << std::endl;
    }

    return std::move(extern_buffer);
}

std::string moon::wstring_to_utf8( const std::wstring& src )
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(src);
}

std::wstring moon::utf8_to_wstring( const std::string& src )
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(src);
}

std::string moon::string_to_utf8(const std::string& src)
{
	return wstring_to_utf8(string_to_wstring(src));
}

std::string moon::utf8_to_string(const std::string& src)
{
	return wstring_to_string(utf8_to_wstring(src));
}
