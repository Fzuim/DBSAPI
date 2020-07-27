/** \string_utility.h
*	���ļ��У�ʵ���ַ���������
*
*	\author Fzuim
*
*	�汾��ʷ
*	\par 2019-9-20
*	
*/

#ifndef _COMMON_STRING_UTILITY_H_
#define _COMMON_STRING_UTILITY_H_

#include <string>
#include <vector>

typedef UINT8 uint8_t;

namespace Fzuim {


	class StringUtility {
	public:
		static void Split(const std::string& str,
			const std::string& delim,
			std::vector<std::string>* result);

		// �ж��ַ���str�Ƿ�����prefix��ͷ
		static bool StartsWith(const std::string& str, const std::string& prefix);

		static bool EndsWith(const std::string& str, const std::string& suffix);

		static std::string& Ltrim(std::string& str); // NOLINT

		static std::string& Rtrim(std::string& str); // NOLINT

		static std::string& Trim(std::string& str); // NOLINT

		static void Trim(std::vector<std::string>* str_list);

		// �Ӵ��滻
		static void string_replace(const std::string &sub_str1,
			const std::string &sub_str2, std::string *str);

		static void UrlEncode(const std::string& src_str, std::string* dst_str);

		static void UrlDecode(const std::string& src_str, std::string* dst_str);

		// ��Сдת��
		static void ToUpper(std::string* str);

		static void ToLower(std::string* str);

		// ȥͷȥβ
		static bool StripSuffix(std::string* str, const std::string& suffix);

		static bool StripPrefix(std::string* str, const std::string& prefix);

		// bin��hexת��
		static bool Hex2Bin(const char* hex_str, std::string* bin_str);

		static bool Bin2Hex(const char* bin_str, std::string* hex_str);

		// խ�ֽںͿ��ֽڶ�ת
		static std::wstring str2wstr(const std::string& src_str);

		static std::string wstr2str(const std::wstring& src_wstr);

		// UTF-8�����ת
		static std::wstring utf8str2wstr(const std::string& src_utf8str);

		static std::string wstr2utf8str(const std::wstring& src_wstr);
	};

} // namespace Fzuim


#endif // _COMMON_STRING_UTILITY_H_
